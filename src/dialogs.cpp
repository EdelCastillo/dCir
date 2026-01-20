/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 *
 *      Dialogos sobre caminos y bloques bloques
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

#include "dialogs.h"
bool extern gEdit, gPrjSaved;

//Constructor
Dialogs::Dialogs(class Blocks *cBlocks_p, PARAMS *params_p)
{
    m_blocks_p=cBlocks_p->m_blocks_p;
    m_paths_p=cBlocks_p->m_paths_p;
    m_cBlocks_p=cBlocks_p;
    m_openedPath=false;   //no iniciado la creación de un camino
    m_closedPath=true;  
    m_params_p=params_p;
}

//slot: Recibe control al clicar sobre el botón izquierdo del ratón
//se usa para generar nuevos caminos
//toma nota de los puntos marcados en el camino 'left clik', y dibuja los segmentos
//m_gItem -> mantiene la referencia al item superior situado bajo el ratón
void Dialogs::bottonLeftAttn()
{
    m_gItem=m_cBlocks_p->getMouseItem(); //Captura del item superior
    if(m_openedPath==true)               //si se ha iniciado la captura de datos para la creación de un nuevo camino
        m_cBlocks_p->drawLineSegment(); //se adquiere la información de un nuevo camino, punto a punto
        
}

//mueve el texto cuando se arrastra el mouse
void Dialogs::dragMouseAttn()
{
    m_cBlocks_p->moveItem(m_gItem);
}

//Recibe control al clicar sobre el botón derecho del ratón
//Gestiona la captura de datos para conformar los caminos y los bloques
void Dialogs::bottonRightAttn()
{
    if(!m_openedPath && !m_pathMode) //si no existe un camino abierto
        {
        int block=-1;
        QPointF p=m_cBlocks_p->m_image_p->mouseRightBottonPoint(); //coordenadas del ratón
        
        //determina si la posición del click derecho coincide con algún bloque
        for(int b=0; b<MAX_BLOCKS; b++)
            {
            int x=p.x(), y=p.y();
            if(m_cBlocks_p->m_blocks_p[b].enable && m_cBlocks_p->m_blocks_p[b].rec.contains(x, y)) 
                {block=b; break;} //coincidencia
            }
            
        if(block!=-1)                           //si el ratón está sobre algún bloque
            editBlock(block);      //edición del bloque
            
        else if(m_blockMode) //click fuera de cualquier bloque pero modo bloque
            {
            block = m_cBlocks_p->getFreeBlock();      //se consigue el primer bloque libre
            editBlock(block);          //creación y edición del bloque
            }
        else
            {
            BottonRightDialog bottonRightDialog;        //puede tratarse de una acción sobre caminos o bloques
            bottonRightDialog.exec(); 
            if(bottonRightDialog.m_result==4) return;   //cancelado
            
            if(bottonRightDialog.m_result==1)           //acción sobre bloques
                {
                block=m_cBlocks_p->getFreeBlock();      //se consigue el primer bloque libre
                editBlock(block);          //creación y edición del bloque
                }
            else if(bottonRightDialog.m_result==2)      //acción sobre caminos
                pathsAttn();                            //atención a la acción sobre caminos
            }
        }
    //si existe un camino abierto
    else pathsAttn(); //atención a la acción sobre caminos
    
 }

//se captura información sobre los caminos y se actualiza en la base de datos
void Dialogs::pathsAttn()
{   
    static int lastPath=0, lastPathBits=1;
    PathDialog pathDialog; //dialogo para la captura de datos
    char txt[50];
    pathDialog.pathValue->setReadOnly(true); //el nombre del camino no se puede editar

    while(true) //control de la información de entrada
        {
        //se presenta info por defecto
        sprintf(txt, "%d", lastPathBits);              //#bits
        pathDialog.bitsValue->setText((txt));       //se visualiza
        if(m_openedPath) //Si el camino ya está iniciado, no se permite modificar su nombre
            {
            sprintf(txt, "p(%d)", lastPath);        //referencia al camino
            pathDialog.pathValue->setText((txt));   //se visualiza
            }
        else  //nuevo camino
            {
            lastPath=m_cBlocks_p->getFreePath();    //referencia al primer camino libre
            if(lastPath==-1) 
                {
                sprintf(txt, "Warning: some problem reading the file %s\n.", txt);
                QMessageBox::warning(0, "Paths", "Too many paths", QMessageBox::Cancel);
                return;
                }
            sprintf(txt, "p(%d)", lastPath);
            pathDialog.pathValue->setText((txt));   //se visualiza
            }
            
        pathDialog.exec(); //1->open; 2->close; 3->remove; 4->cancel; 5->update
        
        //captura de datos
        
        lastPathBits= pathDialog.bitsValue->text().toInt(); //anchura del bus
        
       //Si el camino ya existe, no se avanza
        if(m_cBlocks_p->existPath(lastPath) && (pathDialog.m_result==1 || pathDialog.m_result==2)) 
            {
            sprintf(txt, "Warning: path p(%d) exist", lastPath);
            QMessageBox::warning(0, "Paths", txt, QMessageBox::Cancel);
            continue;
            }
        else  //el camino avanza en su creación
            {
            if(pathDialog.m_result==1)              //se inicializa la captura de datos de un nuevo camino
                {
                if(m_openedPath==true)              //no se puede reabrir un camino abierto
                {
                    sprintf(txt, "Warning: path p(%d) is open. Please close it", lastPath);
                    QMessageBox::warning(0, "Paths", txt, QMessageBox::Cancel);
                    continue;
                }
                m_openedPath=true;                  //nuevo camino 
                m_closedPath=false;                 //no ha terminado su recorrido
                m_cBlocks_p->initGetPointsPath(lastPath, lastPathBits);   //inicializa la captura de datos
                }
            else if(pathDialog.m_result==2)         //se termina el camino
                {
                if(m_openedPath==false) continue;   //si no estaba abierto
                m_openedPath=false;   
                m_closedPath=true;                  //fin del camino
                m_cBlocks_p->finishGetPointsPath(lastPath, lastPathBits); //toda la info capturada
                lastPath=-1;                        //para la vez siguiente
                gEdit = true; //variable global: indica que se ha creado algún nuevo camino (modo edición de caminos)
                gPrjSaved = false; //proyecto no salvado
            }
            else if(pathDialog.m_result==3 || pathDialog.m_result==4) //eliminar o cancelar el camino
                {
                if(m_openedPath==true)              //si el camino sigue abierto
                    m_cBlocks_p->removeTmpPath();   //se elimina
                    
                else if(pathDialog.m_result==3)     //eliminar un camino ya creado
                    {
                    int delPath=pathDialog.delPathValue->text().toInt();    //camino a eliminar
                    if(!m_cBlocks_p->existPath(delPath))                    //solo si existe
                    {
                        sprintf(txt, "Warning: path(%d) do not exist", lastPath);
                        QMessageBox::warning(0, "Paths", txt, QMessageBox::Cancel);
                        continue;
                    }
                    if(m_cBlocks_p->removePath(delPath)==-1)                //no se pudo eliminar
                    {
                        sprintf(txt, "Warning: It is not possible to remove the path %d", delPath);
                        QMessageBox::warning(0, "Paths", txt, QMessageBox::Cancel);
                        continue;
                    }
                    m_cBlocks_p->removePathFromAllBlocks(delPath);          //el camino se elimina de todos los bloques
                    }
                    m_openedPath = false; //camino concluido
                    m_closedPath = false;
                }
            else if (pathDialog.m_result == 5)         //se actualizan los bits del camino
            {
                if (m_openedPath == true) continue;   //si esta abierto no se hace nada
                m_closedPath = true;                  //fin del camino
                int path = pathDialog.delPathValue->text().toInt();    //camino a actualizar
                int bits = pathDialog.bitsValue->text().toInt();    //bits
                m_cBlocks_p->updateBitsPath(path, bits); //bits actualizados en camino
                //si el camino está conectado, se deben actualizar los bloques
                m_cBlocks_p->updateBitsBlocks(path); //bits actualizados en los bloques conectados
                }
                break; //todo bien
            }
        }//while(true)
}

//se presenta información del bloque, permitiendo su edición
//el bloque se identifica a partir del click derecho en el ratón
void Dialogs::editBlock(int block)
{
    //mantiene los valores anteriores
    static int blockType = (char)Type::COMBI, combiType = (char)Oper::NOT, FFType = (char)FF::FFD, logicCmp = 0;
    //Debe iniciarse con la ventana de mayor tamaño para que opere bien con todas
    bool newBlock = false;
    char txt[100];
    BlockDialog blockDialog;                    //diálogo
    blockDialog.nameValue->setReadOnly(true);   //nombre del camino bloqueado
    if (!m_cBlocks_p->existBlock(block)) newBlock = true;

    Common tools;       //herramientas para convertir cadenas de texto en cadenas de enteros y viceversa
    bool hit;
    int pathsIn[MAX_BLOCK_IN],      //array para alojar los caminos de entrada al bloque
        pathsOut[MAX_BLOCK_IN],     //array para alojar los caminos de salida  al bloque
        pathsSelect[MAX_BLOCK_IN],  //array para alojar los caminos de selección (mux)
        nPathsIn=0, nPathsOut=0,        //# de entradas y salidas
        nPathsSelect, pathEnable;
    BITS_PATH nodePathOut[10];
    BITS_PATH nodePathIn[10];
    float delay;                    //retardo
    int type;                       //tipo de bloque
    int value;
    bool unKnow = false;

    //itera hasta que no hayan errores en los datos o se desee salir
    while (true)                     
    {
        //presentación de información actual
        sprintf(txt, "%d", block);                  //identificación del bloque
        blockDialog.nameValue->setText((txt));
        if (!newBlock)
            blockType = m_blocks_p[block].type;
        blockDialog.Ctype->setCurrentIndex(blockType); //tipo de bloque a editar
        blockDialog.initValue->setText("0x0");          //valor por defecto

        //bloque del tipo COMBY o MEMORY
        if (blockType == (char)Type::COMBI || blockType == (char)Type::MEMORY) //combinacional
        {
            if (!newBlock) //si se trata de un bloque ya creado
            {
                combiType = m_blocks_p[block].combi.type;
                logicCmp = m_blocks_p[block].combi.logicCmp;

                //caminos que salen del bloque
                txt[0] = 0;
                for (int i = 0; i < m_blocks_p[block].nPathOut; i++)
                {
                    int path = m_blocks_p[block].pathOut[i];
                    sprintf(txt + strlen(txt), "%d, ", path);
                }
                blockDialog.pathOutValue->setText(txt);

                //caminos de selección para mux
                txt[0] = 0;
                for (int i = 0; i < m_blocks_p[block].combi.nPathSelect; i++)
                {
                    int path = m_blocks_p[block].combi.pathSelect[i];
                    sprintf(txt + strlen(txt), "%d, ", path);
                }
                blockDialog.selectValue->setText(txt);
                if ((blockType == (char)Type::COMBI && combiType == (char)Oper::OTHERS) || blockType == (char)Type::MEMORY)
                    blockDialog.TTfileValue->setText(m_blocks_p[block].TT.fileName);

                //caminos que llegan al bloque
                txt[0] = 0;
                for (int i = 0; i < m_blocks_p[block].nPathIn; i++)
                {
                    int path = m_blocks_p[block].pathIn[i];
                    sprintf(txt + strlen(txt), "%d, ", path);
                }
                blockDialog.pathInValue->setText(txt);
            }
            else //se trata de nuevo bloque
            {
                txt[0] = 0;
                blockDialog.pathOutValue->setText(txt);
                blockDialog.TTfileValue->setText(txt);
                blockDialog.selectValue->setText(txt);
                blockDialog.pathInValue->setText(txt);
                for (int i = 0; i < MAX_BLOCKS_PATHS; i++) //se inicializan los caminos
                    {
                    m_blocks_p[block].pathIn[i] = -1;
                    m_blocks_p[block].nPathIn = 0;
                    m_blocks_p[block].pathOut[i] = -1;
                    m_blocks_p[block].nPathOut = 0;
                    m_blocks_p[block].pathEnable = -1;
                    m_blocks_p[block].combi.pathSelect[i] = -1;
                    m_blocks_p[block].combi.nPathSelect = 0;
                    }
            }
        //Si es bloque COMBI
        if(blockType== (char)Type::COMBI)
            {
            blockDialog.Coperation->setCurrentIndex(combiType); //tipo de combi 
            //si comparador lógico
            if(combiType== (char)Oper::CMP)
                {
                blockDialog.ClogicCmp->setVisible(true);
                blockDialog.ClogicCmp->setCurrentIndex(logicCmp);
                }
            else
                blockDialog.ClogicCmp->setVisible(false);
            if(combiType== (char)Oper::DECOD || combiType== (char)Oper::DECODN)
                   blockDialog.pathInValue->setVisible(false);
            else 
                   blockDialog.pathInValue->setVisible(true);

            }
        }
    //Si no es COMBI ni MEMORY pero es SEQU   
    else if(blockType== (char)Type::SEQ) //secuencial (FFs)
        {
        if(newBlock) //Si es un bloque nuevo
            {
            m_blocks_p[block].FF.resetValueUnknow=false;
            m_blocks_p[block].FF.resetValue=0;                
            m_blocks_p[block].pathEnable=-1;                
            }
        else //el bloque ya existe
            {
            FFType=m_blocks_p[block].FF.type;
            //caminos que llegan al bloque
            txt[0]=0;
            for(int i=0; i<m_blocks_p[block].nPathIn; i++)
                {
                int path=m_blocks_p[block].pathIn[i];
                sprintf(txt+strlen(txt), "%d, ", path);
                }
            blockDialog.pathInValue->setText((txt));
            //caminos que salen del bloque
            txt[0]=0;
            for(int i=0; i<m_blocks_p[block].nPathOut; i++)
                {
                int path=m_blocks_p[block].pathOut[i];
                sprintf(txt+strlen(txt), "%d, ", path);
                }
            blockDialog.pathOutValue->setText((txt));
            if(FFType== (char)FF::FSM)
                blockDialog.TTfileValue->setText(m_blocks_p[block].TT.fileName);
            }
        //para ambos casos (nuevos y ya creados)
        if(m_blocks_p[block].FF.resetValueUnknow==true) 
            sprintf(txt, "x");
        else m_cBlocks_p->toText(m_blocks_p[block].FF.resetValue, txt, m_blocks_p[block].nBits);
        blockDialog.initValue->setText((txt));
    
        blockDialog.CflipFlop->setCurrentIndex(FFType);
        if(m_blocks_p[block].FF.type!= (char)FF::LATCH) //caso de no LATCH
            {
            blockDialog.CkEnableFF->setVisible(true);
            if(m_blocks_p[block].FF.enable==true) //Si está siempre enable
                blockDialog.CkEnableFF->setCheckState(Qt::Checked);
            else
                {
                blockDialog.CkEnableFF->setCheckState(Qt::Unchecked);
                sprintf(txt, "%d", m_blocks_p[block].pathEnable);
                blockDialog.enableValue->setText(txt);            
                }
            }
        else //caso de latch
            {
            blockDialog.CkEnableFF->setVisible(false);
            if(!newBlock)    sprintf(txt, "%d", m_blocks_p[block].pathEnable);//bloque viejo
            else    txt[0]=0; //nuevo
            blockDialog.enableValue->setText(txt);            
            }
        }
    //Si no es COMBI ni MEMORY ni SEQU pero es NODE o TAB
    else if(blockType== (char)Type::NODE || blockType== (char)Type::TAB)
        {
        txt[0]=0;
        if(!newBlock) //si bloque viejo
            {
            for(int i=0; i<m_blocks_p[block].nPathIn; i++) //info de caminos de entrada
                {
                sprintf(txt+strlen(txt),"%d[%d:%d], ", m_blocks_p[block].nodePathIn[i].path, 
                        m_blocks_p[block].nodePathIn[i].bitIni, m_blocks_p[block].nodePathIn[i].bitEnd);
                }
            blockDialog.pathInValue->setText((txt));
            txt[0]=0;
            for(int i=0; i<m_blocks_p[block].nPathOut; i++) //info de caminos de salida
                {
                sprintf(txt+strlen(txt),"%d[%d:%d], ", m_blocks_p[block].nodePathOut[i].path, 
                        m_blocks_p[block].nodePathOut[i].bitIni, m_blocks_p[block].nodePathOut[i].bitEnd);
                }
            blockDialog.pathOutValue->setText((txt));
            }
        else //bloque nuevo
            {
            blockDialog.pathOutValue->setText(txt);
            blockDialog.pathInValue ->setText(txt);        
            }
        }

    //Si no es COMBI ni MEMORY ni SEQU ni NODE ni TAB pero es IN_PORT
    else if(blockType== (char)Type::IN_PORT) //Puerto de entrada
        {
        if(newBlock) //si es nuevo el bloque
            {
            m_blocks_p[block].initPortValueUnknow=false;
            m_blocks_p[block].initPortValue=0;
            }
        else  //es viejo
            {
            //caminos que salen del bloque
            txt[0]=0;
            for(int i=0; i<m_blocks_p[block].nPathOut; i++) //info de caminos que salen
                {
                int path=m_blocks_p[block].pathOut[i];
                sprintf(txt+strlen(txt), "%d, ", path);
                }
            blockDialog.pathOutValue->setText((txt));
            }
        if(m_blocks_p[block].initPortValueUnknow==true)    //caso de info desconocida
            sprintf(txt, "x");
        else m_cBlocks_p->toText(m_blocks_p[block].initPortValue, txt, m_blocks_p[block].nBits);
        blockDialog.initValue->setText((txt));    
        }
    
    //Si no es COMBI ni MEMORY ni SEQU ni NODE ni TAB ni IN_PORT pero es OUT_PORT
    else if(blockType== (char)Type::OUT_PORT) //Puerto de entrada
        {
        if(!newBlock) //si es viejo
            {    
            //caminos que entran al bloque
            txt[0]=0;
            for(int i=0; i<m_blocks_p[block].nPathIn; i++)
                {
                int path=m_blocks_p[block].pathIn[i];
                sprintf(txt+strlen(txt), "%d, ", path);
                }
            blockDialog.pathInValue->setText((txt));
            }
        }
    //para todos los casos                   
    blockDialog.handleCurrentIndexBaseChanged(blockType);  //inicializa los campos visibles
       
    //Conversión de tics a segundos
//    float delay_s=(m_blocks_p[block].delay*m_params_p->deltaT_blocks)/1000.0;
    float delay_s=m_blocks_p[block].delay/1000.0;  //ms -> s
    sprintf(txt, "%.1f", delay_s);
    blockDialog.delayValue->setText((txt));
 
    //////////////////////////////////////////////////////////////
    //Se crea la ventana
    blockDialog.exec(); //presentación y captura de datos
    //////////////////////////////////////////////////////////////
    
    gEdit = false;

    if (blockDialog.m_result == 4 || blockDialog.m_result == 0) return;     //botón  cancel o abortar
   
    //Para tratar con los posibles errores de usuario, primero se captura la información a variables temporales
    else if(blockDialog.m_result==3)        //botón  remove (eliminación del bloque)
        {
        if(m_cBlocks_p->existBlock(block))               //si el bloque existe, se elimina y termina
            {m_cBlocks_p->removeBlock(block); return;}
        else                                //error de identificación: itera
            {
            sprintf(txt, "Warning: cannot delete the block b(%d)", block);
            QMessageBox::warning(0, "Paths", txt, QMessageBox::Cancel);
            }
        }
    else  if(blockDialog.m_result==1)       //botón accept
        {
        hit=false; 
        if (newBlock) {
            gEdit = true; //nuevo bloque añadido
            gPrjSaved = false; //proyecto no salvado
        }

        //Captura temporal de datos para poder volver atrás si algo falla
        /////////////////////////////////////////////////////////////////
        blockType=blockDialog.Ctype->currentIndex();         //tipo de bloque
        delay=blockDialog.delayValue->text().toFloat(); //retardo
        if(delay<0)  //control de fallos
            {
            QMessageBox::warning(0, "Paths", "Warning: time is negative", QMessageBox::Cancel);
            continue;
            }
        //tratamiento específico para cada tipo de bloque
        nPathsIn=0;  
        nPathsOut=0;  
        if (blockType== (char)Type::NODE || blockType== (char)Type::TAB) //
                {
                char txt2[100];
                //capturamos info de entradas y salidas
                QlineEditToChar(blockDialog.pathInValue, txt);  //captura de info de los puertos de entrada
                if((nPathsIn=m_cBlocks_p->getBitsPath(txt, nodePathIn, 0))==-1) continue;

                QlineEditToChar(blockDialog.pathOutValue, txt2);  //captura de info de los puertos de salida  
                if((nPathsOut=m_cBlocks_p->getBitsPath(txt2, nodePathOut, 0))==-1) continue;

                //en TAB, el camino de entrada y salida no pueden coincidir
                if (blockType == (char)Type::TAB && nPathsIn == 1 && nPathsOut == 1 && nodePathIn[0].path == nodePathOut[0].path)
                    continue;
                //Sólo puede existir un único camino en la sección de las entradas o de las salidas
                //si es en las entradas, pueden existir bifurcaciones de anchos variados en las salidas
                //Si es en la salida,  pueden existir bifurcaciones de anchos variados en las entradas
                //Si solo existe un camino de entrada y uno de ssalida, deben coincidir en sus anchos
                if(nPathsIn==1 && nPathsOut==1 && m_paths_p[nodePathIn[0].path].nBits == m_paths_p[nodePathOut[0].path].nBits) 
                    {
                    nodePathIn[0]. bitIni=m_paths_p[nodePathIn[0].path].nBits-1;
                    nodePathOut[0].bitIni=m_paths_p[nodePathOut[0].path].nBits-1;
                    nodePathIn[0].bitEnd = 0;
                    nodePathOut[0].bitEnd = 0;
                    }
                else if(nPathsIn==1) //un camino de entrada que puede bifurcarse a las salidas
                    {
                    int nBitsIn=m_paths_p[nodePathIn[0].path].nBits;
                    if((nPathsOut=m_cBlocks_p->getBitsPath(txt2, nodePathOut, nBitsIn))==-1) continue; 
                    nodePathIn[0].bitIni = nBitsIn-1;
                    nodePathIn[0].bitEnd=0;
                    }
                else if(nPathsOut==1)//un camino de salida que puede componerse a partir de entradas de ancho variado
                    {
                    int nBitsOut=m_paths_p[nodePathOut[0].path].nBits;
                    if((nPathsIn=m_cBlocks_p->getBitsPath(txt, nodePathIn, nBitsOut))==-1) continue;                    
                    nodePathOut[0].bitIni=nBitsOut-1;
                    nodePathOut[0].bitEnd =0;
                    }
                else //varios caminos de entrada y salida
                    {

                    }
                }
        else if (blockType== (char)Type::OUT_PORT) //si no se trata de un puerto de entrada
                {
                QlineEditToChar(blockDialog.pathInValue, txt);  //captura de info de los puertos de entrada
                nPathsIn=tools.strToArray(txt, pathsIn, MAX_BLOCK_IN);
                nPathsOut=0;
                }
        else if (blockType== (char)Type::IN_PORT) //si no se trata de un puerto de salida
                {
                hit=true;
                QlineEditToChar(blockDialog.pathOutValue, txt); //captura de info de los puertos de salida
                nPathsOut=tools.strToArray(txt, pathsOut, MAX_BLOCK_IN);
                nPathsIn=0;
                }
        else if(blockType== (char)Type::COMBI)
                {
                QlineEditToChar(blockDialog.pathInValue, txt);  //captura de info de los puertos de entrada
                nPathsIn=tools.strToArray(txt, pathsIn, MAX_BLOCK_IN);
                QlineEditToChar(blockDialog.pathOutValue, txt); //captura de info de los puertos de salida
                nPathsOut=tools.strToArray(txt, pathsOut, MAX_BLOCK_IN);
                if( blockDialog.Coperation->currentIndex() == (char)Oper::MUX   || 
                    blockDialog.Coperation->currentIndex() == (char)Oper::DMUX  ||
                    blockDialog.Coperation->currentIndex() == (char)Oper::DECOD ||
                    blockDialog.Coperation->currentIndex() == (char)Oper::DECODN)
                    {
                    QlineEditToChar(blockDialog.selectValue, txt); //captura de info de los puertos de selección
                    nPathsSelect=tools.strToArray(txt, pathsSelect, MAX_BLOCK_IN);                    
                    pathEnable=blockDialog.enableValue->text().toInt();
                    if(!m_cBlocks_p->existPath(pathEnable)) 
                        {
                        sprintf(txt, "Warning: the path %d do not exist", pathEnable);
                        QMessageBox::warning(0, "Paths", txt, QMessageBox::Cancel);
                        continue;
                        }
                    }
                if(blockDialog.Coperation->currentIndex()== (char)Oper::CMP)
                    {
                    logicCmp=blockDialog.ClogicCmp->currentIndex();
                    }
                    //combiType=blockType;  
                combiType=blockDialog.Coperation->currentIndex();
                }
        else if(blockType== (char)Type::SEQ)
                {
                QlineEditToChar(blockDialog.pathInValue, txt);  //captura de info de los puertos de entrada
                nPathsIn=tools.strToArray(txt, pathsIn, MAX_BLOCK_IN);
                QlineEditToChar(blockDialog.pathOutValue, txt); //captura de info de los puertos de salida
                nPathsOut=tools.strToArray(txt, pathsOut, MAX_BLOCK_IN);            
                if(blockDialog.CkEnableFF->checkState()==Qt::Unchecked || blockDialog.Coperation->currentIndex()== (char)FF::LATCH)
                    {
                    pathEnable=blockDialog.enableValue->text().toInt();
                    if(!m_cBlocks_p->existPath(pathEnable)) 
                    {
                        sprintf(txt, "Warning: the path %d do not exist", pathEnable);
                        QMessageBox::warning(0, "Paths", txt, QMessageBox::Cancel);
                        continue;
                    }
                    }
                    //FFType=blockType; 
                    FFType=blockDialog.CflipFlop->currentIndex();
                }
        else if(blockType== (char)Type::MEMORY)
                {
                QlineEditToChar(blockDialog.pathInValue, txt);  //captura de info de los puertos de entrada
                nPathsIn=tools.strToArray(txt, pathsIn, MAX_BLOCK_IN);
                QlineEditToChar(blockDialog.pathOutValue, txt); //captura de info de los puertos de salida
                nPathsOut=tools.strToArray(txt, pathsOut, MAX_BLOCK_IN);
                }
        hit=true;
        if(blockType!= (char)Type::NODE && blockType!= (char)Type::TAB)
                {
                for(int i=0; i<nPathsIn; i++)                   //control de fallos
                    if(!m_cBlocks_p->existPath(pathsIn[i]) && pathsIn[i]!=-1) //se acepta -1 como indicador de no camino (entrada al aire)
                {
                    sprintf(txt, "Warning: the path %d do not exist", pathsIn[i]);
                    QMessageBox::warning(0, "Paths", txt, QMessageBox::Cancel);
                    hit = false; break;
                }
                if(!hit) continue;
                if(nPathsIn==-1) 
                {
                    QMessageBox::warning(0, "Paths", "Too many inputs en block", QMessageBox::Cancel);
                    continue;
                }
                if(nPathsIn==0 && (blockType!= (char)Type::IN_PORT &&
                    (blockType== (char)Type::COMBI && blockDialog.Coperation->currentIndex() != (char)Oper::DECOD) &&
                    (blockType== (char)Type::COMBI && blockDialog.Coperation->currentIndex() != (char)Oper::DECODN)))
                {
                    sprintf(txt, "Warning: There are no input paths on the block b(%d)", block);
                    QMessageBox::warning(0, "Paths", txt, QMessageBox::Cancel);
                    continue;
                }

                for(int i=0; i<nPathsOut; i++)                  //control de fallos
                    if(!m_cBlocks_p->existPath(pathsOut[i]) && pathsOut[i]!=-1) 
                {
                    sprintf(txt, "Warning: the path %d do not exist", pathsOut[i]);
                    QMessageBox::warning(0, "Paths", txt, QMessageBox::Cancel);
                    hit = false; break;
                }
                if(!hit) continue;
                if(nPathsOut==-1) 
                {
                    QMessageBox::warning(0, "Paths", "Too many outputs in block", QMessageBox::Cancel);
                    continue;
                }
                if(nPathsOut==0 && blockType!= (char)Type::OUT_PORT)
                {
                    sprintf(txt, "Warning: There are no output paths on the block b(%d)", block);
                    QMessageBox::warning(0, "Paths", txt, QMessageBox::Cancel);
                    continue;
                }
                }
        //valor de inicialización
        //se considera el formato hexadecimal (binario para Qt ver>=6)
        if(blockType== (char)Type::SEQ || blockType== (char)Type::IN_PORT)
                {
                unKnow=false;
                hit=false;
                QString str=blockDialog.initValue->text();
                if(str.contains("0x", Qt::CaseInsensitive) || str.contains("0b", Qt::CaseInsensitive) ||
                    str.contains("O", Qt::CaseInsensitive))
                    {
                    value=str.toInt(&hit, 0);
                    if(hit==false) continue;
                    }
                else if (str.contains("x", Qt::CaseInsensitive)) {unKnow = true; value = 0;}
                else value=blockDialog.initValue->text().toInt();
                } 
            }
        break;  //info correcta-> fin de iteración
        }   
        
    //la info, considerada válida, se registra 
    ////////////////////////////////////////
    if (nPathsIn == 0 && nPathsOut == 0) return;

    //tratamiento específco según el tipo de bloque
    m_blocks_p[block].type=blockType;
    if(blockType== (char)Type::COMBI || blockType== (char)Type::MEMORY) //bloque combinacional
        {
        for(int i=0; i<nPathsIn; i++)   //copia de caminos de entrada
            {
            m_blocks_p[block].pathIn[i]=pathsIn[i];                    
            m_paths_p[pathsIn[i]].blockEnd=block;
            }
        m_blocks_p[block].nPathIn=nPathsIn;
        for(int i=0; i<nPathsOut; i++)  //copia de caminos de salida
            {
            m_blocks_p[block].pathOut[i]=pathsOut[i];
            m_paths_p[pathsOut[i]].blockIni=block;
            }
        m_blocks_p[block].nPathOut=nPathsOut;
        if(blockType== (char)Type::COMBI && 
            (blockDialog.Coperation->currentIndex()== (char)Oper::MUX || 
            blockDialog.Coperation->currentIndex() == (char)Oper::DMUX ||
            blockDialog.Coperation->currentIndex() == (char)Oper::DECOD ||
            blockDialog.Coperation->currentIndex() == (char)Oper::DECODN))
            {
            for(int i=0; i<nPathsSelect; i++)  //copia de caminos de selección para mux
                m_blocks_p[block].combi.pathSelect[i]=pathsSelect[i];
            m_blocks_p[block].combi.nPathSelect=nPathsSelect;
            m_blocks_p[block].pathEnable=pathEnable;
            for(int i=0; i<nPathsSelect; i++)
                m_paths_p[m_blocks_p[block].combi.pathSelect[i]].blockEnd=block;                
            }
        if(blockType== (char)Type::COMBI) m_blocks_p[block].combi.type=blockDialog.Coperation->currentIndex(); //tipo
        if((blockType== (char)Type::COMBI && m_blocks_p[block].combi.type== (char)Oper::OTHERS) || blockType== (char)Type::MEMORY)
            {
            Files file;
            if (m_blocks_p[block].TT.fileName) //si existe memoria reservada anterior, se elimina y rehace
            {
                delete [] m_blocks_p[block].TT.fileName;
                m_blocks_p[block].TT.fileName = new char[200];
                m_blocks_p[block].TT.table_p = 0;
            }
            QlineEditToChar(blockDialog.TTfileValue, m_blocks_p[block].TT.fileName);
            if(blockType== (char)Type::MEMORY)   file.fileToMemory(&m_blocks_p[block].TT);
            else                    file.fileToTrueTable(&m_blocks_p[block].TT, false);
            }
        if(blockType== (char)Type::COMBI && blockDialog.Coperation->currentIndex()== (char)Oper::CMP)
            {
            m_blocks_p[block].combi.logicCmp=logicCmp;
            }
        }
    else if(blockType== (char)Type::SEQ) //bloque secuencial
        {
        for(int i=0; i<nPathsIn; i++)   //copia de caminos de entrada
            {
            m_blocks_p[block].pathIn[i]=pathsIn[i];                    
            m_paths_p[pathsIn[i]].blockEnd=block;
            }
        m_blocks_p[block].nPathIn=nPathsIn;
        for(int i=0; i<nPathsOut; i++)  //copia de caminos de salida
            {
            m_blocks_p[block].pathOut[i]=pathsOut[i];
            m_paths_p[pathsOut[i]].blockIni=block;
            }
        m_blocks_p[block].nPathOut=nPathsOut;
        m_blocks_p[block].FF.type=blockDialog.CflipFlop->currentIndex(); //tipo
        m_blocks_p[block].FF.resetValue=value;//valor de inicialización
        m_blocks_p[block].FF.resetValueUnknow=unKnow;//valor de inicialización
        if(blockDialog.CkEnableFF->checkState()==Qt::Checked && 
            blockDialog.Coperation->currentIndex()!= (char)FF::LATCH)
                m_blocks_p[block].FF.enable=true;
        else 
            {
            m_blocks_p[block].FF.enable=false;
            int pathEnable=blockDialog.enableValue->text().toInt();
            m_blocks_p[block].pathEnable=pathEnable;
            m_paths_p[pathEnable].blockEnd=block;
            }
        if(m_blocks_p[block].FF.type== (char)FF::FSM)
            {
            Files file;
            if(m_blocks_p[block].TT.fileName==0) m_blocks_p[block].TT.fileName=new char[200];
            QlineEditToChar(blockDialog.TTfileValue, m_blocks_p[block].TT.fileName); 
            file.fileToTrueTable(&m_blocks_p[block].TT, true);
            }
        }        
    else if(blockType== (char)Type::NODE || blockType== (char)Type::TAB)
            {
            int width, maxWidth=0;
            m_blocks_p[block].enable=true; //bloque activo
            m_blocks_p[block].nPathIn=nPathsIn;
            m_blocks_p[block].nPathOut=nPathsOut;
            for(int i=0; i<nPathsIn; i++)
                {
                m_blocks_p[block].nodePathIn[i].path=nodePathIn[i].path;
                m_blocks_p[block].nodePathIn[i].bitIni=nodePathIn[i].bitIni;
                m_blocks_p[block].nodePathIn[i].bitEnd=nodePathIn[i].bitEnd;
                if(blockType== (char)Type::NODE)
                    m_paths_p[nodePathIn[i].path].blockEnd=block;
                width=nodePathIn[i].bitIni-nodePathIn[i].bitEnd+1;
                if(width>maxWidth) maxWidth=width;
                }
            for(int i=0; i<nPathsOut; i++)
                {
                m_blocks_p[block].nodePathOut[i].path=nodePathOut[i].path;
                m_blocks_p[block].nodePathOut[i].bitIni=nodePathOut[i].bitIni;
                m_blocks_p[block].nodePathOut[i].bitEnd=nodePathOut[i].bitEnd;
                m_paths_p[nodePathOut[i].path].blockIni=block;
                width=nodePathOut[i].bitIni-nodePathOut[i].bitEnd+1;
                if(width>maxWidth) maxWidth=width;
                }
            m_blocks_p[block].nBits=maxWidth; //anchura del mayor camino
            }
            
    //si es tipo IN_PORT, se deben activar sus caminos de salida, inmediatamente
    else if(blockType== (char)Type::IN_PORT)
        {
        m_blocks_p[block].nPathOut=nPathsOut;
        for(int i=0; i<nPathsOut; i++)  //copia de caminos de salida
            {
            m_blocks_p[block].pathOut[i]=pathsOut[i];
            m_paths_p[pathsOut[i]].blockIni=block;
            }
        m_blocks_p[block].initPortValue=value;   //valor de inicialización
        m_blocks_p[block].initPortValueUnknow=unKnow;   //valor de inicialización
        
        int pathOut=m_blocks_p[block].pathOut[0];   //dado que todas sus salidas son iguales, solo se analiza una
        int valueOld=m_paths_p[pathOut].valueIni.value;
        bool unKnowOld=m_paths_p[pathOut].valueIni.unKnow;
        int valueNew=m_blocks_p[block].initPortValue;
        if(unKnow!=unKnowOld || valueOld!=valueNew)                      //ha habido cambios, se actualiza la info de los caminos de salida
            {
            for(int p=0; p<m_blocks_p[block].nPathOut; p++)
                m_cBlocks_p->startPath(m_blocks_p[block].pathOut[p], valueNew, unKnow);            
            }
        m_blocks_p[block].nPathIn=0; //sin caminos de entrada
        }
    else if(blockType== (char)Type::OUT_PORT)
        {
        m_blocks_p[block].nPathOut=0; //sin caminos de salida
        m_blocks_p[block].nPathIn=nPathsIn;
        for(int i=0; i<nPathsIn; i++)   //copia de caminos de entrada
            {
            m_blocks_p[block].pathIn[i]=pathsIn[i];                    
            m_paths_p[pathsIn[i]].blockEnd=block;
            }
        }
    
    //para todos los bloques
    m_blocks_p[block].enable=true; //bloque activo
                
    //control de errores en los tamaños de los caminos que entran o salen del bloque 
    int nBits=m_cBlocks_p->getBlockBits(block);  
    if(nBits==-1) 
    {
        sprintf(txt, "Warning: the bits in block %d are wrong", block);
        QMessageBox::warning(0, "Paths", txt, QMessageBox::Cancel);
    }
    m_blocks_p[block].nBits=nBits;
        
    //Conversión de segundos a tics y copia
//    m_blocks_p[block].delay=(1000.0*delay/m_params_p->deltaT_blocks);
    m_blocks_p[block].delay=1000.0*delay; //a s->ms
    if(delay<=0) 
        m_blocks_p[block].delay=1;
//    m_blocks_p[block].currentDelay=delay/m_blocks_p[block].delay;
    m_blocks_p[block].currentDelay=m_blocks_p[block].delay/m_params_p->deltaT_blocks;
            
    //visualización de los posibles cambios
    QPointF point=m_cBlocks_p->drawRectBlock(block); 
    if(m_blocks_p[block].type== (char)Type::TAB)
        {
        int path=0;   
        if(m_blocks_p[block].nPathIn==1)
            path=m_blocks_p[block].nodePathIn[0].path;
        else if(m_blocks_p[block].nPathOut==1)
            path=m_blocks_p[block].nodePathOut[0].path;
        m_cBlocks_p->setFork(point, path, block);            
        }

    //Sobre la visualización de rectángulos sobre los bloques y sus nombres
     
    //los tipos de bloque de puertos de entrada y salida siempre visualizan el rectágulo
    if(blockType== (char)Type::IN_PORT || blockType== (char)Type::OUT_PORT)
        m_blocks_p[block].recItem->setVisible(true);   
    else //resto de bloques
        m_blocks_p[block].recItem->setVisible(m_params_p->viewRectBlocks);   

    if(blockType!= (char)Type::NODE && blockType!= (char)Type::TAB && (m_params_p->viewRectBlocks ||
        blockType== (char)Type::IN_PORT || blockType== (char)Type::OUT_PORT))
        {
        m_cBlocks_p->drawRectBlock(block); //dibuja un rectangulo sobre el bloque
        }

    if  ((m_params_p->viewNodeBlocksName && (blockType== (char)Type::NODE || blockType== (char)Type::TAB)) || //visualizar el nombre de bloque NODE o
        (m_params_p->viewPortBlocksName &&          //visualizar el nombre del puerto, si se trata de un puerto
        (blockType== (char)Type::IN_PORT || blockType== (char)Type::OUT_PORT)))
            m_cBlocks_p->drawBlockName(block);                       //presenta nombre de bloques
            
    else if(m_params_p->viewOthersBlocksName &&     //resto de casos: 
        blockType!= (char)Type::NODE &&             //bloque no es NODE
        blockType!= (char)Type::TAB &&              //bloque no es TAB
        blockType!= (char)Type::IN_PORT &&          //bloque no es IN_PORT
        blockType!= (char)Type::OUT_PORT)           //bloque no es OUT_PORT        
        m_cBlocks_p->drawBlockName(block);                       //presenta nombre de bloques        
 }

//Convierte de QlineEdit a array de char
void Dialogs::QlineEditToChar(QLineEdit* Qline, char *str)
  {
  QString Qstr;
  Qstr.resize(200);
  QByteArray ba;
  ba.resize(200);
  Qstr=Qline->text();
  ba = Qstr.toLatin1();
  strcpy(str, ba.data()); 
  }
