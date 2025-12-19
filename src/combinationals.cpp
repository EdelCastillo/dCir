/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 *
 *    Tratamiento de info en bloques combinacionales
 *
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 *
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **************************************************************************/

#include "combinationals.h"

//Constructor, recibe un puntero a clase de tartamiento con bloques
Combinationals::Combinationals(class Blocks *cBlocks_p)
{
    m_blocks_p=cBlocks_p->m_blocks_p;
    m_paths_p=cBlocks_p->m_paths_p;
    m_cBlocks_p=cBlocks_p;
}

//puerta NOT
//una sola entrada de ancho variable
void Combinationals::NOT(int bk)
{
    bool unKnow, unKnowOld;
    int endValue, tmp, pathIn, mask;
    mask=m_cBlocks_p->andMask(m_paths_p[m_blocks_p[bk].pathOut[0]].nBits);

    pathIn=m_blocks_p[bk].pathIn[0];
    unKnow=m_paths_p[pathIn].valueEnd.unKnow;
    tmp=m_paths_p[pathIn].valueEnd.value;
    if(unKnow==false) //si el valor de entrada no es desconocido
        {tmp=~tmp; tmp&=mask;} //inversión y máscara para conservar los bits de interés
       
    unKnowOld=m_blocks_p[bk].combi.tmpValueUnknow[0];
    m_blocks_p[bk].combi.tmpValueUnknow[0]=unKnow;
    if(unKnowOld!=unKnow || m_blocks_p[bk].combi.tmpValue[0]!=tmp)                  //si el valor anterior del camino difiere del actual
        {
        m_blocks_p[bk].combi.tmpValue[0]=tmp;               //se actualiza el nuevo valor          
        m_blocks_p[bk].currentDelay=m_blocks_p[bk].delay/m_cBlocks_p->m_params_p->deltaT_blocks; //se actualiza el retardo
        if (m_blocks_p[bk].currentDelay == 0) m_blocks_p[bk].currentDelay = 1;
    }
}

//puerta AND
//Si más de una entrada, se hace la operación por columnas y la salida es del mismo ancho que las entradas
//Si sólo una entrada,   se hace la operación por bits en la fila y la salida es de un solo bit
void Combinationals::AND(int bk)
{
    bool unKnow, unKnowOld;
    int endValue, tmp, pathIn, mask;
    mask=m_cBlocks_p->andMask(m_paths_p[m_blocks_p[bk].pathOut[0]].nBits);
    
    tmp=0xFFFFFFFF; //8 bits
    int countX=0, nPathIn=m_blocks_p[bk].nPathIn;
    if (nPathIn == 1)
    {
        pathIn = m_blocks_p[bk].pathIn[0];
        mask = m_cBlocks_p->andMask(m_paths_p[pathIn].nBits);
        unKnow = m_paths_p[pathIn].valueEnd.unKnow;
        endValue = m_paths_p[pathIn].valueEnd.value;  //valor de entrada
        if (unKnow)  countX = nPathIn;      //contador de entradas desconocidas
        else if (endValue != mask) tmp = 0;
        else tmp = 1;
    }
    else
    for(int pIn=0; pIn<m_blocks_p[bk].nPathIn; pIn++) //AND sobre todas las entradas
        {
        pathIn=m_blocks_p[bk].pathIn[pIn];
        unKnow=m_paths_p[pathIn].valueEnd.unKnow;
        endValue=m_paths_p[pathIn].valueEnd.value;  //valor de entrada
        if(unKnow) {countX++; continue;}      //contador de entradas desconocidas
        else    tmp&=endValue;
        }
    tmp&=mask;//máscara 
    if(countX==nPathIn || (countX>0 && tmp!=0))  unKnow=true;    //si todas las entradas ='x', salida='x'                              
    else unKnow=false;
    
    unKnowOld=m_blocks_p[bk].combi.tmpValueUnknow[0];
    m_blocks_p[bk].combi.tmpValueUnknow[0]=unKnow;
    if(unKnowOld!=unKnow || m_blocks_p[bk].combi.tmpValue[0]!=tmp)                  //si el valor anterior del camino difiere del actual
        {
        m_blocks_p[bk].combi.tmpValue[0]=tmp;               //se actualiza el nuevo valor          
        m_blocks_p[bk].currentDelay=m_blocks_p[bk].delay/m_cBlocks_p->m_params_p->deltaT_blocks; //se actualiza el retardo
        if (m_blocks_p[bk].currentDelay == 0) m_blocks_p[bk].currentDelay = 1;
    }
}

//puerta OR
//Si más de una entrada, se hace la operación por columnas y la salida es del mismo ancho que las entradas
//Si sólo una entrada,   se hace la operación por bits en la fila y la salida es de un solo bit
void Combinationals::OR(int bk)
{
    bool unKnow, unKnowOld;
    int endValue, tmp, pathIn, mask;
    mask=m_cBlocks_p->andMask(m_paths_p[m_blocks_p[bk].pathOut[0]].nBits);
    
    tmp=0x00; //8 bits
    int countX=0, nPathIn=m_blocks_p[bk].nPathIn; //OR sobre todas las entradas
    if (nPathIn == 1)
    {
        pathIn = m_blocks_p[bk].pathIn[0];
        unKnow = m_paths_p[pathIn].valueEnd.unKnow;
        endValue = m_paths_p[pathIn].valueEnd.value;  //valor de entrada
        if (unKnow)  countX = nPathIn;      //contador de entradas desconocidas
        else if (endValue != 0) tmp = 1;
        else tmp = 0;
    }
    else
        for(int pIn=0; pIn<nPathIn; pIn++)
        {
        pathIn=m_blocks_p[bk].pathIn[pIn];      
        unKnow=m_paths_p[pathIn].valueEnd.unKnow;
        endValue=m_paths_p[pathIn].valueEnd.value;  //valor de entrada
        if(unKnow==true) {countX++; continue;}      //contador de entradas desconocidas
        else tmp|=endValue;
        }
    tmp&=mask;//máscara 
    if(countX==nPathIn || (countX>0 && tmp!=mask)) unKnow=true;        //todas las entradas ='x', salida='x'
    else unKnow=false;
    
    unKnowOld=m_blocks_p[bk].combi.tmpValueUnknow[0];
    m_blocks_p[bk].combi.tmpValueUnknow[0]=unKnow;
    if(unKnowOld!=unKnow || m_blocks_p[bk].combi.tmpValue[0]!=tmp)                  //si el valor anterior del camino difiere del actual
        {
        m_blocks_p[bk].combi.tmpValue[0]=tmp;               //se actualiza el nuevo valor          
        m_blocks_p[bk].currentDelay=m_blocks_p[bk].delay/m_cBlocks_p->m_params_p->deltaT_blocks; //se actualiza el retardo
        if (m_blocks_p[bk].currentDelay == 0) m_blocks_p[bk].currentDelay = 1;
        }                                            
    
}

//puerta XOR
//Si más de una entrada, se hace la operación por columnas y la salida es del mismo ancho que las entradas
//Si sólo una entrada,   se hace la operación por bits en la fila y la salida es de un solo bit
void Combinationals::XOR(int bk)
{
    bool unKnow, unKnowOld;
    int endValue, tmp, pathIn, mask;
    mask=m_cBlocks_p->andMask(m_paths_p[m_blocks_p[bk].pathOut[0]].nBits);
    
    tmp=0x00; //8 bits
    int countX = 0, nPathIn = m_blocks_p[bk].nPathIn;
    if (nPathIn == 1)
        {
        pathIn = m_blocks_p[bk].pathIn[0];
        mask = 1;
        unKnow = m_paths_p[pathIn].valueEnd.unKnow;
        endValue = m_paths_p[pathIn].valueEnd.value;  //valor de entrada
        if (unKnow)  countX = nPathIn;      //contador de entradas desconocidas
        else 
            for (int i = 0; i < m_paths_p[pathIn].nBits; i++)
                {
                if ((mask & endValue) != 0) countX++;
                mask <<= 1;
                }
        mask = 1; //si el bit de menos peso es '1' es impar
        if ((countX & mask) != 0) tmp = 1; //impar
        else tmp = 0; //par
        }
    else
        for(int pIn=0; pIn<m_blocks_p[bk].nPathIn; pIn++) //XOR sobre todas las entradas
        {
        pathIn=m_blocks_p[bk].pathIn[pIn];
        unKnow=m_paths_p[pathIn].valueEnd.unKnow;
        endValue=m_paths_p[pathIn].valueEnd.value;      //valor de entrada
        if(unKnow==true) break;   //si una entrada es 'x', salida='x'
        tmp ^=endValue;                                 //xor
        }
    tmp&=mask;             //máscara 
    
    unKnowOld=m_blocks_p[bk].combi.tmpValueUnknow[0];
    m_blocks_p[bk].combi.tmpValueUnknow[0]=unKnow;
    if(unKnowOld!=unKnow || m_blocks_p[bk].combi.tmpValue[0]!=tmp)                  //si el valor anterior del camino difiere del actual
        {
        m_blocks_p[bk].combi.tmpValue[0]=tmp;               //se actualiza el nuevo valor          
        m_blocks_p[bk].currentDelay=m_blocks_p[bk].delay/m_cBlocks_p->m_params_p->deltaT_blocks; //se actualiza el retardo
        if (m_blocks_p[bk].currentDelay == 0) m_blocks_p[bk].currentDelay = 1;
    }
    
}

//Puerta NAND
//Si más de una entrada, se hace la operación por columnas y la salida es del mismo ancho que las entradas
//Si sólo una entrada,   se hace la operación por bits en la fila y la salida es de un solo bit
void Combinationals::NAND(int bk)
{
    bool unKnow, unKnowOld;
    int endValue, tmp, pathIn, mask;
    mask=m_cBlocks_p->andMask(m_paths_p[m_blocks_p[bk].pathOut[0]].nBits);

    tmp=0xFFFFFFFF; //8 bits
    int countX=0, nPathIn=m_blocks_p[bk].nPathIn;
    if (nPathIn == 1)
    {
        pathIn = m_blocks_p[bk].pathIn[0];
        mask = m_cBlocks_p->andMask(m_paths_p[pathIn].nBits);
        unKnow = m_paths_p[pathIn].valueEnd.unKnow;
        endValue = m_paths_p[pathIn].valueEnd.value;  //valor de entrada
        if (unKnow)  countX = nPathIn;      //contador de entradas desconocidas
        else if (endValue != mask) tmp = 0;
        else tmp = 1;
    }
    else
        for(int pIn=0; pIn<m_blocks_p[bk].nPathIn; pIn++)   //NAND sobre todas las entradas
        {
        pathIn=m_blocks_p[bk].pathIn[pIn];
        unKnow=m_paths_p[pathIn].valueEnd.unKnow;
        endValue=m_paths_p[pathIn].valueEnd.value;      //valor de entrada
        if(unKnow==true) {countX++; continue;}          //contador de entradas desconocidas
        tmp &=endValue;                      //máscara para conservar el bit bajo
        }
    if(countX==nPathIn || (countX>0 && tmp!=0)) unKnow=true;     //si todas las entradas ='x', salida='x'                        
    else unKnow=false;
    tmp=~tmp; tmp&=mask;                        //máscara                         
    
    unKnowOld=m_blocks_p[bk].combi.tmpValueUnknow[0];
    m_blocks_p[bk].combi.tmpValueUnknow[0]=unKnow;
    if(unKnowOld!=unKnow || m_blocks_p[bk].combi.tmpValue[0]!=tmp)                  //si el valor anterior del camino difiere del actual
        {
        m_blocks_p[bk].combi.tmpValue[0]=tmp;               //se actualiza el nuevo valor          
        m_blocks_p[bk].currentDelay=m_blocks_p[bk].delay/m_cBlocks_p->m_params_p->deltaT_blocks; //se actualiza el retardo
        if (m_blocks_p[bk].currentDelay == 0) m_blocks_p[bk].currentDelay = 1;
    }
    
}

//Puerta NOR
//Si más de una entrada, se hace la operación por columnas y la salida es del mismo ancho que las entradas
//Si sólo una entrada,   se hace la operación por bits en la fila y la salida es de un solo bit
void Combinationals::NOR(int bk)
{
    bool unKnow, unKnowOld;
    int endValue, tmp, pathIn, mask;
    mask=m_cBlocks_p->andMask(m_paths_p[m_blocks_p[bk].pathOut[0]].nBits);
    
    tmp=0x00; //8 bits
    int countX=0, nPathIn=m_blocks_p[bk].nPathIn;
    if (nPathIn == 1)
        {
        pathIn = m_blocks_p[bk].pathIn[0];
        unKnow = m_paths_p[pathIn].valueEnd.unKnow;
        endValue = m_paths_p[pathIn].valueEnd.value;  //valor de entrada
        if (unKnow)  countX = nPathIn;      //contador de entradas desconocidas
        else if (endValue != 0) tmp = 1;
        else tmp = 0;
        }
    else
        for(int pIn=0; pIn<nPathIn; pIn++)                  //NOR sobre todas las entradas
        {
        pathIn=m_blocks_p[bk].pathIn[pIn];
        unKnow=m_paths_p[pathIn].valueEnd.unKnow;
        endValue=m_paths_p[pathIn].valueEnd.value;      //valor de entrada
        if(unKnow==true) {countX++; continue;}          //contador de entradas desconocidas
        else tmp |=endValue; 
        }
    if(countX==nPathIn || (countX>0 && tmp!=mask)) unKnow=true; //todas las entradas ='x', salida='x'
    else unKnow=false;
    tmp=~tmp; tmp&=mask;                         //máscara                        
    
    unKnowOld=m_blocks_p[bk].combi.tmpValueUnknow[0];
    m_blocks_p[bk].combi.tmpValueUnknow[0]=unKnow;
    if(unKnowOld!=unKnow || m_blocks_p[bk].combi.tmpValue[0]!=tmp)                  //si el valor anterior del camino difiere del actual
        {
        m_blocks_p[bk].combi.tmpValue[0]=tmp;               //se actualiza el nuevo valor          
        m_blocks_p[bk].currentDelay=m_blocks_p[bk].delay/m_cBlocks_p->m_params_p->deltaT_blocks; //se actualiza el retardo
        if (m_blocks_p[bk].currentDelay == 0) m_blocks_p[bk].currentDelay = 1;
    }
    
}

//Puerta XOR
//Si más de una entrada, se hace la operación por columnas y la salida es del mismo ancho que las entradas
//Si sólo una entrada,   se hace la operación por bits en la fila y la salida es de un solo bit
void Combinationals::XNOR(int bk)
{
    bool unKnow, unKnowOld;
    int endValue, tmp, pathIn, mask;
    mask=m_cBlocks_p->andMask(m_paths_p[m_blocks_p[bk].pathOut[0]].nBits);
    
    tmp=0x00; //8 bits
    int countX = 0, nPathIn = m_blocks_p[bk].nPathIn;
    if (nPathIn == 1)
    {
        pathIn = m_blocks_p[bk].pathIn[0];
        mask = 1;
        unKnow = m_paths_p[pathIn].valueEnd.unKnow;
        endValue = m_paths_p[pathIn].valueEnd.value;  //valor de entrada
        if (unKnow)  countX = nPathIn;      //contador de entradas desconocidas
        else
            for (int i = 0; i < m_paths_p[pathIn].nBits; i++)
            {
                if ((mask & endValue) != 0) countX++;
                mask <<= 1;
            }
        mask = 1; //si el bit de menos peso es '1' es impar
        if ((countX & mask) != 0) tmp = 1; //impar
        else tmp = 0; //par
    }
    else
        for(int pIn=0; pIn<m_blocks_p[bk].nPathIn; pIn++) //XOR sobre todas las entradas
        {
        pathIn=m_blocks_p[bk].pathIn[pIn];
        unKnow=m_paths_p[pathIn].valueEnd.unKnow;
        endValue=m_paths_p[pathIn].valueEnd.value;      //valor de entrada
        if(unKnow==true) break;   //si una entrada es 'x', salida='x'
        tmp ^=endValue;                                 //xor
        }
    tmp=~tmp; tmp&=mask; //negación y máscara 
    
    unKnowOld=m_blocks_p[bk].combi.tmpValueUnknow[0];
    m_blocks_p[bk].combi.tmpValueUnknow[0]=unKnow;
    if(unKnowOld!=unKnow || m_blocks_p[bk].combi.tmpValue[0]!=tmp)                  //si el valor anterior del camino difiere del actual
        {
        m_blocks_p[bk].combi.tmpValue[0]=tmp;               //se actualiza el nuevo valor          
        m_blocks_p[bk].currentDelay=m_blocks_p[bk].delay/m_cBlocks_p->m_params_p->deltaT_blocks; //se actualiza el retardo
        if (m_blocks_p[bk].currentDelay == 0) m_blocks_p[bk].currentDelay = 1;
    }
    
}

//Suma 
//requiere de más de una entrada
void Combinationals::ADD(int bk)
{
    bool unKnow, unKnowOld;
    int endValue, tmp, pathIn, mask;
    mask=m_cBlocks_p->andMask(m_paths_p[m_blocks_p[bk].pathOut[0]].nBits);
    
    tmp=0;
    for(int pIn=0; pIn<m_blocks_p[bk].nPathIn; pIn++) //XOR sobre todas las entradas
        {
        pathIn=m_blocks_p[bk].pathIn[pIn];
        unKnow=m_paths_p[pathIn].valueEnd.unKnow;
        endValue=m_paths_p[pathIn].valueEnd.value;      //valor de entrada
        if(unKnow==true) break;   //si una entrada es 'x', salida='x'
        tmp+=endValue;                                 //xor
        }                            
    tmp&=mask;
    
    unKnowOld=m_blocks_p[bk].combi.tmpValueUnknow[0];
    m_blocks_p[bk].combi.tmpValueUnknow[0]=unKnow;
//printf("...%d %d %d\n",tmp,  m_blocks_p[bk].combi.tmpValue[0], m_blocks_p[bk].combi.tmpValueUnknow[0]);
    if(unKnowOld!=unKnow || m_blocks_p[bk].combi.tmpValue[0]!=tmp)                  //si el valor anterior del camino difiere del actual
        {
        m_blocks_p[bk].combi.tmpValue[0]=tmp;               //se actualiza el nuevo valor          
        m_blocks_p[bk].currentDelay=m_blocks_p[bk].delay/m_cBlocks_p->m_params_p->deltaT_blocks; //se actualiza el retardo
        if (m_blocks_p[bk].currentDelay == 0) m_blocks_p[bk].currentDelay = 1;
    }
    
}

//Multiplexor
//Si más de una entrada, se hace la selección de una de ellas y la salida es del mismo ancho que las entradas
//Si sólo una entrada,   se hace la selección de un bit en la fila que sale
void Combinationals::MUX(int bk)
{
    Common tools;
    bool unKnow=true, unKnowOld;
    int endValue, tmp, pathIn, mask;
//    mask=m_cBlocks_p->andMask(m_paths_p[m_blocks_p[bk].pathOut[0]].nBits);
    
    //1) se determina el valor de la selección:puede ser un bus o varios caminos de un bit
    char nPathSelec=m_blocks_p[bk].combi.nPathSelect;
    if(nPathSelec==1)  //un solo camino de entrada (un bus de nBits)
        {    
        pathIn=m_blocks_p[bk].combi.pathSelect[0];
        mask=m_cBlocks_p->andMask(m_paths_p[pathIn].nBits);
        unKnow=m_paths_p[pathIn].valueEnd.unKnow;
        tmp=m_paths_p[pathIn].valueEnd.value;      //entrada seleccionada
        tmp&=mask;   
        }
    else    //varios caminos: se esperan de 1 bit
        {
        int mask1=1;
        tmp=0;
        for(int p=0; p<nPathSelec; p++) //se compone la palabra de selección
            {
            pathIn=m_blocks_p[bk].combi.pathSelect[p];
            unKnow=m_paths_p[pathIn].valueEnd.unKnow;
            if(unKnow==true) break;
            endValue=m_paths_p[pathIn].valueEnd.value & mask1;      //valor de entrada
            if(endValue==1)     tmp|=mask1; //entrada seleccionada
            else tmp&=~mask1;
            mask1<<=1;                 //se adecuan las máscaras
            }
        }
                            
    if(unKnow==false) //si el valor de selección es conocido
        {
        int nPathIn=m_blocks_p[bk].nPathIn; 
        pathIn = m_blocks_p[bk].pathIn[0];
        if (nPathIn == 1 && m_paths_p[pathIn].nBits > 1)  //un solo camino de entrada y es un bus
            {
            pathIn = m_blocks_p[bk].pathIn[0];
            unKnow = m_paths_p[pathIn].valueEnd.unKnow;
            if (!unKnow)
                {
                mask = 1;
                mask <<= tmp; //la máscara indica el bit a seleccionar 
                endValue = m_paths_p[pathIn].valueEnd.value;  //valor de entrada
                endValue &= mask;
                endValue >>= tmp;
                tmp = endValue & 0x01;
                }
            }
        else  //varios caminos de entrada           
            {
            pathIn = m_blocks_p[bk].pathIn[(int)tmp];
            //        if(!tools.exist(pathIn, m_blocks_p[bk].pathIn, m_blocks_p[bk].nPathIn)) {unKnow=true; tmp=0;}
            if (!m_cBlocks_p->existPath(pathIn) || (int)tmp >= m_blocks_p[bk].nPathIn) { unKnow = true; tmp = 0; } //atiende a entradas no usadas
            else
                {
                mask = m_cBlocks_p->andMask(m_paths_p[pathIn].nBits);
                tmp = m_paths_p[pathIn].valueEnd.value;      //2) valor de entrada seleccionado
                unKnow = m_paths_p[pathIn].valueEnd.unKnow;
                tmp &= mask;
                }
            }
        }
    
    unKnowOld=m_blocks_p[bk].combi.tmpValueUnknow[0];
    m_blocks_p[bk].combi.tmpValueUnknow[0]=unKnow;
    if(unKnowOld!=unKnow || m_blocks_p[bk].combi.tmpValue[0]!=tmp)                  //si el valor anterior del camino difiere del actual
        {
        m_blocks_p[bk].combi.tmpValue[0]=tmp;               //se actualiza el nuevo valor          
        m_blocks_p[bk].currentDelay=m_blocks_p[bk].delay/m_cBlocks_p->m_params_p->deltaT_blocks; //se actualiza el retardo
        if (m_blocks_p[bk].currentDelay == 0) m_blocks_p[bk].currentDelay = 1;
    }
    
}

//deMultiplexor/decodificador
//Si más de una salida, se hace la selección de una de ellas y la salida es del mismo ancho que la entrada
//Si sólo una salida,   se hace la selección de un bit en la fila que sale
//defaultValue: '1' decodificador; '0' decodificador inversor; otros-> demultiplexor 
void Combinationals::DMUX(int bk, int subtype)
{
   // return;
    Common tools;
    bool unKnow = true, unKnowOld;
    int endValue, tmp, pathIn, pathOut, pathSelect, mask;
    //    mask=m_cBlocks_p->andMask(m_paths_p[m_blocks_p[bk].pathOut[0]].nBits);

    //1) se determina el valor de la selección:puede ser un bus o varios caminos de un bit (queda en tmp)
    char nPathSelec = m_blocks_p[bk].combi.nPathSelect;
    if (nPathSelec == 1)  //un solo camino de entrada (un bus de nBits)
    {
        pathSelect = m_blocks_p[bk].combi.pathSelect[0];
        mask = m_cBlocks_p->andMask(m_paths_p[pathSelect].nBits);
        unKnow = m_paths_p[pathSelect].valueEnd.unKnow;
        tmp = m_paths_p[pathSelect].valueEnd.value;      //salida seleccionada
        tmp &= mask;
    }
    else    //varios caminos: se esperan de 1 bit
    {
        int mask1 = 1;
        tmp = 0;
        for (int p = 0; p < nPathSelec; p++) //se compone la palabra de selección
        {
            pathSelect = m_blocks_p[bk].combi.pathSelect[p];
            unKnow = m_paths_p[pathSelect].valueEnd.unKnow;
            if (unKnow == true) break;
            endValue = m_paths_p[pathSelect].valueEnd.value & mask1;      //valor de entrada
            if (endValue == 1)     tmp |= mask1; //entrada seleccionada
            else tmp &= ~mask1;
            mask1 <<= 1;                 //se adecuan las máscaras
        }
    }

    //en este punto, tmp mantiene el valor de selección
    //Se determina el valor a la salida
    m_blocks_p[bk].combi.pathSelectOut = m_blocks_p[bk].pathOut[tmp]; //info para evaluateBlocks()
    int tmpSelect = tmp;
    if (unKnow == false) //si el valor de selección es conocido
    {
        int nPathOut = m_blocks_p[bk].nPathOut;
        pathIn = m_blocks_p[bk].pathIn[0];
        if (nPathOut == 1 && m_paths_p[pathIn].nBits > 1)  //un solo camino de salida y es un bus
        {
            //pathIn = m_blocks_p[bk].pathIn[0];
            unKnow = m_paths_p[pathIn].valueEnd.unKnow;
            if (!unKnow)
            {
                mask = 1;
                mask <<= tmp; //la máscara indica el bit a seleccionar 
                if (subtype > 1)
                    endValue = m_paths_p[pathIn].valueEnd.value;  //valor de entrada
                else  endValue = subtype; 
                endValue &= mask;
                endValue >>= tmp;
                tmp = endValue & 0x01; //valor de salida
            }
        }
        else  //varios caminos de salida           
        {
            pathOut = m_blocks_p[bk].pathOut[(int)tmp];
            //        if(!tools.exist(pathIn, m_blocks_p[bk].pathIn, m_blocks_p[bk].nPathIn)) {unKnow=true; tmp=0;}
            //atiende a salidas no usadas (inexistentes)
            if ((!m_cBlocks_p->existPath(pathIn) && subtype==2) //si no existe pathIn y se trata de DMUX o no existe pathOut
                || (int)tmp >= m_blocks_p[bk].nPathOut) { unKnow = true; tmp =0; } 
            else
            {
                mask = m_cBlocks_p->andMask(m_paths_p[pathOut].nBits);
                if (subtype > 1) //DMUX
                {
                    tmp = m_paths_p[pathIn].valueEnd.value;      //2) valor de entrada
                    unKnow = false;
                    //unKnow = m_paths_p[pathIn].valueEnd.unKnow;
                }
                else //DECOD & DECODN
                {
                    tmp = subtype;
                    unKnow = false;
                }
                tmp &= mask;
            }
        }
    }
    //ahora, tmp mantiene el valor de la salida
    //Se establecen los valores de las salidas
    //unKnow hace referencia a la info de selección
    pathOut = m_blocks_p[bk].combi.pathSelectOut;
    for (int i = 0; i < m_blocks_p[bk].nPathOut; i++)
    {
        unKnowOld = m_blocks_p[bk].combi.tmpValueUnknow[i];
        m_blocks_p[bk].combi.tmpValueUnknow[i] = unKnow;
        if (i == tmpSelect) //salida seleccionada
        {
            if (unKnowOld != unKnow || m_paths_p[pathOut].valueIni.value != tmp) //si el valor anterior del camino difiere del actual
            {
                m_blocks_p[bk].combi.tmpValue[i] = tmp;               //se actualiza el nuevo valor          
                m_blocks_p[bk].currentDelay = m_blocks_p[bk].delay / m_cBlocks_p->m_params_p->deltaT_blocks; //se actualiza el retardo
                if (m_blocks_p[bk].currentDelay == 0) m_blocks_p[bk].currentDelay = 1;
            }
        }
        else //salidas no seleccionadas
        {
            if(subtype == 0) //decodificador inversor
                m_blocks_p[bk].combi.tmpValue[i] = 1;                         

            else if(subtype == 1) //decodificador no inversor
                m_blocks_p[bk].combi.tmpValue[i] = 0;                         

            else //demultiplexor 
                m_blocks_p[bk].combi.tmpValue[i] = 0;
            //m_blocks_p[bk].combi.tmpValueUnknow[i] = true; (desconocidas)
        }
    }

}


//Comparador
//requiere de dos entradas del mismo ancho
//Según .logicCmp= '0' -> menor que; '1' -> igual a; '2' -> mayor que
void Combinationals::CMP(int bk)
{
    bool unKnow, unKnowOld;
    int tmp, pathIn, mask;
    mask=m_cBlocks_p->andMask(m_paths_p[m_blocks_p[bk].pathOut[0]].nBits);
    
    pathIn=m_blocks_p[bk].pathIn[0];   
    bool unKnow1=m_paths_p[pathIn].valueEnd.unKnow;
    int endValue1=m_paths_p[pathIn].valueEnd.value;
    pathIn=m_blocks_p[bk].pathIn[1];   
    bool unKnow2=m_paths_p[pathIn].valueEnd.unKnow;
    unKnow=m_paths_p[pathIn].valueEnd.unKnow;
    int endValue2=m_paths_p[pathIn].valueEnd.value;
    if(unKnow1==true || unKnow2==true) unKnow=true;
    else
        {
        if     (m_blocks_p[bk].combi.logicCmp==0 && endValue1< endValue2) tmp=1; //comparador menor que (<)
        else if(m_blocks_p[bk].combi.logicCmp==1 && endValue1==endValue2) tmp=1; //comparador igual que (=)
        else if(m_blocks_p[bk].combi.logicCmp==2 && endValue1> endValue2) tmp=1; //comparador mayor que (>)        
        else tmp=0;
        }
    tmp&=mask;   
    
    unKnowOld=m_blocks_p[bk].combi.tmpValueUnknow[0];
    m_blocks_p[bk].combi.tmpValueUnknow[0]=unKnow;
    if(unKnowOld!=unKnow || m_blocks_p[bk].combi.tmpValue[0]!=tmp)                  //si el valor anterior del camino difiere del actual
        {
        m_blocks_p[bk].combi.tmpValue[0]=tmp;               //se actualiza el nuevo valor          
        m_blocks_p[bk].currentDelay=m_blocks_p[bk].delay/m_cBlocks_p->m_params_p->deltaT_blocks; //se actualiza el retardo
        if (m_blocks_p[bk].currentDelay == 0) m_blocks_p[bk].currentDelay = 1;
    }
}

//pestaña que separa un camino de otro principal. Pueden ser de ancho distinto
void Combinationals::TAB(int bk)
{
    bool hit;
    int mask, bitIni, bitEnd, valueIn, valueOut, pathIn, pathOut, tmp, mask2;
    hit=false;
    if(m_blocks_p[bk].nPathIn==1)
//    for(int pIn=0; pIn<m_blocks_p[bk].nPathIn; pIn++)       //para todos los caminos de salida
        {
        pathIn=m_blocks_p[bk].nodePathIn[0].path;                    //camino de entrada
        for(int i=0; i<m_paths_p[pathIn].nSegments; i++)
            {
            for(int j=0; j<m_paths_p[pathIn].segment_p[i].nForks; j++)
                if(m_paths_p[pathIn].segment_p[i].forks[j].tab==bk && m_paths_p[pathIn].segment_p[i].forks[j].pass==true) 
                    {hit=true; m_paths_p[pathIn].segment_p[i].forks[j].pass=false; break;}
            if(hit) break;
            }
        }
    if(hit)
    {
            {
            pathIn =m_blocks_p[bk].nodePathIn[0].path;               //camino de salida
            for(int pOut=0; pOut<m_blocks_p[bk].nPathOut; pOut++)       //para todos los caminos de salida
                {
                pathOut=m_blocks_p[bk].nodePathOut[pOut].path;               //camino de salida
                //if(m_paths_p[pathOut].nBits!= m_paths_p[pathIn].nBits)
                    {
                    //bitIni es el bit alto. formato: path[bitIni:bitEnd]
                    //bitEnd es el bit bajo
                    bitIni=m_blocks_p[bk].nodePathOut[pOut].bitIni;
                    bitEnd=m_blocks_p[bk].nodePathOut[pOut].bitEnd;
                    valueIn=m_paths_p[pathIn].valueIni.value;
                    tmp=valueIn>>bitEnd; //
                    mask2=0; mask=1;
                    for(int i=0; i<=bitIni-bitEnd; i++) {mask2|=mask; mask<<=1;}
                    tmp&=mask2;
                    m_cBlocks_p->startPath(pathOut, tmp, m_paths_p[pathIn].valueIni.unKnow);   //se activan los caminos de salida
                    }
                }
            }
    }
}

//separador/concentrador de caminos
//Un camino de entrada y varios de salida de ancho variable
//Varios caminos de entrada de ancho variable y uno de salida
void Combinationals::NODE(int bk)
{
    int mask, bitIni, bitEnd, valueIn, valueOut, pathIn=0, pathOut, tmp, mask2;
    
        //se copia el valor de entrada sobre cada camino de salida y se inician, sin retardos
        if(m_blocks_p[bk].nPathIn==1 && m_blocks_p[bk].nPathOut==1)
            {
            pathIn =m_blocks_p[bk].nodePathOut[0].path;               //camino de salida
            pathOut=m_blocks_p[bk].nodePathOut[0].path;               //camino de salida
            valueIn=m_paths_p[pathIn].valueEnd.value;
            m_cBlocks_p->startPath(pathOut, valueIn, m_paths_p[pathIn].valueEnd.unKnow);   //se activan los caminos de salida
            }
        else if(m_blocks_p[bk].nPathIn==1) //entra un camino salen varios
            {
            pathIn =m_blocks_p[bk].nodePathIn[0].path;               //camino de salida
            for(int pOut=0; pOut<m_blocks_p[bk].nPathOut; pOut++)       //para todos los caminos de salida
                {
                pathOut=m_blocks_p[bk].nodePathOut[pOut].path;               //camino de salida
                //bitIni es el bit alto. formato: path[bitIni:bitEnd]
                //bitEnd es el bit bajo.
                bitIni=m_blocks_p[bk].nodePathOut[pOut].bitIni;
                bitEnd=m_blocks_p[bk].nodePathOut[pOut].bitEnd;
                valueIn=m_paths_p[pathIn].valueEnd.value;
                tmp=valueIn>>bitEnd;
                mask2=0; mask=1;
                for(int i=0; i<=bitIni-bitEnd; i++) {mask2|=mask; mask<<=1;}
                tmp&=mask2;
                m_cBlocks_p->startPath(pathOut, tmp, m_paths_p[pathIn].valueEnd.unKnow);   //se activan los caminos de salida
                }
            }
        else if(m_blocks_p[bk].nPathOut==1) //varios caminos que entrar y uno que sale
            {
            pathOut=m_blocks_p[bk].nodePathOut[0].path;               //camino de salida
            valueOut=0;
            for(int pIn=0; pIn<m_blocks_p[bk].nPathIn; pIn++)       //para todos los caminos de salida
                {
                pathIn=m_blocks_p[bk].nodePathIn[pIn].path;               //camino de salida
                if(m_paths_p[pathIn].valueEnd.unKnow==true) break;
                if(m_paths_p[pathOut].nBits!= m_paths_p[pathIn].nBits)
                    {
                    bitIni=m_blocks_p[bk].nodePathIn[pIn].bitIni;
                    bitEnd=m_blocks_p[bk].nodePathIn[pIn].bitEnd;
                    valueIn=m_paths_p[pathIn].valueEnd.value;
                    tmp=valueIn<<bitEnd;
                    valueOut|=tmp;
                    }
                }
            m_cBlocks_p->startPath(pathOut, valueOut, m_paths_p[pathIn].valueEnd.unKnow);   //se activan los caminos de salida
            }
        else //varios caminos de entrada y salida
        {
            bool inValueUnknow = false;
            valueOut = 0;
            for (int pIn = 0; pIn < m_blocks_p[bk].nPathIn; pIn++)       //para todos los caminos de salida
            {
                //Se unifican los caminos de entrada
                pathIn = m_blocks_p[bk].nodePathIn[pIn].path;               //camino de salida
                if (m_paths_p[pathIn].valueEnd.unKnow == true) {inValueUnknow = true;  break;}
                bitIni = m_blocks_p[bk].nodePathIn[pIn].bitIni;
                bitEnd = m_blocks_p[bk].nodePathIn[pIn].bitEnd;
                valueIn = m_paths_p[pathIn].valueEnd.value;
                tmp = valueIn << bitEnd;
                valueOut |= tmp;
            }
            //Se segregan la info en los caminos de salida
            //en caso de que un camino de entrada tenga valor desconocido, todas las salidas se hacen desconocidas
            valueIn = valueOut;
            for (int pOut = 0; pOut < m_blocks_p[bk].nPathOut; pOut++)       //para todos los caminos de salida
            {
                pathOut = m_blocks_p[bk].nodePathOut[pOut].path;               //camino de salida
                if (inValueUnknow)
                    m_cBlocks_p->startPath(pathOut, 0, true);  //caminos de salida con valor desconocido
                else
                {
                    //bitIni es el bit alto. formato: path[bitIni:bitEnd]
                    //bitEnd es el bit bajo.
                    bitIni = m_blocks_p[bk].nodePathOut[pOut].bitIni;
                    bitEnd = m_blocks_p[bk].nodePathOut[pOut].bitEnd;
                    tmp = valueIn >> bitEnd;
                    mask2 = 0; mask = 1;
                    for (int i = 0; i <= bitIni - bitEnd; i++) { mask2 |= mask; mask <<= 1; }
                    tmp &= mask2;
                    m_cBlocks_p->startPath(pathOut, tmp, m_paths_p[pathIn].valueEnd.unKnow);   //se activan los caminos de salida
                }
            }
        }
}

//Gestión de la memoria
//se trata de un bloque sin clock pero surge un problema con la escritura si no se toman medidas.
//para que se permita la escritura debe de haber novedad en la linea de control a '1' (sincronismo con control)
//si la novedad se produce sólo en la linea de datos de entrada, no se hace nada.
void Combinationals::MEMORY(int bk, bool clock)
{
    bool unKnow, unKnowOld, hit=false, enableWrite;
    int tmp, maskIn, maskOut, pathAddr, addr, pathDataIn, dataIn, pathCtr, dataCtr;
    bool unKnowAddr, unKnowDataIn, unKnowCtr;
    
    maskIn =m_cBlocks_p->andMask(m_paths_p[m_blocks_p[bk].pathIn [0]].nBits);
    maskOut=m_cBlocks_p->andMask(m_paths_p[m_blocks_p[bk].pathOut[0]].nBits);
    
    pathAddr=m_blocks_p[bk].pathIn[0];   
    unKnowAddr=m_paths_p[pathAddr].valueEnd.unKnow;
    addr  =m_paths_p[pathAddr].valueEnd.value & maskIn;
    
    pathDataIn=m_blocks_p[bk].pathIn[1];   
    unKnowDataIn=m_paths_p[pathDataIn].valueEnd.unKnow;
    dataIn=m_paths_p[pathDataIn].valueEnd.value & maskOut;
    
    pathCtr=m_blocks_p[bk].pathIn[2];   
    unKnowCtr=m_paths_p[pathCtr].valueEnd.unKnow;
    dataCtr=m_paths_p[pathCtr].valueEnd.value & 0x01;  //R/W
    
    if(!clock && !m_cBlocks_p->m_pathsNews[pathAddr].news && //si no hay novedades para el bloque
        !m_cBlocks_p->m_pathsNews[pathCtr].news) return; 
        
    if(clock && dataCtr==1) enableWrite=true; //permite la escritura
    else enableWrite=false;
      
    unKnow=false;
    if(unKnowAddr) {unKnow=true; tmp=0;}
    else 
        {
        for(int i=0; i<m_blocks_p[bk].TT.nRows; i++)
            {
            if(m_blocks_p[bk].TT.table_p[i][0]==addr) 
                {
                hit=true;
                if(dataCtr==0 || unKnowCtr==true) //read o control desconocido
                    {tmp=m_blocks_p[bk].TT.table_p[i][1]; break;}
                else if(enableWrite)//escribir
                    {
                    if(unKnowDataIn==true) hit=false; //si dataIn desconocido, no se altera el contenido y salida desconocida
                    else {m_blocks_p[bk].TT.table_p[i][1]=dataIn; tmp=dataIn;}
                    break;
                    }
                else{ //sin cambios
                    unKnow=m_blocks_p[bk].combi.tmpValueUnknow[0];
                    tmp=m_blocks_p[bk].combi.tmpValue[0];
                    hit=true;
                    }
                }
            }        
        if(!hit) unKnow=true;
        }
    tmp&=maskOut;   
    unKnowOld=m_blocks_p[bk].combi.tmpValueUnknow[0];
    m_blocks_p[bk].combi.tmpValueUnknow[0]=unKnow;
//printf("%d %d %X %X\n",unKnowOld, unKnow, m_blocks_p[bk].combi.tmpValue[0], tmp);
    if(unKnowOld!=unKnow || m_blocks_p[bk].combi.tmpValue[0]!=tmp)  //si el valor anterior del camino difiere del actual
        {
        m_blocks_p[bk].combi.tmpValue[0]=tmp;               //se actualiza el nuevo valor          
        m_blocks_p[bk].currentDelay=m_blocks_p[bk].delay/m_cBlocks_p->m_params_p->deltaT_blocks; //se actualiza el retardo
        if (m_blocks_p[bk].currentDelay == 0) m_blocks_p[bk].currentDelay = 1;
    }

}

//Tabla de la verdad con dos columnas (entradas y salidas)
void Combinationals::OTHERS(int bk)
{
    bool unKnow, unKnowOld, hit=false;
    int tmp, pathIn, mask,inValue;
    
    mask=m_cBlocks_p->andMask(m_paths_p[m_blocks_p[bk].pathOut[0]].nBits);
    pathIn=m_blocks_p[bk].pathIn[0];   
    unKnow=m_paths_p[pathIn].valueEnd.unKnow;
    inValue=m_paths_p[pathIn].valueEnd.value;
    if(unKnow==true) tmp=0;
    else 
        {
        for(int i=0; i<m_blocks_p[bk].TT.nRows; i++)
            {
            int mask=m_blocks_p[bk].TT.table_p[i][1];
            if(m_blocks_p[bk].TT.table_p[i][0]==(inValue&mask)) {hit=true; tmp=m_blocks_p[bk].TT.table_p[i][2]; break;}
            }
        if(!hit) unKnow=true;
        }        
    tmp&=mask;   
    unKnowOld=m_blocks_p[bk].combi.tmpValueUnknow[0];
    m_blocks_p[bk].combi.tmpValueUnknow[0]=unKnow;
    if(unKnowOld!=unKnow || m_blocks_p[bk].combi.tmpValue[0]!=tmp) //si el valor anterior del camino difiere del actual
        {
        m_blocks_p[bk].combi.tmpValue[0]=tmp;               //se actualiza el nuevo valor          
        m_blocks_p[bk].currentDelay=m_blocks_p[bk].delay/m_cBlocks_p->m_params_p->deltaT_blocks; //se actualiza el retardo
        if (m_blocks_p[bk].currentDelay == 0) m_blocks_p[bk].currentDelay = 1;
    }
}
