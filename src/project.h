/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 *
 *   guarda a un fichero el proyecto actual
 *   carga un proyecto desde un fichero *.prj
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

#ifndef PROJECT_FILES_H
#define PROJECT_FILES_H

//#include "types.h"
#include "extractfromfile.h"
#include "blocks.h"
#include "paths.h"
#include "dCir.h"
#include "files.h"
#include "dirDialog.h"
#include <QMessageBox>

class Project
{
public:
    //Constructor
    //requiere un puntero a clase Gui con la que interactua
    Project(class Gui *gui_p);
    
    //Destructor
    //Elimina la memoria reservada
    ~Project();
    
    //guarda al fichero m_gui_p->m_projectName el proyecto actual
    int saveProject();
    
    //carga desde el fichero m_gui_p->m_projectName el proyecto actual
    int loadProject();
    
    //interpreta cada linea de texto
    int readOrder(char *line_p);
    

    class Gui       *m_gui_p;                       //puntero a clase Gui   
    class Blocks    *m_cBlocks_p;                   //puntero a clase Blocks
    BLOCK           *m_blocks_p;                    //puntero a array de estructuras BLOCKs
    PATH            *m_paths_p;                     //puntero a array de estructuras PATHs
    QPointF         *m_allPathsPoints_p[MAX_PATHS]; //array de punteros para alojar temporalmente los puntos del camino
    int             m_nAllPathsPoints[MAX_PATHS],   //#puntos en cada camino
                    m_nPathBits[MAX_PATHS],
                    m_pathBlockIni[MAX_PATHS],
                    m_pathBlockEnd[MAX_PATHS];
};

#endif
