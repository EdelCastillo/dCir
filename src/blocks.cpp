/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 *
 *  Tratamiento de bloques
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
 
#include "blocks.h"
#include "combinationals.h"
#include "sequentials.h"

//constructor
//clase derivada de la clase Paths
//recibe un puntero a la clase Image para la visualización gráfica
Blocks::Blocks(class Image *image_p):Paths(image_p)
{
    m_combi_p=0;
    m_sequ_p=0;
    m_image_p=image_p;
    m_blocks_p=0;       //puntero al array de estructuras BLOCKS
    createAllBlocks(); //crea e inicializa la estructura de bloques como desactivados
    m_combi_p=new Combinationals(this);
    m_sequ_p =new Sequentials(this);
}

//destructor
//elimina todos los items y libera la memoria reservada
Blocks::~Blocks()
{
//printf("init destructor Block class\n");
    if(m_blocks_p)
        {
        for(int bk=0; bk<MAX_BLOCKS; bk++)  //se inicializan los bloques
            {
            if(existBlock(bk)) 
                {
                if(m_blocks_p[bk].recItem)  m_image_p->removeRectItem(m_blocks_p[bk].recItem);
                if(m_blocks_p[bk].nameItem) m_image_p->removeTextItem(m_blocks_p[bk].nameItem);
                if(m_blocks_p[bk].TT.fileName) delete [] m_blocks_p[bk].TT.fileName;
                if(m_blocks_p[bk].TT.table_p)
                    {
                    for(int i=0; i<m_blocks_p[bk].TT.nRows; i++)
                        if(m_blocks_p[bk].TT.table_p[i]) delete []m_blocks_p[bk].TT.table_p[i]; 
                    delete []  m_blocks_p[bk].TT.table_p;  m_blocks_p[bk].TT.table_p=0; 
                    }
                }
            }
        delete []m_blocks_p; m_blocks_p=0;
        }
    if(m_combi_p) delete m_combi_p;
    if(m_sequ_p)  delete m_sequ_p;
          
//printf("finist destructor Block class\n");
}

//crea e inicializa la estructura de bloques como desactivados
void Blocks::createAllBlocks()
{
    if(m_blocks_p==0)
        m_blocks_p=new BLOCK[MAX_BLOCKS];
    for(int i=0; i<MAX_BLOCKS; i++)  //se inicializan los bloques
        {
        m_blocks_p[i].blockName.value=0;    //referencia
        m_blocks_p[i].type=0;               //tipo de bloque
        m_blocks_p[i].nBits=1;              //bits por defecto
        m_blocks_p[i].TT.fileName=0;             //fichero con tabla de la verdad
        m_blocks_p[i].TT.table_p=0;             //tabla de la verdad
        m_blocks_p[i].combi.type=0;         //tipo de bloque combinacional
        m_blocks_p[i].nPathIn=0;            //ningún camino de entrada
        m_blocks_p[i].nPathOut=0;           //ningún camino de salida
        m_blocks_p[i].pathEnable=-1;           //ningún camino de validación
        m_blocks_p[i].combi.tmpValueUnknow[0]=true;    //desconocido 
        m_blocks_p[i].combi.nPathSelect=0;  //sin caminos de selección
        m_blocks_p[i].combi.pathSelectOut=-1;  //>=0 si operativo (bloque DMUX)
        m_blocks_p[i].combi.logicCmp=1;     //comparador de igualdad
        m_blocks_p[i].enable=false;         //bloque desactivado
        m_blocks_p[i].delay=1;              //1 ms
//        m_blocks_p[i].currentDelay=m_blocks_p[i].delay/m_params_p->deltaT_blocks; //retardo
        m_blocks_p[i].currentDelay=1;       //retardo de 1 m_params_p->deltaT_blocks
        m_blocks_p[i].recItem=0;            //item para el rectángulo
        m_blocks_p[i].nameItem=0;           //item para el nombre
        m_blocks_p[i].initPortValue = 0;
        m_blocks_p[i].initPortValueUnknow = 0;
        m_blocks_p[i].FF.type=0;            //tipo de FF
        m_blocks_p[i].FF.enable=true;       //FF siempre activo
        m_blocks_p[i].FF.start=0;           //clock no recibido
        m_blocks_p[i].FF.valueAUnknow=true;         //desconocido 
        m_blocks_p[i].FF.valueBUnknow=true;         //desconocido 
        m_blocks_p[i].FF.valueOutUnknow=true;       //desconocido 
        m_blocks_p[i].FF.resetValueUnknow=true;     //desconocido 
    }
}

//inicializa los bloques activos
//si puerto de entrada o FF, traslada a los caminos de salida el valor de inicialización
void Blocks::initActiveBlocks()
{
    for(int i=0; i<MAX_BLOCKS; i++)     //se inicializan los bloques
        {
        if(!existBlock(i))  continue;   //si no existe el bloque (no activado)
        
        if(m_blocks_p[i].type==(char)Type::IN_PORT) //bloque es port de entrada
            {
            m_blocks_p[i].enable=true;  //activado
            for(int j=0; j<m_blocks_p[i].nPathOut; j++) //para todos los caminos de salida
                {
                int path=m_blocks_p[i].pathOut[j];  //camino de salida
                m_paths_p[path].valueIni.value=m_blocks_p[i].initPortValue;  //copia el valor dado sobre el camino
                m_paths_p[path].valueIni.unKnow=m_blocks_p[i].initPortValueUnknow;  //copia el valor dado sobre el camino
                m_paths_p[path].start=true;         //se inicia cada camino conectado al bloque
                }
            }
            
        else if(m_blocks_p[i].type== (char)Type::SEQ) //si el bloque es secuencial
            {
            m_blocks_p[i].FF.start=0;               //clock no recibido
            m_blocks_p[i].FF.valueAUnknow=true;             //valores de entrada desconocidos
            m_blocks_p[i].FF.valueBUnknow=true;
            int pathOut=m_blocks_p[i].pathOut[0];   //el valor de ini se traslada atodos los caminos de salida
            if(m_blocks_p[i].FF.type != (char)FF::FSM)
                {
                m_blocks_p[i].FF.valueOut=m_blocks_p[i].FF.resetValue; //valor de salida a valor de inicialización
                m_blocks_p[i].FF.valueOutUnknow=m_blocks_p[i].FF.resetValueUnknow; //valor de salida a valor de inicialización
                m_paths_p[pathOut].valueIni.value =m_blocks_p[i].FF.valueOut;
                m_paths_p[pathOut].valueIni.unKnow=m_blocks_p[i].FF.valueOutUnknow;
                }
            else  //FSM
                {
                bool hit=false, unKnow=false;
                int tmp=0;
                m_blocks_p[i].FF.status=m_blocks_p[i].FF.resetValue;
                for(int j=0; j<m_blocks_p[i].TT.nRows; j++)
                    {
                     if(m_blocks_p[i].TT.table_p[j][0]==m_blocks_p[i].FF.status) 
                        {
                        hit=true; tmp=m_blocks_p[i].TT.table_p[j][4];
                        m_blocks_p[i].FF.valueOut=tmp; //valor de salida a valor de estado inicial
                        m_blocks_p[i].FF.valueOutUnknow=false;                          
                        }
                    }
                if(!hit) unKnow=true;
                m_paths_p[pathOut].valueIni.value =tmp;
                m_paths_p[pathOut].valueIni.unKnow=unKnow;
                }
            m_paths_p[pathOut].start=true;          //se inicia cada camino conectado al bloque
            if(m_blocks_p[i].FF.type== (char)FF::LATCH) m_blocks_p[i].FF.enable=false;
            }                 
        else if(m_blocks_p[i].type== (char)Type::MEMORY) //MEMORY
            {
            Files file;
            file.fileToMemory(&m_blocks_p[i].TT);        //se recarga la memoria por si se han alterado los datos
            }
        else
            for (int j = 0; j < m_blocks_p[i].nPathOut; j++)
            {

            int path=m_blocks_p[i].pathOut[j];  //camino de salida
            m_paths_p[path].valueIni.unKnow=true;
            m_paths_p[path].valueIni.value=-1;
            m_blocks_p[i].combi.tmpValueUnknow[j] = true;
            m_blocks_p[i].combi.tmpValue[0] = -1;

//                if (m_paths_p[tmpPath].valueIni.unKnow != m_blocks_p[bk].combi.tmpValueUnknow[i] ||
//                    m_paths_p[tmpPath].valueIni.value  != m_blocks_p[bk].combi.tmpValue[i])
            }
        }

    for(int p=0; p<MAX_PATHS; p++)
        {
        if(!existPath(p))  continue;   //si no existe el bloque (no activado)
        m_pathsNews[p].news=true;
        m_pathsNews[p].block=m_paths_p[p].blockEnd;
        if(existBlock(m_paths_p[p].blockEnd))
            m_pathsNews[p].type=m_blocks_p[m_pathsNews[p].block].type;
        }

}

//retorna true si existe el camino
//existe si la estructura está creada y el bloque está habilitado
bool Blocks::existBlock(int block)
{
    if(block<0 || block>MAX_BLOCKS) return false;
    if(m_blocks_p && m_blocks_p[block].enable==true) //existe
        return true;
    return false;
}

//retorna true si existe el nombre del bloque
//existe si la estructura está creada y ya se creó el nombre
bool Blocks::existNameBlock(int block)
{
    if(block<0 || block>MAX_BLOCKS) return false;
    if(m_blocks_p && m_blocks_p[block].nameItem) //existe
        return true;
    return false;
}

//revisa que todos los bloques activos sean correctos, de no serlo lo comunica
//retorna false si algo no está bien
bool Blocks::isAllBlocksOK()
{
    char txt[100];
    for(int bk=0; bk<MAX_BLOCKS; bk++)
        {
        if(!existBlock(bk)) continue;
        if(m_blocks_p[bk].type== (char)Type::IN_PORT && m_blocks_p[bk].nPathOut<=0)
            {
            sprintf(txt, "Warning: the information about the block b(%d) is incomplete. It may not have any output paths.", bk);
            QMessageBox::warning(0, "Paths", txt, QMessageBox::Cancel);
            return false;
            }
        if(m_blocks_p[bk].type== (char)Type::OUT_PORT && m_blocks_p[bk].nPathIn<=0)
            {
            sprintf(txt, "Warning: the information about the block b(%d) is incomplete. It may not have any output paths.", bk);
            QMessageBox::warning(0, "Paths", txt, QMessageBox::Cancel);
            return false;
            }            
        if(m_blocks_p[bk].type!= (char)Type::IN_PORT && m_blocks_p[bk].type!= (char)Type::OUT_PORT &&
            (m_blocks_p[bk].type == (char)Type::COMBI && m_blocks_p[bk].combi.type != (char)Oper::DECOD) &&
            (m_blocks_p[bk].type == (char)Type::COMBI && m_blocks_p[bk].combi.type != (char)Oper::DECODN) &&
            (m_blocks_p[bk].nPathIn<=0 || m_blocks_p[bk].nPathOut<=0))
            {
            sprintf(txt, "Warning: the information about the block b(%d) is incomplete. It may not have any output paths.", bk);
            QMessageBox::warning(0, "Paths", txt, QMessageBox::Cancel);
            return false;
            }
        if(m_blocks_p[bk].type== (char)Type::COMBI && 
            (m_blocks_p[bk].combi.type== (char)Oper::MUX || m_blocks_p[bk].combi.type == (char)Oper::DMUX ||
                m_blocks_p[bk].combi.type == (char)Oper::DECOD || m_blocks_p[bk].combi.type == (char)Oper::DECODN) &&
            m_blocks_p[bk].combi.nPathSelect<=0)
            {
            sprintf(txt, "Warning: the block b(%d) has not selection paths", bk);
            QMessageBox::warning(0, "Paths", txt, QMessageBox::Cancel);
            return false;
            }
         if((m_blocks_p[bk].type== (char)Type::COMBI && m_blocks_p[bk].combi.type== (char)Oper::OTHERS) ||
             (m_blocks_p[bk].type== (char)Type::SEQ && m_blocks_p[bk].FF.type== (char)FF::FSM) ||
             m_blocks_p[bk].type== (char)Type::MEMORY)
                {
                //intenta abrir el fichero de proyecto
                FILE *fp=0;
                if((fp=fopen(m_blocks_p[bk].TT.fileName, "r"))==0) 
                {
                    sprintf(txt, "Warning: file '%s' does not exist, in block b(%d).", m_blocks_p[bk].TT.fileName, bk);
                    QMessageBox::warning(0, "Paths", txt, QMessageBox::Cancel);
                    return false;
                }
                if (fp) fclose(fp);
                if(m_blocks_p[bk].TT.nRows<=0 || m_blocks_p[bk].TT.nCols<=0)
                    {
                    sprintf(txt, "Warning: file '%s' is empty, in block b(%d). The project may need to be reloaded.", m_blocks_p[bk].TT.fileName, bk);
                    QMessageBox::warning(0, "Paths", txt, QMessageBox::Cancel);
                    return false;
                    }
                }
        }
    return true;
}

//retorna true si el bloque no tienen ningún camino conectado
bool Blocks::isIsolatedBlock(int bk)
{
    bool ret=false;
    if(!existBlock(bk)) return true;
    if(m_blocks_p[bk].type== (char)Type::TAB && (m_blocks_p[bk].nPathOut<=0 || m_blocks_p[bk].nPathIn <= 0)) 
        ret=true;
    if(m_blocks_p[bk].type== (char)Type::IN_PORT && m_blocks_p[bk].nPathOut<=0) ret=true;

    else if(m_blocks_p[bk].type== (char)Type::OUT_PORT && m_blocks_p[bk].nPathIn<=0) ret=true;
        
    else if(m_blocks_p[bk].nPathIn<=0 && m_blocks_p[bk].nPathOut<=0)  
        {
        if(m_blocks_p[bk].type== (char)Type::SEQ)
            {
            if     (m_blocks_p[bk].FF.enable==true ) ret=true;  
            else if(m_blocks_p[bk].pathEnable<=0)    ret=true;  
            }
        if(m_blocks_p[bk].type== (char)Type::COMBI && m_blocks_p[bk].combi.type== (char)Oper::MUX && m_blocks_p[bk].combi.nPathSelect<=0)
            ret=true;
        }
    return ret;
}

//evalua la lista de bloques a la búsqueda del primero libre
//Retorna el índice a la primera entrada libre o -1 si no existe o no está creada la entrada en m_blocks_p
int Blocks::getFreeBlock()
{
    if(m_blocks_p==0) return -1;
    int bk;
    for(bk=0; bk<MAX_BLOCKS; bk++)
        if(m_blocks_p[bk].enable==0) return bk;
    if(bk==MAX_PATHS) return -1;
    return -1;
}

//elimina todos los rectángulos alrededor de los bloques a menos que sean puertos de entrada o salida
void Blocks::removeAllBlocksRecItems()
{
        for(int bk=0; bk<MAX_BLOCKS; bk++)
            {
            if(!existBlock(bk)) continue; //si no existe el bloque
            if(m_blocks_p[bk].type== (char)Type::IN_PORT || m_blocks_p[bk].type== (char)Type::OUT_PORT ) continue;
            if(m_blocks_p[bk].recItem)  {m_image_p->removeRectItem(m_blocks_p[bk].recItem);  m_blocks_p[bk].recItem=0;}
            }        
} 

//elimina un bloque
//elimina los items y desactiva el bloque
void Blocks::removeBlock(int bk)
{
    if(existBlock(bk))
        {
        int path=0;
        if(m_blocks_p[bk].recItem)  {m_image_p->removeRectItem(m_blocks_p[bk].recItem);  m_blocks_p[bk].recItem=0; }
        if(m_blocks_p[bk].nameItem) {m_image_p->removeTextItem(m_blocks_p[bk].nameItem); m_blocks_p[bk].nameItem=0;}
        if(m_blocks_p[bk].type != (char)Type::TAB)
            for(int i=0; i<m_blocks_p[bk].nPathIn; i++) 
                {
                if (m_blocks_p[bk].type == (char)Type::NODE)// || m_blocks_p[bk].type == (char)Type::TAB)
                     path = m_blocks_p[bk].nodePathIn[i].path;
                else path = m_blocks_p[bk].pathIn[i];
                m_paths_p[path].blockEnd=-1;
                }
        m_blocks_p[bk].nPathIn=0;    
        for(int i=0; i<m_blocks_p[bk].nPathOut; i++) 
            {
            if (m_blocks_p[bk].type == (char)Type::NODE || m_blocks_p[bk].type == (char)Type::TAB)
                path = m_blocks_p[bk].nodePathOut[i].path;
            else path = m_blocks_p[bk].pathOut[i];
            m_paths_p[path].blockIni=-1;
            }
        m_blocks_p[bk].nPathOut=0; 
        if(m_blocks_p[bk].type== (char)Type::COMBI)
            {
            for(int i=0; i<m_blocks_p[bk].combi.nPathSelect; i++) 
                {
                int path=m_blocks_p[bk].combi.pathSelect[i];
                m_paths_p[path].blockEnd=-1;
                }
            m_blocks_p[bk].combi.nPathSelect=0; 
            }
        if(m_blocks_p[bk].type== (char)Type::SEQ && m_blocks_p[bk].FF.enable==false) //si existe linea de validación
                {
                int path=m_blocks_p[bk].pathEnable;
                m_paths_p[path].blockEnd=-1;
                m_blocks_p[bk].FF.enable=true; //siempre validado (sin camino para hacerlo)
                }
            
        m_blocks_p[bk].pathEnable=-1;  //Sin caminos de validación
        m_blocks_p[bk].enable=false;
        }
}

//Determina si un camino existe y lo elimina. Busca en todos los bloque 
//retorna -1 si no existe ese camino en ningún bloque o no existe ningún bloque
int Blocks::removePathFromAllBlocks(int path)
{
    int index;
    bool hit=false;
    for(int block=0; block<MAX_BLOCKS; block++) //para todos los bloques
        {
        index=-1;        
        if(!existBlock(block)) continue; //si no existe el bloque, pasamos al siguiente
        
        //caminos de salida
        for(int i=0; i<m_blocks_p[block].nPathOut; i++) //Se busca el lugar que ocupa en al array 
            {
            if ((m_blocks_p[block].type == (char)Type::NODE || m_blocks_p[block].type == (char)Type::TAB) &&
                m_blocks_p[block].nodePathOut[i].path == path)
                    {index = i; break;}//si se encuentra               
            else if (m_blocks_p[block].pathOut[i] == path)
                    {index = i; break;}//si se encuentra
            }
        if(index!=-1) //se encuentra el elemento
            {
            //se desplazan los elementos a la derecha del encontrado un lugar hacia la izquierda para eliminarlo
            if(index<m_blocks_p[block].nPathOut-1) //índice anterior al último
                for(int i=index; i<m_blocks_p[block].nPathOut; i++) //caminos por encima
                    {
                    if(m_blocks_p[block].type== (char)Type::NODE || m_blocks_p[block].type== (char)Type::TAB)
                        {
                        m_blocks_p[block].nodePathOut[i].path=m_blocks_p[block].nodePathOut[i+1].path; //sobrescribe al anterior
                        m_blocks_p[block].nodePathOut[i].bitIni=m_blocks_p[block].nodePathOut[i+1].bitIni; //sobrescribe al anterior
                        m_blocks_p[block].nodePathOut[i].bitEnd=m_blocks_p[block].nodePathOut[i+1].bitEnd; //sobrescribe al anterior
                        }
                    else
                        m_blocks_p[block].pathOut[i]=m_blocks_p[block].pathOut[i+1]; //sobrescribe al anterior
                    }
            m_blocks_p[block].nPathOut--; //se actualizan los elementos
            hit=true;
            }
            
        //caminos de selección
        if(m_blocks_p[block].type== (char)Type::COMBI &&  m_blocks_p[block].combi.nPathSelect>0)
            {
            index=-1;  
            for(int i=0; i<m_blocks_p[block].combi.nPathSelect; i++) //Se busca el lugar que ocupa en al array de los caminos de entrada
                if(m_blocks_p[block].combi.pathSelect[i]==path) {index=i; break;}//si se encuentra
            if(index!=-1) //se encuentra el elemento
                {
                //se desplazan los elementos a la derecha de encontrdo un lugar hacia la izquierda para eliminarlo
                if(index<m_blocks_p[block].combi.nPathSelect-1)
                    for(int i=index; i<m_blocks_p[block].combi.nPathSelect; i++) //caminos por encima
                        m_blocks_p[block].combi.pathSelect[i]=m_blocks_p[block].combi.pathSelect[i+1]; //sobrescribe al anterior
                m_blocks_p[block].combi.nPathSelect--; //se actualizan los elementos
                hit=true;
                }  
            }
            
        //caminos de entrada
        index=-1;  
        for(int i=0; i<m_blocks_p[block].nPathIn; i++) //Se busca el lugar que ocupa en al array de los caminos de entrada
            {
            if ((m_blocks_p[block].type == (char)Type::NODE || m_blocks_p[block].type == (char)Type::TAB) &&
                m_blocks_p[block].nodePathIn[i].path == path)
                    {index = i; break;}//si se encuentra               
            else if (m_blocks_p[block].pathIn[i] == path)
                    {index = i; break;}//si se encuentra
            }
        if(index!=-1) //se encuentra el elemento
            {
            //se desplazan los elementos a la derecha de encontrdo un lugar hacia la izquierda para eliminarlo
            if(index<m_blocks_p[block].nPathIn-1)
                for(int i=index; i<m_blocks_p[block].nPathIn; i++) //caminos por encima
                    {
                    if(m_blocks_p[block].type== (char)Type::NODE || m_blocks_p[block].type== (char)Type::TAB)
                        {
                        m_blocks_p[block].nodePathIn[i].path=m_blocks_p[block].nodePathIn[i+1].path; //sobrescribe al anterior
                        m_blocks_p[block].nodePathIn[i].bitIni=m_blocks_p[block].nodePathIn[i+1].bitIni; //sobrescribe al anterior
                        m_blocks_p[block].nodePathIn[i].bitEnd=m_blocks_p[block].nodePathIn[i+1].bitEnd; //sobrescribe al anterior
                        }
                    else
                        m_blocks_p[block].pathIn[i]=m_blocks_p[block].pathIn[i+1]; //sobrescribe al anterior
                    }
            m_blocks_p[block].nPathIn--; //se actualizan los elementos
            hit=true;
            }  
        //Si, al eliminar el camino, el bloque queda sin ninguna conexión, se elimina también el bloque
        if(isIsolatedBlock(block)) 
            removeBlock(block); 
        }
    if(!hit) return -1; 
    return 0;
}

//elimina los nombres de todos los bloques
void Blocks::removeAllBlocksNameItems()
{
    for(int bk=0; bk<MAX_BLOCKS; bk++)
        {
        if(!existBlock(bk)) continue; // si no existe
        if(m_blocks_p[bk].nameItem) {m_image_p->removeTextItem(m_blocks_p[bk].nameItem); m_blocks_p[bk].nameItem=0;}
        }        
}

//recibe control al pulsar el clock
//Se captura la info de entrada y se inicializa el reloj del retardo
//no se actua en la salida hasta pasado el tiempo de retardo
void Blocks::clockAttn()
{
    for(int bk=0; bk<MAX_BLOCKS; bk++)
        {
        if(!existBlock(bk)) continue; // si no existe
        
        if(m_blocks_p[bk].type== (char)Type::SEQ) //bloque secuencial
            {
            //la acción del clock depende de que el FF esté habilitado
            //puede que la habilitación sea permanente o por cable
            if(m_blocks_p[bk].FF.enable==true || m_paths_p[m_blocks_p[bk].pathEnable].valueEnd.value==1)    
                m_blocks_p[bk].FF.start=true;   //inicio de actividad
            else m_blocks_p[bk].FF.start=false;
            
            //se copia la información de los caminos y se inicializa el reloj de retardo
//            m_blocks_p[bk].currentDelay=m_blocks_p[bk].delay; //retardo
            m_blocks_p[bk].currentDelay=m_blocks_p[bk].delay/m_params_p->deltaT_blocks; //retardo
            switch(m_blocks_p[bk].FF.type) //según el tipo de FF
                {
                case (char)FF::FFD: case (char)FF::FFT: case (char)FF::FSM:
                    {
                    //se copia el valor del camino de entrada
                    int pathIn=m_blocks_p[bk].pathIn[0];    
                    m_blocks_p[bk].FF.valueA=m_paths_p[pathIn].valueEnd.value;                            
                    m_blocks_p[bk].FF.valueAUnknow=m_paths_p[pathIn].valueEnd.unKnow;  
                    }
                    break;
                case (char)FF::FFJK:
                    {
                    //se copian los valores de entrada. El primer camino dado es J y el segundo K
                    int pathIn=m_blocks_p[bk].pathIn[0];    
                    m_blocks_p[bk].FF.valueA=m_paths_p[pathIn].valueEnd.value;                            
                    m_blocks_p[bk].FF.valueAUnknow=m_paths_p[pathIn].valueEnd.unKnow;                            
                    pathIn=m_blocks_p[bk].pathIn[1];    
                    m_blocks_p[bk].FF.valueB=m_paths_p[pathIn].valueEnd.value;                                                   
                    m_blocks_p[bk].FF.valueBUnknow=m_paths_p[pathIn].valueEnd.unKnow;                            
                    }
                    break;
                
                } //end of switch            
            } //end of SEQ
        else if(m_blocks_p[bk].type== (char)Type::MEMORY) //bloque memoria
            m_combi_p->MEMORY(bk, 1);            
        }//end of blocks
        
}

//recibe control a intervalos regulares
void  Blocks::evaluateBlocks()   
{
    for(int bk=0; bk<MAX_BLOCKS; bk++) //para cada bloque
        {
        //control de tiempos en bloque combinacional
        //se decrementa el contador y, cuando se anula, se actua sobre la salida
        //Los latch se evaluan como combinacionales, al carecer de reloj.
        if(existBlock(bk) && (m_blocks_p[bk].type== (char)Type::COMBI || m_blocks_p[bk].type== (char)Type::MEMORY || 
            (m_blocks_p[bk].type== (char)Type::SEQ && m_blocks_p[bk].FF.type== (char)FF::LATCH))
            && m_blocks_p[bk].currentDelay>0 && --m_blocks_p[bk].currentDelay==0)
            {
            if(m_blocks_p[bk].nPathOut>0) //módulo con varias salidas
             {
                //int pathOut = m_blocks_p[bk].combi.pathSelectOut;
                int tmpPath;
                for (int i = 0; i < m_blocks_p[bk].nPathOut; i++)
                {
                tmpPath = m_blocks_p[bk].pathOut[i];
                //si hay cambios
                if (m_paths_p[tmpPath].valueIni.unKnow != m_blocks_p[bk].combi.tmpValueUnknow[i] ||
                    m_paths_p[tmpPath].valueIni.value  != m_blocks_p[bk].combi.tmpValue[i])
                    {
                        startPath(tmpPath, m_blocks_p[bk].combi.tmpValue[i], m_blocks_p[bk].combi.tmpValueUnknow[i]); //salida activa
                    }
                 }
            }
            else //módulo con una única salida
            {
                int pathOut = m_blocks_p[bk].pathOut[0];
                startPath(pathOut, m_blocks_p[bk].combi.tmpValue[0], m_blocks_p[bk].combi.tmpValueUnknow[0]); //salida activa
            }
            
            } 
        }
        
    for(int path=0; path<MAX_PATHS; path++)
        {
        if(!existPath(path)) continue;
            
        if(m_pathsNews[path].news==true) //si hay novedad en la entrada a algún bloque
            {
            int block=m_pathsNews[path].block;  //bloque que debe ser evaluado
//            if(block==19)
//                if (!existBlock(block)) continue;

            if(!existBlock(block)) continue;
            
            //caso especial de Latch: se trata como combi
            if(m_blocks_p[block].type== (char)Type::SEQ && m_blocks_p[block].FF.type== (char)FF::LATCH)
                {
                m_sequ_p->LATCH(block);
                m_pathsNews[path].news=false;
                }
            
            char type=m_blocks_p[block].type;   //tipo de bloque
            switch(type)
                {
                case (char)Type::COMBI:
                    {
                    char type=m_blocks_p[block].combi.type;
                    switch(type)
                        {
                        case (char)Oper::NOT:
                            m_combi_p->NOT(block);
                            break;
                        case (char)Oper::AND:
                            m_combi_p->AND(block);
                            break;
                        case (char)Oper::OR:
                            m_combi_p->OR(block);
                            break;
                        case (char)Oper::XOR:
                            m_combi_p->XOR(block);
                            break;
                        case (char)Oper::NAND:
                            m_combi_p->NAND(block);
                            break;
                        case (char)Oper::NOR:
                            m_combi_p->NOR(block);
                            break;
                        case (char)Oper::XNOR:
                            m_combi_p->XNOR(block);
                            break;
                        case (char)Oper::ADD:
                            m_combi_p->ADD(block);
                            break;
                        case (char)Oper::MUX:
                            m_combi_p->MUX(block);
                            break;
                        case (char)Oper::DMUX:
                            m_combi_p->DMUX(block, 2); //demultiplexor
                            break;
                        case (char)Oper::DECOD:
                            m_combi_p->DMUX(block, 1); //decodificador no inversor
                            break;
                        case (char)Oper::DECODN:
                            m_combi_p->DMUX(block, 0);//decodificador inversor
                            break;
                        case (char)Oper::CMP:
                            m_combi_p->CMP(block);
                            break;
                        case (char)Oper::OTHERS:
                            m_combi_p->OTHERS(block);
                            break;
                        }
                    m_pathsNews[path].news=false;
                    }
                    break;
                case (char)Type::NODE:
                    {
                    m_combi_p->NODE(block);
                    m_pathsNews[path].news=false;
                    }
                    break;
                case (char)Type::TAB:
                    {
                    m_combi_p->TAB(block);
                    m_pathsNews[path].news=false;
                    }
                    break;
                case (char)Type::IN_PORT:
                    {
                    m_pathsNews[path].news=false;
                    }
                    break;
                case (char)Type::OUT_PORT:
                    {
                    m_pathsNews[path].news=false;
                    }
                    break;
                case (char)Type::MEMORY:
                    {
                    m_combi_p->MEMORY(block, 0);
                    m_pathsNews[path].news=false;
                    }
                    break;
//                case LATCH:
//                    m_sequ_p->LATCH(block);
//                    m_pathsNews[path].news=false;
//                    break;
                
                }
            }
        }
    //Los bloque secuenciales no son sensibles al cambio en la información del camino sino al clock        
    for(int bk=0; bk<MAX_BLOCKS; bk++) //para cada bloque
        {
        if(m_blocks_p[bk].type== (char)Type::SEQ && m_blocks_p[bk].FF.start==true && m_blocks_p[bk].currentDelay--==0)
            {
            //aconteció clock y transcurrió el tiempo de retardo    
            char type=m_blocks_p[bk].FF.type;
            switch(type)
                {
                case (char)FF::FFD:
                    m_sequ_p->FFD(bk);
                    break;
                case (char)FF::FFT:
                    m_sequ_p->FFT(bk);
                    break;
                case (char)FF::FFJK:
                    m_sequ_p->FFJK(bk);
                    break;
//                case LATCH:
//                    m_sequ_p->LATCH(bk);
//                    break;
                case (char)FF::FSM:
                    m_sequ_p->FSM(bk);
                    break;
                }
            }
        }
}

 //actualiza los bits en los bloques que conectan con un camino dado
 //Retorna -1 si no existe el camino o los bloque que conecta
int Blocks::updateBitsBlocks(int path)
{
    if(!existPath(path)) return -1;
    int bkIni=m_paths_p[path].blockIni;
    int bkEnd=m_paths_p[path].blockEnd;
    if(!existBlock(bkIni) || !existBlock(bkEnd)) return -1;
    for(int i=0; i<m_blocks_p[bkIni].nPathIn; i++)
        if(m_blocks_p[bkIni].nodePathOut[i].path==path)
            {
            m_blocks_p[bkIni].nodePathOut[i].bitIni=0;
            m_blocks_p[bkIni].nodePathOut[i].bitEnd=m_paths_p[path].nBits;
            break;
            }
    for(int i=0; i<m_blocks_p[bkEnd].nPathOut; i++)
        if(m_blocks_p[bkEnd].nodePathOut[i].path==path)
            {
            m_blocks_p[bkEnd].nodePathOut[i].bitIni=0;
            m_blocks_p[bkEnd].nodePathOut[i].bitEnd=m_paths_p[path].nBits;
            break;
            }
    return 0;
}

//Dibuja un rectángulo sobre el bloque
//El rectángulo se conforma a partir de la posición de los caminos que llegan y salen al bloque
//Retorna el punto sobre el primer segmento más próximo al inicio del camino
//Si el bloque no existe, retorna el punto (-1, -1)
QPointF Blocks::drawRectBlock(int block)
{
    if(!existBlock(block)) return QPointF(-1, -1);  //si no existe el bloque
    QPointF p, pRet, pRet2, minP(0,0);
    int minX=0x7FFFFFFF, minY=0x7FFFFFFF, maxX=0, maxY=0, path; //valores extremos
    if(m_blocks_p[block].type!= (char)Type::TAB)
        {
        if (!(m_blocks_p[block].type == (char)Type::COMBI && 
            (m_blocks_p[block].combi.type == (char)Oper::DECOD || m_blocks_p[block].combi.type == (char)Oper::DECODN)))
            for(int i=0; i<m_blocks_p[block].nPathIn; i++) //max-min X,Y en entradas
            {
            if(m_blocks_p[block].type== (char)Type::NODE) path=m_blocks_p[block].nodePathIn[i].path;
            else path=m_blocks_p[block].pathIn[i];
            if(!existPath(path)) continue;
            p=m_paths_p[path].segment_p[m_paths_p[path].nSegments-1].pEnd;   
            if(p.x()<minX) minX=p.x();
            if(p.x()>maxX) maxX=p.x();
            if(p.y()<minY) minY=p.y();
            if(p.y()>maxY) maxY=p.y();
            }
        for(int i=0; i<m_blocks_p[block].nPathOut; i++) //max-min X,Y en salidas y entradas
            {
            if(m_blocks_p[block].type== (char)Type::NODE) path=m_blocks_p[block].nodePathOut[i].path;
            else path=m_blocks_p[block].pathOut[i];
            if(!existPath(path)) continue;
            p=m_paths_p[path].segment_p[0].pIni;   
            if(p.x()<minX) minX=p.x();
            if(p.x()>maxX) maxX=p.x();
            if(p.y()<minY) minY=p.y();
            if(p.y()>maxY) maxY=p.y();
            }
        if(m_blocks_p[block].type== (char)Type::COMBI && (m_blocks_p[block].combi.type== (char)Oper::MUX ||
            m_blocks_p[block].combi.type == (char)Oper::DECOD || m_blocks_p[block].combi.type == (char)Oper::DECODN))
            for(int i=0; i<m_blocks_p[block].combi.nPathSelect; i++) //max-min X,Y en salidas y entradas
                {
                path=m_blocks_p[block].combi.pathSelect[i];
                if(!existPath(path)) continue;
                p=m_paths_p[path].segment_p[m_paths_p[path].nSegments-1].pEnd;   
                if(p.x()<minX) minX=p.x();
                if(p.x()>maxX) maxX=p.x();
                if(p.y()<minY) minY=p.y();
                if(p.y()>maxY) maxY=p.y();
                }
        if(m_blocks_p[block].type== (char)Type::SEQ && (m_blocks_p[block].FF.enable==false ||
           m_blocks_p[block].FF.type== (char)FF::LATCH))
                {
                path=m_blocks_p[block].pathEnable;
                if(existPath(path))
                    {
                    p=m_paths_p[path].segment_p[m_paths_p[path].nSegments-1].pEnd;   
                    if(p.x()<minX) minX=p.x();
                    if(p.x()>maxX) maxX=p.x();
                    if(p.y()<minY) minY=p.y();
                    if(p.y()>maxY) maxY=p.y();
                    }
                }
        pRet.setX((minX+maxX)/2.0); pRet.setY((minY+maxY)/2.0); 
        
        minX-=10; maxX+=10;
        minY-=10; maxY+=10;
        }
    else //TAB
        {
        int pathOut, pathIn, lastSegment;
        double L, minL=1e32;
        QPointF pOut, pIn, pDiff;
        if(m_blocks_p[block].nPathIn==1) //un único camino de entrada
          for(int i=0; i<m_blocks_p[block].nPathOut; i++) //max-min X,Y en salidas y entradas
            {
            pathOut=m_blocks_p[block].nodePathOut[i].path;
            pOut=m_paths_p[pathOut].segment_p[0].pIni;   
            if(pOut.x()<minX) minX=pOut.x();
            if(pOut.x()>maxX) maxX=pOut.x();
            if(pOut.y()<minY) minY=pOut.y();
            if(pOut.y()>maxY) maxY=pOut.y();
            pRet2=pointToPath(pathOut, pOut, &lastSegment);
            if(pRet2.x()<minX) minX=pRet2.x();
            if(pRet2.x()>maxX) maxX=pRet2.x();
            if(pRet2.y()<minY) minY=pRet2.y();
            if(pRet2.y()>maxY) maxY=pRet2.y();
            pDiff=pRet2-pIn; L=pDiff.x()*pDiff.x() + pDiff.y()*pDiff.y();
            if(L<minL) {minL=L; minP=pRet2;}
            }        
        minX-=5; maxX+=5;
        minY-=5; maxY+=5;                   
        }

    if(m_blocks_p[block].nPathIn==1) pRet=pRet2;
    
    //dibujado
    QPoint p1(minX, minY), p2(maxX, maxY);
    m_blocks_p[block].rec=QRect(p1, p2);
    QPen pen; QBrush brush; 
    QColor inPortColor (0, 200, 0); //verdoso
    QColor outPortColor(200, 0, 0); //rojizo
    QColor othersColor(100, 100, 100); //gris
    if(m_blocks_p[block].type== (char)Type::IN_PORT ) //son especiales
        {
        pen.setStyle(Qt::DashLine); pen.setBrush(QBrush(m_params_p->blockColor, Qt::SolidPattern));//pen.setColor(m_params.arrowColor); 
        brush.setColor(inPortColor); brush.setStyle(Qt::Dense4Pattern);
        }
    else if (m_blocks_p[block].type == (char)Type::OUT_PORT) //son especiales
        {
        pen.setStyle(Qt::DashLine); pen.setBrush(QBrush(m_params_p->blockColor, Qt::SolidPattern));//pen.setColor(m_params.arrowColor); 
        brush.setColor(outPortColor); brush.setStyle(Qt::Dense4Pattern);
        }
    else
        {
        pen.setStyle(Qt::DashLine); pen.setBrush(QBrush(m_params_p->blockColor, Qt::SolidPattern));//pen.setColor(m_params.arrowColor); 
        brush.setColor(othersColor); brush.setStyle(Qt::NoBrush);
        }
        
    if (m_blocks_p[block].recItem)
    {
        m_blocks_p[block].recItem->setPen(pen);
        m_blocks_p[block].recItem->setBrush(brush);
        m_blocks_p[block].recItem->setRect(m_blocks_p[block].rec); //si ya existe 
    }
    else    m_blocks_p[block].recItem=m_image_p->addRect(m_blocks_p[block].rec, pen, brush); //sino, se crea
        
return minP;           
}


//dibuja el nombre del bloque
//Lo posiciona en un lugar central del bloque
void Blocks::drawBlockName(int block)
{
    if(!existBlock(block)) return; //si no existe el bloque
    int x1, y1, x2, y2;
    float xc, yc;
    m_blocks_p[block].rec.getCoords(&x1, &y1, &x2, &y2); //coordenadas del rectángulo del bloque
    xc=(x1+x2)/2.0-20; yc=(y1+y2)/2.0-10;   //posicón de dibujado
    
    //texto adaptado al tipo de bloque
    char txt[25], txt2[25];
    if(m_blocks_p[block].type== (char)Type::COMBI)
        switch(m_blocks_p[block].combi.type)
            {
            case (char)Oper::NOT:   strcpy(txt2, "-NOT");   break;
            case (char)Oper::AND:   strcpy(txt2, "-AND");   break;
            case (char)Oper::OR:    strcpy(txt2, "-OR");    break;
            case (char)Oper::XOR:   strcpy(txt2, "-XOR");   break;
            case (char)Oper::NAND:  strcpy(txt2, "-NAND");  break;
            case (char)Oper::NOR:   strcpy(txt2, "-NOR");   break;
            case (char)Oper::XNOR:  strcpy(txt2, "-XNOR");  break;
            case (char)Oper::ADD:   strcpy(txt2, "-ADD");   break;
            case (char)Oper::MUX:   strcpy(txt2, "-MUX");   break;
            case (char)Oper::DMUX:  strcpy(txt2, "-DMUX");   break;
            case (char)Oper::DECOD:  strcpy(txt2, "-DECOD");   break;
            case (char)Oper::DECODN:  strcpy(txt2, "-DECODN");   break;
            case (char)Oper::CMP:
                    {
                    if(m_blocks_p[block].combi.logicCmp==0)
                        strcpy(txt2, "-CMP(<)");   
                    if(m_blocks_p[block].combi.logicCmp==1)
                        strcpy(txt2, "-CMP(=)");   
                    if(m_blocks_p[block].combi.logicCmp==2)
                        strcpy(txt2, "-CMP(>)");   
                    break;
                    }
            case (char)Oper::OTHERS:strcpy(txt2, "-GEN");   break;
            }
    else if(m_blocks_p[block].type== (char)Type::SEQ)
        switch(m_blocks_p[block].FF.type)
            {
            case  (char)FF::FFD:   strcpy(txt2, "-FFD");   break;
            case  (char)FF::FFT:   strcpy(txt2, "-FFT");   break;
            case  (char)FF::FFJK:  strcpy(txt2, "-FFJK");  break;
            case  (char)FF::LATCH: strcpy(txt2, "-LATCH"); break;
            case  (char)FF::FSM:   strcpy(txt2, "-FSM");   break;
            }
    else if(m_blocks_p[block].type== (char)Type::NODE)       strcpy(txt2, "-NODE");
    else if(m_blocks_p[block].type== (char)Type::TAB)        strcpy(txt2, "-TAB");
    else if(m_blocks_p[block].type== (char)Type::IN_PORT)    strcpy(txt2, "-IN");
    else if(m_blocks_p[block].type== (char)Type::OUT_PORT)   strcpy(txt2, "-OUT");
    else if(m_blocks_p[block].type== (char)Type::MEMORY)     strcpy(txt2, "-MEM");
    
    sprintf(txt, "b(%d)%s", block, txt2);    
    if (m_blocks_p[block].nameItem)
    {
        if (m_blocks_p[block].nameItem) m_image_p->removeTextItem(m_blocks_p[block].nameItem);
     //   m_blocks_p[block].nameItem->setText(txt);    //si existe
    }
    m_blocks_p[block].nameItem=newText(txt, 0, QPointF(xc, yc));           //sino, se crea
}

//dibuja todos los rectángulos sobre los bloques 
void Blocks::drawAllRectBlocks()
{
    if(m_blocks_p)
        for(int bk=0; bk<MAX_BLOCKS; bk++)
                drawRectBlock(bk);

}

//dibuja todos los nombres sobre boques NODE & TAB
void Blocks::drawAllNodeBlocksName()
{
    if(m_blocks_p)
        for(int bk=0; bk<MAX_BLOCKS; bk++)
            if(m_blocks_p[bk].type== (char)Type::NODE || m_blocks_p[bk].type== (char)Type::TAB) drawBlockName(bk);

}

//dibuja todos los nombres sobre boques PORT
void Blocks::drawAllPortsBlocksName()
{
    if(m_blocks_p)
        for(int bk=0; bk<MAX_BLOCKS; bk++)
            if(m_blocks_p[bk].type== (char)Type::IN_PORT || m_blocks_p[bk].type== (char)Type::OUT_PORT) drawBlockName(bk);

}

//dibuja todos los nombres sobre resto de boques (distintos de NODE, TAB y PORT)
void Blocks::drawAllOthersBlocksName()
{
    if(m_blocks_p)
        for(int bk=0; bk<MAX_BLOCKS; bk++)
            if(m_blocks_p[bk].type!= (char)Type::NODE && m_blocks_p[bk].type!= (char)Type::TAB &&
                m_blocks_p[bk].type!= (char)Type::IN_PORT && m_blocks_p[bk].type!= (char)Type::OUT_PORT)
                drawBlockName(bk);
}

//Hace que los rectángulos sobre los bloques sean visibles o no
void Blocks::setRectBlocksVisibles(bool visible)
{
    if(m_blocks_p)
        for(int bk=0; bk<MAX_BLOCKS; bk++)
            {
            if(!existBlock(bk) || m_blocks_p[bk].recItem==0) continue;    
            if((m_blocks_p[bk].type== (char)Type::IN_PORT || m_blocks_p[bk].type== (char)Type::OUT_PORT))
                m_blocks_p[bk].recItem->setVisible(true);     
            else  
                m_blocks_p[bk].recItem->setVisible(visible);   
            }
}

//Hace que los nombres sobre los bloques NODE sean visibles o no
void Blocks::setNodeBlocksNameVisibles(bool visible)
{
    if(m_blocks_p)
        for(int bk=0; bk<MAX_BLOCKS; bk++)
            if(existBlock(bk) && (m_blocks_p[bk].type== (char)Type::NODE || m_blocks_p[bk].type== (char)Type::TAB) && m_blocks_p[bk].nameItem)
                m_blocks_p[bk].nameItem->setVisible(visible);     
}

//Hace que los nombres sobre los bloques PORT sean visibles o no
void Blocks::setPortBlocksNameVisibles(bool visible)
{
    if(m_blocks_p)
        for(int bk=0; bk<MAX_BLOCKS; bk++)
            if(existBlock(bk) && (m_blocks_p[bk].type== (char)Type::IN_PORT || m_blocks_p[bk].type== (char)Type::OUT_PORT) &&
                m_blocks_p[bk].nameItem) m_blocks_p[bk].nameItem->setVisible(visible);     
}

//Hace que los nombres sobre el resto de bloques sean visibles o no (distintos de NODE y PORT)
void Blocks::setOthersBlocksNameVisibles(bool visible)
{
    if(m_blocks_p)
        for(int bk=0; bk<MAX_BLOCKS; bk++)
            if(existBlock(bk) && m_blocks_p[bk].type!= (char)Type::IN_PORT && m_blocks_p[bk].type!= (char)Type::OUT_PORT &&
                m_blocks_p[bk].type!= (char)Type::NODE && m_blocks_p[bk].type!= (char)Type::TAB && m_blocks_p[bk].nameItem) 
                m_blocks_p[bk].nameItem->setVisible(visible);
}

//retorna el top item situado bajo el ratón            
QGraphicsItem* Blocks::getMouseItem()
{
    //Conversión de coordenadas de mouse a view
    QPoint p=m_image_p->m_view_p->mapFromScene(m_image_p->m_scene_p->m_clicLeftPoint);
    
    //Detección del item superior en esa posición
    return(m_image_p->m_view_p->itemAt(p.x(), p.y()));
}

//mueve el texto cuando se arrastra el mouse
void Blocks::moveItem(QGraphicsItem* gItem)
{
    if(m_openedPath==true) drawTmpLineSegment();
    else if (gItem && gItem->type() == 9) //si existe un iten bajo el mouse y el item es del tipo texto
    {
        QPointF point = gItem->mapFromScene(m_image_p->m_scene_p->m_dragPoint); //Nueva posición
        gItem->moveBy(point.x(), point.y()); //desplazamiento
        for (int p = 0; p < MAX_PATHS; p++)
        {
            if (m_paths_p[p].iniItem == gItem) { m_paths_p[p].valueIni.pos += point; break; } //valor inicial del camino
            else if (m_paths_p[p].endItem == gItem) { m_paths_p[p].valueEnd.pos += point; break; } //valor final del camino
        }
    }
}

//Establece los bits de cada bloque
void Blocks::setAllBlockBits()
{
    int bits;
    for(int bk=0; bk<MAX_BLOCKS; bk++)
        if((bits=getBlockBits(bk))==-1) continue;
        else m_blocks_p[bk].nBits=bits;
            
}

//retorna los bits del camino de mayor número de bits de entre los que entran o salen al block
//si el bloque no existe retorna -1
int Blocks::getBlockBits(int block)
{
    if(!existBlock(block)) return -1;
    
    BLOCK *block_p=m_blocks_p+block;; 
    int nBits, maxBits;
    maxBits=0;
    if(block_p->type== (char)Type::NODE || block_p->type== (char)Type::TAB)
        {
        for(int p=0; p<block_p->nPathIn; p++)
            {
            nBits=m_paths_p[block_p->nodePathIn[p].path].nBits;
            if(nBits>maxBits) maxBits=nBits;
            }
        for(int p=0; p<block_p->nPathOut; p++)
            {
            nBits=m_paths_p[block_p->nodePathOut[p].path].nBits;
            if(nBits>maxBits) maxBits=nBits;
            }
        }
    else
        {
        for(int p=0; p<block_p->nPathIn; p++)
            {
            nBits=m_paths_p[block_p->pathIn[p]].nBits;
            if(nBits>maxBits) maxBits=nBits;
            }
        for(int p=0; p<block_p->nPathOut; p++)
            {
            nBits=m_paths_p[block_p->pathOut[p]].nBits;
            if(nBits>maxBits) maxBits=nBits;
            }
        }
    return maxBits;
        
}

//retorna el punto de un camino más próximo al punto dado
//en segment, indica el segmento del camino donde se ubica el punto
//Se hace por iteración
QPointF Blocks::pointToPath(int path, QPointF point, int *segment)
{
    double modul, distance, minDistance=1e32;
    QPointF vec, unitVec, pNearest, pIni, dif, newP; 
    int seg=0;
    for(int s=0; s<m_paths_p[path].nSegments; s++) //búsqueda por segmentos
        {
        vec=(m_paths_p[path].segment_p[s].pEnd-m_paths_p[path].segment_p[s].pIni);
        modul=sqrt(vec.x()*vec.x()+vec.y()*vec.y());
        unitVec=vec/modul;
        pIni=m_paths_p[path].segment_p[s].pIni;
        for(int i=0; i<(int)modul; i++)
            {
            newP=pIni+i*unitVec;    
            dif=point-newP;
            distance=sqrt(dif.x()*dif.x()+dif.y()*dif.y());
            if(distance<minDistance) {minDistance=distance; pNearest=newP; seg=s;}
            }
        }
    if(segment) *segment=seg;
    return pNearest;
}

//analiza una info de texto y la descompone en path, bit inicial y bit final
//ej:txt="2[1:0], 5[4:2]" bPath[0].path=2, bPath[0].bitIni=1, bPath[0].bitEnd=0; bPath[1].path=5..
//Si no se especifica un ango de bits, se asume que la salida coincide con la entrada
int Blocks::getBitsPath(char *txt, BITS_PATH *bPath_p, char nBitsIn)
{
    char line[20], word[20];
    int iLine=0, iWord;
    extractFromFile str;
    for(int i=0; i<10; i++)
        {
        if(str.extractNextWord(txt, &iLine, line, ',')==-1) return i; //sin info
        if(str.searchChar(txt, '[')!=-1)        //si existe info de bits
            {
            iWord=0;    
            if(str.extractNextWord(line, &iWord, word, '[')==-1) return -1; //path
            bPath_p[i].path=atoi(word);
            if(str.extractNextWord(line, &iWord, word, ':')==-1) return -1; //bit ini
            bPath_p[i].bitIni=atoi(word);
            if(str.extractNextWord(line, &iWord, word, ']')==-1) return -1; //bit end
            bPath_p[i].bitEnd=atoi(word);
            }
        else
            {
            bPath_p[i].path=atoi(line);
            bPath_p[i].bitIni=nBitsIn-1;
            bPath_p[i].bitEnd=0;
            }
        }
    return 0;
}

