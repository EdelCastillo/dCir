/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 *
 *    Manejo de bloques secuenciales
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

#include "sequentials.h"

//Constructor
Sequentials::Sequentials(class Blocks *cBlocks_p)
{
    m_blocks_p=cBlocks_p->m_blocks_p;
    m_paths_p=cBlocks_p->m_paths_p;
    m_cBlocks_p=cBlocks_p;
}

//Gestion de flip-flops tipo D
void Sequentials::FFD(int bk)
{
    bool unKnow, unKnowOld;
    int tmp, input, output, pathOut, mask;
    mask=m_cBlocks_p->andMask(m_paths_p[m_blocks_p[bk].pathOut[0]].nBits);
    
    input=m_blocks_p[bk].FF.valueA;        //valor de entrada D
    output=m_blocks_p[bk].FF.valueOut; //valor de salida actual                     
    unKnow=m_blocks_p[bk].FF.valueAUnknow;
    //se copia la entrada a la salida
    tmp=input; tmp&=mask;                   //salida = entrada y máscara 1 bit
        
    unKnowOld=m_blocks_p[bk].FF.valueOutUnknow;
    m_blocks_p[bk].FF.valueOutUnknow=unKnow;    //salida ='x' si entrada = 'x'
    if(unKnowOld!=unKnow || tmp!=output)                             //si hay cambios en la salida
        {
        m_blocks_p[bk].FF.valueOut=tmp;         //actualización de salida  
        pathOut=m_blocks_p[bk].pathOut[0];
        m_cBlocks_p->startPath(pathOut, tmp, unKnow);                //copia a la entrada del camino de salida y activación
        }   
    else
        m_blocks_p[bk].FF.start=false;          //sin cambios
}


//Gestion de flip-flops tipo T
void Sequentials::FFT(int bk)
{
    bool unKnow, unKnowOld;
    int tmp, input, output, pathOut, mask;
    mask=m_cBlocks_p->andMask(m_paths_p[m_blocks_p[bk].pathOut[0]].nBits);
    
    input=m_blocks_p[bk].FF.valueA;        //valor de entrada T
    output=m_blocks_p[bk].FF.valueOut; //valor de salida actual      
    unKnow=m_blocks_p[bk].FF.valueAUnknow;
    //se invierte la salida si ésta no es 'x' y la entrada es '1'
    tmp=output ^ input;     //inversión si '1'
        
    unKnowOld=m_blocks_p[bk].FF.valueOutUnknow;
    m_blocks_p[bk].FF.valueOutUnknow=unKnow;    //salida ='x' si entrada = 'x'
    if(unKnowOld!=unKnow || tmp!=output)                             //si hay cambios en la salida
        {
        m_blocks_p[bk].FF.valueOut=tmp;         //actualización de salida  
        pathOut=m_blocks_p[bk].pathOut[0];
        m_cBlocks_p->startPath(pathOut, tmp, unKnow);                //copia a la entrada del camino de salida y activación
        }   
    else
        m_blocks_p[bk].FF.start=false;          //sin cambios
}

//Gestion de flip-flops tipo JK
void Sequentials::FFJK(int bk)
{
    bool unKnow, unKnowOld;
    int tmp, input, output, pathOut, mask;
    mask=m_cBlocks_p->andMask(m_paths_p[m_blocks_p[bk].pathOut[0]].nBits);
    
    mask=1;
    int mask2=1;
    char bit;
    int nBits=m_paths_p[m_blocks_p[bk].pathOut[0]].nBits;
    int inputJ=m_blocks_p[bk].FF.valueA,       //valor de entrada J
    inputK=m_blocks_p[bk].FF.valueB;       //valor de entrada K
    bool unKnow1=m_blocks_p[bk].FF.valueAUnknow;
    bool unKnow2=m_blocks_p[bk].FF.valueBUnknow;
    output=m_blocks_p[bk].FF.valueOut; //valor de salida actual    
        tmp=0;    
    //idea: 
    //si el bit de salida vale 1,(estado 1), el nuevo estado es la xor con K
    //si el bit de salida vale 0,(estado 0), el nuevo estado es la xor con J
    for(int b=0; b<nBits; b++)                  //XOR sobre todas las entradas bit a bit
        {
        if((output&mask) != 0)                  //si el estado es 1 en este peso
            {bit= output^inputK; bit&=mask;}    //nuevo estado= estado anterior xor K
            else {bit= output^inputJ; bit&=mask;}   //sino, nuevo estado= estado anterior xor J
            if(bit!=0) tmp|=mask;                   //si '1', se fuerza '1' en el peso correspondiente
            else tmp&=mask2;                        //si '0', se fuerza '0' en el peso correspondiente
            mask<<=1;  mask2|=mask;                 //se adecuan las máscaras
            }
        if(unKnow1==true || unKnow2==true) unKnow=true;         //J o K valen 'x', salida 'x'
        else unKnow=false;
        
    unKnowOld=m_blocks_p[bk].FF.valueOutUnknow;
    m_blocks_p[bk].FF.valueOutUnknow=unKnow;    //salida ='x' si entrada = 'x'
    if(unKnowOld!=unKnow || tmp!=output)                             //si hay cambios en la salida
        {
        m_blocks_p[bk].FF.valueOut=tmp;         //actualización de salida  
        pathOut=m_blocks_p[bk].pathOut[0];
        m_cBlocks_p->startPath(pathOut, tmp, unKnow);                //copia a la entrada del camino de salida y activación
        }   
    else
        m_blocks_p[bk].FF.start=false;          //sin cambios
}

//Gestion de latches
void Sequentials::LATCH(int bk)
{
    bool unKnow, unKnowOld;
    int pathIn, pathEnable, tmp=0, mask;
    mask=m_cBlocks_p->andMask(m_paths_p[m_blocks_p[bk].pathOut[0]].nBits);
    
    pathIn=m_blocks_p[bk].pathIn[0];   
    m_paths_p[pathIn].endPath=false; 
    pathEnable=m_blocks_p[bk].pathEnable;   
    m_paths_p[pathEnable].endPath=false; 
    unKnow=m_paths_p[pathEnable].valueEnd.unKnow;
    if(unKnow==false) //valor conocido en enable
        {
        if(m_paths_p[pathEnable].valueEnd.value==1) //si enable=1
            {
            unKnow=m_paths_p[pathIn].valueEnd.unKnow;
                tmp=m_paths_p[pathIn].valueEnd.value;
            }
        else  //si enable=0
            {
            int pathOut=m_blocks_p[bk].pathOut[0];
            tmp=m_paths_p[pathOut].valueIni.value;
            unKnow=m_paths_p[pathOut].valueIni.unKnow;
            }
        }
    tmp&=mask;                   
    unKnowOld=m_blocks_p[bk].combi.tmpValueUnknow[0];
    m_blocks_p[bk].combi.tmpValueUnknow[0] = unKnow;
    if(unKnowOld!=unKnow || m_blocks_p[bk].combi.tmpValue[0]!=tmp)//si el valor anterior del camino difiere del actual
        {
        m_blocks_p[bk].combi.tmpValue[0] = tmp;               //se actualiza el nuevo valor          
        m_blocks_p[bk].currentDelay=m_blocks_p[bk].delay; //se actualiza el retardo
    }                                            
            
}

//Gestion de tabla de la verdad para FSM
void Sequentials::FSM(int bk)
{    
    bool hit=false;
    bool unKnow, unKnowOld;
    int tmp, input, output, pathOut, maskOut, maskIn;
    maskIn=m_cBlocks_p->andMask(m_paths_p[m_blocks_p[bk].pathIn[0]].nBits);
    maskOut=m_cBlocks_p->andMask(m_paths_p[m_blocks_p[bk].pathOut[0]].nBits);
    
    input=m_blocks_p[bk].FF.valueA;        //valor de entrada T
    input&=maskIn;
    output=m_blocks_p[bk].FF.valueOut; //valor de salida actual      
    unKnow=m_blocks_p[bk].FF.valueAUnknow;
    int statusTT, statusF, inputTT, outputTT, mask;
    if(unKnow==true) tmp=0;
    else 
        {
        for(int i=0; i<m_blocks_p[bk].TT.nRows; i++) //búsqueda de coincidencias
            {
            statusTT=m_blocks_p[bk].TT.table_p[i][0];   //estado en TT
            inputTT =m_blocks_p[bk].TT.table_p[i][1];   //entrada en TT
            mask    =m_blocks_p[bk].TT.table_p[i][2];   //máscara para valor de entrada
            statusF =m_blocks_p[bk].TT.table_p[i][3];   //estado futuro según TT
            if(statusTT==m_blocks_p[bk].FF.status && inputTT==(input&mask)) 
                {
                m_blocks_p[bk].FF.status=statusF; 
                for(int j=0; j<m_blocks_p[bk].TT.nRows; j++) //búsqueda de coincidencias
                    {
                    statusTT=m_blocks_p[bk].TT.table_p[j][0];   //estado en TT
                    outputTT=m_blocks_p[bk].TT.table_p[j][4];   //salida
                    if(statusTT==m_blocks_p[bk].FF.status) 
                        {tmp=outputTT; hit=true; break;}                        
                    }
                if(hit) break;
                }
            }
        if(!hit) unKnow=true;
        }        
    tmp&=maskOut; 
    
    unKnowOld=m_blocks_p[bk].FF.valueOutUnknow;
    m_blocks_p[bk].FF.valueOutUnknow=unKnow;    //salida ='x' si entrada = 'x'
    if(unKnowOld!=unKnow || tmp!=output)                             //si hay cambios en la salida
        {
        m_blocks_p[bk].FF.valueOut=tmp;         //actualización de salida  
        pathOut=m_blocks_p[bk].pathOut[0];
        m_cBlocks_p->startPath(pathOut, tmp, unKnow);                //copia a la entrada del camino de salida y activación
        }   
    else
        m_blocks_p[bk].FF.start=false;          //sin cambios
    
}

