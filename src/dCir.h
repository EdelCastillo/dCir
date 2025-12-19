/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 *
 *   Ventana principal
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

#ifndef DCIR_BASE_H
#define DCIR_BASE_H
 
#include <QtWidgets>
#include <QMainWindow>
#include <QApplication>
#include <QPushButton>
#include <QTimer>
#include "pathDialog.h"
#include "blockDialog.h"
#include "common.h"
#include "types.h"
#include "project.h"
#include "newProjectDialog.h"
#include "fileNameDialog.h"
#include "image.h"
#include "paths.h"
#include "blocks.h"
#include "common.h"
#include "addrDialog.h"
#include "dialogs.h"


//Esta clase, llamada desde main(), debe llamarse Gui. Otros nombres fallan
class Gui : public QMainWindow
    {
        Q_OBJECT
             
private slots:
      void quit();              //termina la aplicación
      int  openProject();       //abre el proyecto desde fichero
      int  saveProject();       //guarda el proyecto en un fichero
      void closeProject();      //cierra el proyecto actual
      void bottonLeftAttn();    //atiende a la pulsación del botón izquierdo del ratón
      void bottonRightAttn();   //atiende a la pulsación del botón derecho del ratón
      void startSimulation();   //inicio de simulación
      void pauseSimulation();   //pausa la simulación
      void clockAttn();         //atención al botón de clock para los biestables
      void dragMouseAttn();     //atiende el arrastre del ratón
      void newProject();        //construir un nuevo proyecto
      void setParams();         //establece nuevos parámetros
      void ASM();               //asembler
      void viewMemory();        //presenta memoria
      void author();            //info de autoría
      void version();           //info de versión
      void manual();            //manual de usuario
      void newDeltaT_blocks();  //recibe control del timer a intervalos deltaT_blocks
      void pathMode();          //agiliza el acceso a la creación y edición de caminos
      void blockMode();         //agiliza el acceso a la creación y edición de bloques


public:
    Gui(int argc, char **argv); //Constructor
    virtual ~Gui();
    //destructor
private:    
    //fin de operación de la app. proviene desde cierre de ventana
    void closeEvent (QCloseEvent *event);

    //se establece el tiempo de operación de avance de las flechas
    void setDelta_T(QPolygonF *arrow);
    
    //se captura información sobre los caminos y se actualiza en la base de datos
    void pathsAttn();
    
    //Establece un valor por defecto en los parámetros del proyecto
    void initParams();
    
    //Retorna la longitud de la flecha pasada
    int getArrowSize(QPolygonF *arrow);
    
    //Convierte de QlineEdit a array de char
    void QlineEditToChar(QLineEdit* Qline, char *str);
    
    //void viewMemoryData();
    void memoryList(int addrInit, int addrEnd);
    bool isAllOK();



    QGraphicsItem* m_gItem;  //item global
    
    //Ventana de menús
    QPlainTextEdit 
            *m_textEdit;    //texto sobre ventana de menús
    QMenu 	*projectMenu,
            *toolsMenu,
            *aboutMenu;
    QAction 	
            *actionLoadProject,
            *actionSaveProject,
            *actionNewProject,
            *actionCloseProject,
            *actionQuit,
            *actionStart,
            *actionPause,
            *actionNext,
            *actionParams,
            *actionASM,
            *actionAuthor,
            *actionVersion,
            *actionMemory,
            *actionMan,
            *actionPathMode,
            *actionBlockMode;
     QToolBar 	
            *toolsBar;	
  
     QTimer     *m_timer;           //timer 
     QPolygonF  m_arrow[5],         //definición de tres flechas completas
                m_mArrow[5],        //definición de tres medias flechas 
                m_mArrowL[5],       //definición de tres medias flechas izquierda
                m_flecha,           //flecha completa utilizada
                m_mFlecha,          //media flecha utilizada
                m_mFlechaL;
        bool    m_startSimulation,  //flag=true comienza la simulación
                m_pauseSimulation;  //flag=true se detiene la simulación
        bool    m_openedPath,       //flag=true iniciada  la creación de un camino
                m_closedPath;       //flag=true terminada la creación de un camino
         bool    m_timerEvent;       //flag=true indica que se procesa la atención al timer
        int     m_timerCount;       //veces que debe ejecutarse la atención a los bloques antes de ejecutar los caminos 

        bool    m_activeProject,    //true si existe un proyecto en memoria
                m_pathMode,         //true si mode paths
                m_blockMode;        //true si mode blocks

    public:
    char    m_projectDir[200];  //directorio del proyecto      
    char    m_graphicsFile[200];    //fichero gráfico con el esquema del circuito digital
    char    m_projectName[200];     //nombre del proyecto *.prj
    PARAMS  m_params;               //parámetros del proyecto
    class   Image  *m_cImage_p;     //clase para visualización gráfica
    class   Blocks *m_cBlocks_p;    //clase para el tratamiento de la información sobre el circuito digital
    bool    m_viewMemory;           //visualización de contenido del bloque memory
    int     m_addrMemoryInit,       //dirección inicial
            m_addrMemoryEnd;        //dirección final
    Dialogs     *m_dialogs_p;       //clase para dialogos en caminos y bloques
    };

#endif
 
 
