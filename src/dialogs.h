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

#ifndef DIALOGS_H
#define DIALOGS_H

#include "types.h"
#include "paths.h"
#include "blocks.h"
#include "pathDialog.h"
#include "blockDialog.h"
#include <QMessageBox>

class Dialogs
{
public:  
    //Constructor
    Dialogs(class Blocks *blocks_p, PARAMS *params_p);

    //slot: Recibe control al clicar sobre el botón izquierdo del ratón
    //se usa para generar nuevos caminos
    //toma nota de los puntos marcados en el camino 'left clik', y dibuja los segmentos
    //m_gItem -> mantiene la referencia al item superior situado bajo el ratón
    void bottonLeftAttn();  

    //mueve el texto cuando se arrastra el mouse
    void dragMouseAttn();

    //Recibe control al clicar sobre el botón derecho del ratón
    //Gestiona la captura de datos para conformar los caminos y los bloques
    void bottonRightAttn();  

    //se captura información sobre los caminos y se actualiza en la base de datos
    void pathsAttn();

    //se presenta información del bloque, permitiendo su edición
    //el bloque se identifica a partir del click derecho en el ratón
    void editBlock(int block);

    //Convierte de QlineEdit a array de char
    void QlineEditToChar(QLineEdit* Qline, char *str);
    
      
    QGraphicsItem   * m_gItem;          //item global
    BLOCK           *m_blocks_p;        //puntero a estructura BLOCK
    PATH            *m_paths_p;         //puntero a estructura PATH
    class Blocks    *m_cBlocks_p;       //puntero a clase Block
    bool            m_openedPath,       //flag=true iniciada  la creación de un camino
                    m_closedPath,       //flag=true terminada la creación de un camino
                    m_pathMode,         //modo caminos activado
                    m_blockMode;        //modo bloques activado
       PARAMS       *m_params_p;        //puntero a structura PARAMS
};

#endif
