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

#include "dCir.h"
#include <stdio.h>
#include <stdlib.h>

#include "moc_dCir.cpp"

class Gui *extGui_p;  //usada por mouse.cpp para lanzar eventos a Gui

#define MAX_IMAGES 100

Gui::Gui(int argc, char **argv) 
{ 
    extGui_p=this;
    m_activeProject = false;

    //menú principal
    projectMenu = menuBar()->addMenu(tr("&Project"));
//    projectMenu = new QMenu();
//    projectMenu->addMenu(tr("&Project"));
    
    actionLoadProject = new QAction("&open", this);
    connect(actionLoadProject,SIGNAL(triggered()), this, SLOT(openProject()));
    projectMenu->addAction(actionLoadProject);
    
    actionSaveProject = new QAction("&save", this);
    connect(actionSaveProject,SIGNAL(triggered()), this, SLOT(saveProject()));
    projectMenu->addAction(actionSaveProject);
    
    actionNewProject = new QAction("&new", this);
    connect(actionNewProject,SIGNAL(triggered()), this, SLOT(newProject()));
    projectMenu->addAction(actionNewProject);
    
    actionCloseProject = new QAction("&close", this);
    connect(actionCloseProject,SIGNAL(triggered()), this, SLOT(closeProject()));
    projectMenu->addAction(actionCloseProject);
    
    actionQuit = new QAction("&quit", this);
    connect(actionQuit,SIGNAL(triggered()), this, SLOT(quit()));
    projectMenu->addAction(actionQuit);
    
        
    toolsBar = addToolBar(tr("Action"));    
    actionStart = new QAction("&start", this);
    connect(actionStart,SIGNAL(triggered()), this, SLOT(startSimulation()));
    toolsBar->addAction(actionStart);
    actionStart->setToolTip(tr("start the simulation"));

    actionPause = new QAction("&ready", this);
    connect(actionPause,SIGNAL(triggered()), this, SLOT(pauseSimulation()));
    toolsBar->addAction(actionPause);
    actionPause->setToolTip("stops/resume the simulation");

    actionNext = new QAction("&clock", this);
    connect(actionNext,SIGNAL(triggered()), this, SLOT(clockAttn()));
    toolsBar->addAction(actionNext);
    actionNext->setToolTip(tr("All flip-flops update their contents"));

    actionPathMode = new QAction("&path_mode", this);
    connect(actionPathMode, SIGNAL(triggered()), this, SLOT(pathMode()));
    toolsBar->addAction(actionPathMode);
    actionPathMode->setToolTip(tr("speeds up the creation and editing of paths"));

    actionBlockMode = new QAction("&block_mode", this);
    connect(actionBlockMode, SIGNAL(triggered()), this, SLOT(blockMode()));
    toolsBar->addAction(actionBlockMode);
    actionBlockMode->setToolTip(tr("speeds up the creation and editing of blocks"));

    actionQuit = new QAction("&quit", this);
    connect(actionQuit,SIGNAL(triggered()), this, SLOT(quit()));
    toolsBar->addAction(actionQuit);
    actionQuit->setToolTip(tr("leave the application"));

    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    
    actionParams = new QAction("&configuration", this);
    connect(actionParams,SIGNAL(triggered()), this, SLOT(setParams()));
    toolsMenu->addAction(actionParams);
    
    actionASM = new QAction("&assembler", this);
    connect(actionASM,SIGNAL(triggered()), this, SLOT(ASM()));
    toolsMenu->addAction(actionASM);
    
    actionMemory = new QAction("&viewMemory", this);
    connect(actionMemory,SIGNAL(triggered()), this, SLOT(viewMemory()));
    toolsMenu->addAction(actionMemory);
    
    aboutMenu = menuBar()->addMenu(tr("&About"));
    actionMan = new QAction("&userManual", this);
    connect(actionMan,SIGNAL(triggered()), this, SLOT(manual()));
    aboutMenu->addAction(actionMan);
    
    actionVersion = new QAction("&version", this);
    connect(actionVersion,SIGNAL(triggered()), this, SLOT(version()));
    aboutMenu->addAction(actionVersion);
    
    actionAuthor = new QAction("&author", this);
    connect(actionAuthor,SIGNAL(triggered()), this, SLOT(author()));
    aboutMenu->addAction(actionAuthor);

    setWindowTitle(tr("Digital circuit simulator"));
    
    //texto genérico debajo del menú principal
    m_textEdit= 	new QPlainTextEdit;
    setCentralWidget(m_textEdit);
    m_textEdit->appendPlainText("--NO PROJECT--");
        
    m_timer=new QTimer(this); //timer para operación en background
    connect(m_timer, SIGNAL(timeout()), this, SLOT(newDeltaT_blocks())); //función de atención al timer
    
    m_openedPath=false;         //no iniciada la creación de un camino
    m_startSimulation=false;    //simulación no iniciada
    m_pauseSimulation=false;    //pausa no establecia
    
    //se establecen los tres tipos de flechas utilizadas: grande, mediana, pequeña
    //MUY GRANDES
    m_arrow[0] << QPointF(-10, -5) << QPointF(5, -5) << QPointF(5, -10) << QPointF(10, 0)
         << QPointF(5, 10) << QPointF(5, 5) << QPointF(-10, 5) ;
    //GRANDES 
    m_arrow[1] << QPointF(-10, -2) << QPointF(5, -2) << QPointF(5, -5) << QPointF(10, 0)
         << QPointF(5, 5) << QPointF(5, 2) << QPointF(-10, 2) ;
    //MEDIANAS
    m_arrow[2] << QPointF(-5, -2) << QPointF(2, -2) << QPointF(2, -5) << QPointF(5, 0)
         << QPointF(2, 5) << QPointF(2, 2) << QPointF(-5, 2) ;
    //PEQUEÑAS
    m_arrow[3] << QPointF(-3, -2) << QPointF(0, -2) << QPointF(0, -5) << QPointF(3, 0)
        << QPointF(0, 5) << QPointF(0, 2) << QPointF(-3, 2);
    //MUY PEQUEÑAS
    m_arrow[4] << QPointF(-3, -1) << QPointF(0, -1) << QPointF(0, -3) << QPointF(2, 0) << QPointF(0, 3)
        << QPointF(0, 1) << QPointF(-3, 1);

    //medias flechas para los comienzos de los segmentos  
    //MUY GRANDES
    m_mArrow[0] << QPointF(0, -5) << QPointF(5, -5) << QPointF(5, -10) << QPointF(10, 0)
         << QPointF(5, 10) << QPointF(5, 5) << QPointF(0, 5) ;
    //GARNDES   
    m_mArrow[1] << QPointF(0, -2) << QPointF(5, -2) << QPointF(5, -5) << QPointF(10, 0)
         << QPointF(5, 5) << QPointF(5, 2) << QPointF(0, 2) ;
    //MEDIANAS
    m_mArrow[2] << QPointF(0, -2) << QPointF(2, -2) << QPointF(2, -5) << QPointF(5, 0)
         << QPointF(2, 5) << QPointF(2, 2) << QPointF(0, 2) ;
    //PEQUEÑAS
    m_mArrow[3] << QPointF(-1, -2) << QPointF(0, -2) << QPointF(0, -5) << QPointF(3, 0) << QPointF(0, 5) 
        << QPointF(0, 2) << QPointF(-1, 2);
    //MUY PEQUEÑAS
    m_mArrow[4] << QPointF(-1, -1) << QPointF(0, -1) << QPointF(0, -3) << QPointF(2, 0) << QPointF(0, 3)
        << QPointF(0, 1) << QPointF(-1, 1);

    //Colas de las flechas
    //MUY GRANDES
    m_mArrowL[0] << QPointF(-10, -5) << QPointF(3, -5) << QPointF(3, 5) << QPointF(-10, 5);
    //GRANDES  
    m_mArrowL[1] << QPointF(-10, -2) << QPointF(2, -2) << QPointF(2, 2) << QPointF(-10, 2);
    //MEDIANAS 
    m_mArrowL[2] << QPointF(-5, -2)  << QPointF(1, -2) << QPointF(1, 2) << QPointF(-5, 2);
    //PEQUEÑAS
    m_mArrowL[3] << QPointF(-3, -2) << QPointF(0, -2) << QPointF(0, 2) << QPointF(-3, 2);
    //MUY PEQUEÑAS
    m_mArrowL[4] << QPointF(-3, -1) << QPointF(0, -1) << QPointF(0, 1) << QPointF(-3, 1);

/*
    m_mArrow[0] << QPointF(10, -5) << QPointF(5, -5) << QPointF(5, -10) << QPointF(10, 0)
         << QPointF(5, 10) << QPointF(5, 5) << QPointF(0, 5) << QPointF(0, 10)<< QPointF(10, 10);
    m_mArrow[1] << QPointF(-10, -2) << QPointF(5, -2) << QPointF(5, -5) << QPointF(10, 0)
         << QPointF(5, 5) << QPointF(5, 2) << QPointF(-6, 2) << QPointF(-6, 5) << QPointF(-10, 5) ;
    m_mArrow[2] << QPointF(-5, -2) << QPointF(2, -2) << QPointF(2, -5) << QPointF(5, 0)
         << QPointF(2, 5) << QPointF(2, 2) << QPointF(-1, 2) << QPointF(-1, 5)<< QPointF(-5, 5);
*/         
         
    m_openedPath=false;   //no iniciado la creación de un camino
    
    m_cImage_p=0;       //puntero a clase Image
    m_cBlocks_p=0;      //puntero a clase Blocks
    m_dialogs_p = 0;
    initParams();       //parámetros por defecto
    m_timerEvent=false; //no se ejecuta la función asociada al timer
    m_viewMemory=false;
    m_addrMemoryInit=0;
    m_addrMemoryEnd=0x7F;
    m_pathMode  = false;
    m_blockMode = false;
}

Gui::~Gui()
{
//printf("init destructor of Gui class\n");
    if (m_activeProject)
        closeProject();             //se cierra el proyecto actual
    if(m_timer) delete m_timer; //se destruye el timer
//printf("finish destructor of Gui class\n");
}

//Establece un valor por defecto en los parámetros del proyecto
void Gui::initParams()
{
    m_params.arrowType=3; //flecha mediana
    m_params.arrowColor0=QColor(0, 255, 0); //color verde para el interior de la flecha con valor '0'
    m_params.arrowColor1=QColor(255, 0, 0); //color rojo para el interior de la flecha con valor '1'
    m_params.pathColor =QColor(255, 0, 0);  //color rojo para la linea discontinua de los caminos
    m_params.blockColor=QColor(0, 0, 255);  //color azúl para la linea discontinua de los bloques
    
    m_params.viewPaths=true;            //caminos, visibles
    m_params.viewPathsName=true;        //nombres de caminos, visibles
    m_params.viewIniValuePaths=true;    //valores iniciales de los caminos, visibles
    m_params.viewEndValuePaths=true;    //valores fiales de los caminos, visibles
    m_params.viewNodeBlocksName=true;   //nombres de los bloques tipo nodos, visibles
    m_params.viewPortBlocksName=true;   //nombres de los bloques tipo puertos, visibles
    m_params.viewOthersBlocksName=true; //otros nombres, visibles
    m_params.viewRectBlocks=true;       //rectángulo alrededor del bloque, visible
    
    m_params.graphicFile[0]=0;          //fichero con dibujo de base no incluido
    m_projectDir  [0] = 0;
    m_projectName [0] = 0;
    m_graphicsFile[0] = 0;

    m_flecha=m_arrow[m_params.arrowType];   //selección de flecha
    m_mFlecha=m_mArrow[m_params.arrowType]; //selección de media flecha

    m_params.velocity = 40;               //velocidad relativa
 //   m_params.deltaT_blocks = 1000 - (1000 - 10) * m_params.velocity / 100; //tiempo de evaluación de los bloques (ms)
    m_params.deltaT_blocks = 1000 * exp(-m_params.velocity / 22.0); //exponencial
    setDelta_T(&m_flecha);                  //se establece el tiempo de operación de avance de las flechas

}

//se establece el tiempo de operación de avance de las flechas
void Gui::setDelta_T(QPolygonF *arrow)
 {
    //se igualan los tiempos de path con los de bloques (no tiene que ser así necesariamente)
    m_params.deltaT_paths = m_params.deltaT_blocks; //tiempo de avance de las flechas(ms)
}
 
int Gui::getArrowSize(QPolygonF *arrow)
 {
     int sizeArrow;
    qreal x, y, w, h;
    QRectF rec=arrow->boundingRect();   //tamaño de la flecha 
    rec.getRect(&x, &y , &w, &h);
    w>h? sizeArrow=(int)w:sizeArrow=(int)h;  //mayor dimensión
    sizeArrow*=1.2; //20% más: asegura espacio entre flechas
    return sizeArrow;
 }
 
//fin de operación de la app. Desde menú
void Gui::quit()
{
    if (m_activeProject)
        closeProject();
   qApp->quit();     
}

//Agiliza el acceso a la creación y edición de caminos
void Gui::pathMode()
{
    if(m_dialogs_p==0)
        {
        QMessageBox::warning(this, "dCir", "Warning: the project is not started.", QMessageBox::Close);
        return;
        }

    if (m_pathMode == true) //si antes estaba activo el modo camino
        {
        m_pathMode  = false; //se desactiva el modo caminos
        m_blockMode = false; //se desactiva el modo bloque
        actionPathMode ->setText("path_mode");
        actionBlockMode->setText("block_Mode");
    }
    else //si antes NO estaba activo el modo camino
        {
        m_pathMode  = true;      //modo camino activado
        m_blockMode = false;    //modo bloque desactivado
        actionPathMode ->setText("PATH_MODE");
        actionBlockMode->setText("block_mode");
        }
    m_dialogs_p->m_pathMode = m_pathMode;
    m_dialogs_p->m_blockMode = m_blockMode;
}

//Agiliza el acceso a la creación y edición de bloques
void Gui::blockMode()
{
    if(m_dialogs_p == 0)
        {
        QMessageBox::warning(this, "dCir", "Warning: the project is not started.", QMessageBox::Close);
        return;
        }
    if (m_blockMode == true) //si antes estaba activo el modo bloque
        {
        m_pathMode  = false; //se desactiva el modo caminos
        m_blockMode = false; //se desactiva el modo bloque
        actionPathMode ->setText("path_mode");
        actionBlockMode->setText("block_Mode");
        }
    else //si antes NO estaba activo el modo bloque
        {
        m_blockMode = true;     //modo bloque activado
        m_pathMode  = false;     //modo camino desactivado
        actionPathMode ->setText("path_mode");
        actionBlockMode->setText("BLOCK_MODE");
        }
    m_dialogs_p->m_pathMode = m_pathMode;
    m_dialogs_p->m_blockMode = m_blockMode;
}

//fin de operación de la app. Desde cierre de ventana
void Gui::closeEvent (QCloseEvent *event)
{
/*  
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "...",
                                                                tr("Are you sure?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
      printf("OK\n");
        event->accept();
    }
*/
event->accept();
quit();
}

//información genérica
void Gui::manual()
{
    FILE *fp=0;
    char txt[200];
    char line[400];
    int ret;

    sprintf(txt,"./userManual.txt");
     //si un fichero con ese nombre existe en el directorio actual, se pide confirmación
    if((fp=fopen(txt, "r"))==0)
        {
        QMessageBox::warning(this, "dCir", "Warning: the userManual.txt file could not be opened.\nIt is expected to be in the same directory as the app.",
            QMessageBox::Close);
        return;
        }
    else
        {
        extractFromFile file; //clase para la extracción de la información desde fichero
        m_textEdit->appendPlainText(QString("\n"));
        while(true)
            {
            ret=file.extractNextLine(fp, line, 200); //nueva linea 
            if(ret==-1) break; //fin de fichero
            else if(ret<-1) 
            {
                sprintf(txt, "Warning: some problem reading the file %s.", txt);
                QMessageBox::warning(this, "dCir", txt, QMessageBox::Close);
                return;
            }
            m_textEdit->appendPlainText(QString(line));
            }
        }
    if (fp) fclose(fp);
}

void Gui::viewMemory()
{
    if (!m_activeProject)
    {
        QMessageBox::warning(this, "dCir", "Warning: no project loaded",
            QMessageBox::Close);
        return;
    }

    bool hit = false;
    for (int i = 0; i < MAX_BLOCKS; i++)
        if (m_cBlocks_p->m_blocks_p[i].type == (char)Type::MEMORY)
        {
            hit = true; break;
        }
    if (!hit)
    {
        QMessageBox::warning(this, "dCir", "Warning: there is no memory block",
            QMessageBox::Close);
        return;
    }

    Numeric cnv;
    char txt[50];
    AddrDialog addrDialog;
    
    sprintf(txt, "0x%02X", m_addrMemoryInit);
    addrDialog.addr1Value->setText(txt);
    sprintf(txt, "0x%02X", m_addrMemoryEnd);
    addrDialog.addr2Value->setText(txt);
    addrDialog.exec();
    if(addrDialog.m_accept==false) return;   //cancelado
    
    QlineEditToChar(addrDialog.addr1Value, txt);
    m_addrMemoryInit=cnv.getValueFromText(txt);
    QlineEditToChar(addrDialog.addr2Value, txt);
    m_addrMemoryEnd=cnv.getValueFromText(txt);
    memoryList(m_addrMemoryInit, m_addrMemoryEnd);
}

void Gui::memoryList(int addrInit, int addrEnd)
{
    Numeric cnv;
    bool hit=false;
    unsigned int addr, value;
    int memoryBlock;
    char txt[20];
    
    for(int i=0; i<MAX_BLOCKS; i++)
        if(m_cBlocks_p->m_blocks_p[i].type== (char)Type::MEMORY)
            {hit=true; memoryBlock=i; break;}
    if (!hit)
        return;

    m_viewMemory = true;

    int nLines=0;
    QTextCursor cursor(m_textEdit->document());
    cursor.beginEditBlock();
    cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1); //go down y-times
    int endDoc=cursor.position();    

    cursor.movePosition(QTextCursor::Start); //inicio de documento
    QString str;
    hit=false;
    int cp;
    QTextBlock tb;
    tb = cursor.document()->findBlockByLineNumber(0); //primer bloque de texto
    while(true) //numero de lineas existentes
        {
        cursor.setPosition(tb.position(), QTextCursor::MoveAnchor);
        cp=cursor.position();
        str=tb.text();
        if(str.compare("-MEMORY-", Qt::CaseSensitive)==0) 
            {
                cp+=9; //se añade -MEMORY- + \n
                cursor.setPosition(cp, QTextCursor::MoveAnchor);
                nLines++; hit=true; 
                break;                
            }
        tb=tb.next(); //siguiente bloque de texto
        if(!tb.isValid()) break;
        nLines++;
   
        }
    if(!hit)  //Se inserta etiqueta para reconocer el punto de inserción
        {
        cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor,1); //1 sola vez
        cursor.insertBlock();
        cursor.insertBlock();
        cursor.insertText("-MEMORY-");
        cursor.insertBlock();
        nLines+=3;
        }
    //se borra deste la etiqueta -MEMORY- hasta el final del texto
    cp=cursor.position();
    cursor.setPosition(cp, QTextCursor::MoveAnchor); //posición actual    
    cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor); //fin de documento
    cursor.removeSelectedText(); //se elimina todo el texto de memoria
        cursor.setPosition(cp, QTextCursor::MoveAnchor);
    for(int i=addrInit; i<=addrEnd; i++)    //se rescribe la zona borrada con info actualizada
        {
        cursor.insertBlock();
        addr =m_cBlocks_p->m_blocks_p[memoryBlock].TT.table_p[i][0];
        value=m_cBlocks_p->m_blocks_p[memoryBlock].TT.table_p[i][1];
        cnv.valueToHexa(addr, txt, 16);
        cursor.insertText(txt);
        cursor.movePosition(QTextCursor::EndOfWord);
        cursor.insertText("\t");
        cursor.movePosition(QTextCursor::EndOfLine);
        cnv.valueToHexa(value, txt, 16);
        cursor.insertText(txt);        
        }
    
    cursor.endEditBlock();    
}



//información de versión
void Gui::version()
{
         QMessageBox::information( this, "Version",
                                            "version: 1.5.2, November 2025",
                                            QMessageBox::Close);
}

//información de autor
void Gui::author()
{
         QMessageBox::information( this, "Author",
                                            "Esteban del Castillo Pérez \ne.mail: esteban.delcastillo@urv.cat\nUniversitat Rovira i Virgili",
                                            QMessageBox::Close);
}

//arranca la simulación. Inicializa los caminos existentes
void Gui::startSimulation()
{
    if (!m_activeProject)
    {
        QMessageBox::warning(this, "dCir", "Warning: no project loaded",
            QMessageBox::Close);
        return;
    }
    if(!isAllOK()) return;
    m_startSimulation=true; //flag de comienzo de simulación
    m_pauseSimulation=false; //flag de pausa
    m_pathMode = false;
    m_blockMode = false;
    m_dialogs_p->m_pathMode = m_pathMode;
    m_dialogs_p->m_blockMode = m_blockMode;
    actionPathMode->setText("path_mode");
    actionBlockMode->setText("block_Mode");

    m_timerCount=m_params.deltaT_paths; //se restablece el valor del contador de tics (tiempo de avance de una flecha)
    
    m_cBlocks_p->removeAllPathsArrowItems(); //se eliminan las posibles flechas existentes sobre los caminos
    
    m_cBlocks_p->removeAllPathsValueItems(); //se eliminan todas las referencias a los valores sobre los caminos
    m_cBlocks_p->initAllPathUnknown();      //todos los caminos inicalizados a 'x'
    m_cBlocks_p->initActiveBlocks();        //todos los bloques activos inicializados
    
    m_timer->start(m_params.deltaT_blocks); //inicio del reloj base. Evaluación de los bloques
}

//comuta el estado de pausa luego del inicio de la simulación 
void Gui::pauseSimulation()
{
       if(m_startSimulation==true) 
            {
           if (m_pauseSimulation == false)
                {
                m_pauseSimulation = true;
                actionPause->setText("PAUSED");
                }
           else
                {
                m_pauseSimulation = false;
                actionPause->setText("ready");
                }
            }
}

//FFs reciben clock: se captura la info de entrada y se inicializa el reloj del retardo
void Gui::clockAttn()
{
    if (!m_activeProject)
    {
        QMessageBox::warning(this, "dCir", "Warning: no project loaded",
            QMessageBox::Close);
        return;
    }
    m_cBlocks_p->clockAttn();
        if(m_viewMemory) memoryList(m_addrMemoryInit, m_addrMemoryEnd);
}

//verificación del estado del sistema
bool Gui::isAllOK()
{
    if(!m_cBlocks_p->isAllPathsOK() || !m_cBlocks_p->isAllBlocksOK()) return false;
    return true;
}

//Recibe control a intervalos m_deltaT_blocks
//Se evaluan los bloques y la información sobre los caminos
//opera si la simulación está activada y no en pausa
void Gui::newDeltaT_blocks()
{
    if(m_startSimulation==false || (m_startSimulation==true && m_pauseSimulation==true)) return;
    
    m_timerEvent=true;                      //flag de aviso de operación en background   
    m_cBlocks_p->evaluateBlocks();          //Se evalua cada bloque
    m_timerCount-=m_params.deltaT_blocks;   //los caminos se avalúan más lentamente que los bloques
    if(m_timerCount<=0)                     //expira el tiempo para evaluar los caminos
        {
        m_timerCount=m_params.deltaT_paths;      //se restablece el tiempo
        m_cBlocks_p->evaluatePaths();       //evaluación de caminos
        }
    m_timerEvent=false;
 }
 

//slot: Recibe control al clicar sobre el botón izquierdo del ratón
//se usa para generar nuevos caminos
//toma nota de los puntos marcados en el camino 'left clik', y dibuja los segmentos
//m_gItem        -> mantiene la referencia al item superior situado bajo el ratón
void Gui::bottonLeftAttn()
{
    m_dialogs_p->bottonLeftAttn();
}

//mueve el texto cuando se arrastra el mouse
void Gui::dragMouseAttn()
{
    m_dialogs_p->dragMouseAttn();
}

//Recibe control al clicar sobre el botón derecho del ratón
//Gestiona la captura de datos para conformar los caminos y los bloques
void Gui::bottonRightAttn()
{
    bool tmpPause = m_pauseSimulation; //toma nota del estado actual
    m_pauseSimulation = true; //pausa
    m_dialogs_p->bottonRightAttn();
    m_pauseSimulation = tmpPause; //Se restaura su estado anterior
}

//se captura información sobre los caminos y se actualiza en la base de datos
void Gui::pathsAttn()
{   
    bool tmpPause = m_pauseSimulation; //toma nota del estado actual
    m_pauseSimulation = true; //pausa
    m_dialogs_p->pathsAttn();
    m_pauseSimulation = tmpPause; //Se restaura su estado anterior
}

//Se captura info de configuración
//se establece el tiempo entre llamadas a la actualización de los caminos y bloques (m_deltaT_paths/blocks)
//se establece el tipo de flecha
void Gui::setParams()
{
    if (!m_activeProject)
    {
        QMessageBox::warning(this, "dCir", "Warning: no project loaded",
            QMessageBox::Close);
        return;
    }
    char txt[200];
    bool tmpPause = m_pauseSimulation; //toma nota del estado actual
    m_pauseSimulation = true; //pausa

    ParamsDialog2 paramsDiag;  //diálogo
    
    //presentación de información
    paramsDiag.graphicValue->setText(m_graphicsFile);  //gráfico de background
    
    sprintf(txt, "%d", m_params.velocity); //velocidad de avance de las flechas
    paramsDiag.velocityValue->setText(txt);
/*
    int index=0;
    if(m_params.deltaT_blocks<=10)  index = 3;
    else if (m_params.deltaT_blocks <= 25)  index = 2;
    else if (m_params.deltaT_blocks <= 50)  index = 1;
    paramsDiag.deltaTValue->setCurrentIndex(index); //tiempo para deltaT_blocks
*/
    paramsDiag.arrowType->setCurrentIndex(m_params.arrowType); //tipo de flecha

    //flags de visualización
    paramsDiag.viewPaths->setCheckState(Qt::Unchecked);
    if(m_params.viewPaths)     paramsDiag.viewPaths->setCheckState(Qt::Checked);

    paramsDiag.viewPathsName->setCheckState(Qt::Unchecked);
    if(m_params.viewPathsName)     paramsDiag.viewPathsName->setCheckState(Qt::Checked);

    paramsDiag.viewIniValuePaths->setCheckState(Qt::Unchecked);
    if(m_params.viewIniValuePaths)     paramsDiag.viewIniValuePaths->setCheckState(Qt::Checked);

    paramsDiag.viewEndValuePaths->setCheckState(Qt::Unchecked);
    if(m_params.viewEndValuePaths)     paramsDiag.viewEndValuePaths->setCheckState(Qt::Checked);

    paramsDiag.viewNodeBlocksName->setCheckState(Qt::Unchecked);
    if(m_params.viewNodeBlocksName)     paramsDiag.viewNodeBlocksName->setCheckState(Qt::Checked);
    
    paramsDiag.viewPortBlocksName->setCheckState(Qt::Unchecked);
    if(m_params.viewPortBlocksName)     paramsDiag.viewPortBlocksName->setCheckState(Qt::Checked);
    
    paramsDiag.viewOthersBlocksName->setCheckState(Qt::Unchecked);
    if(m_params.viewOthersBlocksName)     paramsDiag.viewOthersBlocksName->setCheckState(Qt::Checked);
    
    paramsDiag.viewRectBlocks->setCheckState(Qt::Unchecked);
    if(m_params.viewRectBlocks)     paramsDiag.viewRectBlocks->setCheckState(Qt::Checked);
    
    //colores    
    int R, G, B;
    m_params.arrowColor0.getRgb(&R, &G, &B);
    sprintf(txt, "%d, %d, %d", R, G, B);
    paramsDiag.arrowColor0Value->setText(txt);
    
    m_params.arrowColor1.getRgb(&R, &G, &B);
    sprintf(txt, "%d, %d, %d", R, G, B);
    paramsDiag.arrowColor1Value->setText(txt);
    
    m_params.pathColor.getRgb(&R, &G, &B);
    sprintf(txt, "%d, %d, %d", R, G, B);
    paramsDiag.pathColorValue->setText(txt);
    
    m_params.blockColor.getRgb(&R, &G, &B);
    sprintf(txt, "%d, %d, %d", R, G, B);
    paramsDiag.blockColorValue->setText(txt);
        
    paramsDiag.exec();
    if (paramsDiag.m_accept == false)
    {
        m_pauseSimulation = tmpPause; //Se restaura su estado anterior
        return;
    }
    
    //captura de nuevos datos
    while(m_timerEvent); //espera a la conclusión de la acción en segundo plano
    
    QlineEditToChar(paramsDiag.graphicValue, m_graphicsFile);
    strcpy((char*)m_params.graphicFile, m_graphicsFile);
    m_params.velocity=paramsDiag.velocityValue->text().toInt();
    if (m_params.velocity <= 0) m_params.velocity = 1;
    else if (m_params.velocity > 100) m_params.velocity = 100;

    m_params.arrowType=paramsDiag.arrowType->currentIndex();

    //se establece el tiempo entre evaluaciones de bloques (max=1s, min=10 ms)
    //m_params.deltaT_blocks = 1000 - (1000 - 10) * m_params.velocity / 100; //lineal
    m_params.deltaT_blocks = 1000*exp(-m_params.velocity/22.0); //exponencial

    //captura de flags de visualización
    m_params.viewPaths=paramsDiag.viewPaths->isChecked();
    m_params.viewPathsName=paramsDiag.viewPathsName->isChecked();
    m_params.viewIniValuePaths=paramsDiag.viewIniValuePaths->isChecked();
    m_params.viewEndValuePaths=paramsDiag.viewEndValuePaths->isChecked();
    m_params.viewNodeBlocksName=paramsDiag.viewNodeBlocksName->isChecked();
    m_params.viewPortBlocksName=paramsDiag.viewPortBlocksName->isChecked();
    m_params.viewOthersBlocksName=paramsDiag.viewOthersBlocksName->isChecked();
    m_params.viewRectBlocks=paramsDiag.viewRectBlocks->isChecked();
        
    //se hacen o no visibles partes de la información
    m_cBlocks_p->setPathsVisibles(m_params.viewPaths);
    m_cBlocks_p->setPathsNamesVisibles(m_params.viewPathsName);
    m_cBlocks_p->setPathsIniValuesVisibles(m_params.viewIniValuePaths);
    m_cBlocks_p->setPathsEndValuesVisibles(m_params.viewEndValuePaths);
    m_cBlocks_p->setNodeBlocksNameVisibles(m_params.viewNodeBlocksName);
    m_cBlocks_p->setPortBlocksNameVisibles(m_params.viewPortBlocksName);
    m_cBlocks_p->setOthersBlocksNameVisibles(m_params.viewOthersBlocksName);
    m_cBlocks_p->setRectBlocksVisibles(m_params.viewRectBlocks);
     
    //captura de colores
    Common tool;
    int rgb[3];
    tool.QlineEditToChar(paramsDiag.arrowColor0Value, txt);
    tool.strToArray(txt, rgb, 3);
    m_params.arrowColor0.setRgb(rgb[0], rgb[1], rgb[2]);
    
    tool.QlineEditToChar(paramsDiag.arrowColor1Value, txt);
    tool.strToArray(txt, rgb, 3);
    m_params.arrowColor1.setRgb(rgb[0], rgb[1], rgb[2]);
    
    tool.QlineEditToChar(paramsDiag.pathColorValue, txt);
    tool.strToArray(txt, rgb, 3);
    m_params.pathColor.setRgb(rgb[0], rgb[1], rgb[2]);
    
    tool.QlineEditToChar(paramsDiag.blockColorValue, txt);
    tool.strToArray(txt, rgb, 3);
    m_params.blockColor.setRgb(rgb[0], rgb[1], rgb[2]);
    
    //establecimiento de nuevas flechas
    QPolygonF tmpArrow=m_arrow[m_params.arrowType];
    m_mFlecha=m_mArrow[m_params.arrowType];
    m_mFlechaL=m_mArrowL[m_params.arrowType];
    if(tmpArrow!=m_flecha)
        m_cBlocks_p->setArrow(tmpArrow, m_mFlecha, m_mFlechaL);  
    m_flecha=tmpArrow;
    
    
    setDelta_T(&m_flecha);  //establecimiento de nuevos tiempos de evaluación    
    m_timerCount = m_params.deltaT_paths; //se restablece el valor del contador de tics (tiempo de avance de una flecha)
    m_timer->start(m_params.deltaT_blocks); //inicio del reloj base. Evaluación de los bloques
    m_cBlocks_p->setParams(&m_params);  //se pasan los parámetros a la clase Blocks

    //creación y visualización (o no) de la información por partes
    for(int bk=0; bk<MAX_BLOCKS; bk++)
        {
        QPointF point=m_cBlocks_p->drawRectBlock(bk);           //rectángulos alrededor de los bloques
        if(m_cBlocks_p->m_blocks_p[bk].type== (char)Type::TAB)
            {
            int path=m_cBlocks_p->m_blocks_p[bk].pathIn[0];
            m_cBlocks_p->setFork(point, path, bk);            
            }
        }
    m_cBlocks_p->setRectBlocksVisibles(m_params.viewRectBlocks);
    if(m_params.viewRectBlocks) m_cBlocks_p->drawAllRectBlocks();   //se actualizan los bloques
    if(m_params.viewPaths) m_cBlocks_p->drawAllPathsLines();        //se actualizan los caminos
    m_pauseSimulation = tmpPause; //Se restaura su estado anterior
}

//Creación de un nuevo proyecto
void Gui::newProject()
{
    initParams();                           //parámetros por defecto
    NewProjectDialog newProjectDiag(&m_params);  //diálogo 
//    newProjectDiag.CkBlankImage->setCheckState(Qt::Unchecked);
    newProjectDiag.exec();
    if(newProjectDiag.m_accept==false) return;

    char txt[200];
    QlineEditToChar(newProjectDiag.baseDirValue, m_projectDir); //directorio del proyecto
    QlineEditToChar(newProjectDiag.nameValue, txt);             //nombre del proyecto
    if(!(strlen(txt)>4 && strcmp(txt+strlen(txt)-4, ".prj")==0))  //no se da la extensión
        strcat(txt, ".prj");
    sprintf(m_projectName, "%s/%s",m_projectDir, txt);
    
    if(newProjectDiag.CkBlankImage->checkState()== Qt::Checked)
    {
        int imageWidht =newProjectDiag.widthValue ->text().toInt();
        int imageHeight=newProjectDiag.heightValue->text().toInt();
        if(imageHeight<=0) imageHeight=100;
        if(imageWidht <=0) imageWidht =100;
        Image newImage;
        Common tools;
        char txt2[200];
        tools.getFileNameWithoutExtension(txt, txt2); //nombre sin extensión
        sprintf(m_graphicsFile, "%s/%s.png", m_projectDir, txt2);
        newImage.blankImage(imageWidht, imageHeight, m_graphicsFile); //genera el fichero en blanco
    }
    else
        QlineEditToChar(newProjectDiag.graphicValue, m_graphicsFile);//nombre del fichero con el gráfico base
    
    Common tools;
    if(!tools.existFile(m_graphicsFile))
    {
        sprintf(txt, "Warning: the %s file does not exist", m_graphicsFile);
        QMessageBox::warning(this, "Project information", txt, QMessageBox::Close);
        return;
    }
    

    m_timer->stop();                        //se detiene el tiempo
    while(m_timerEvent);                    //espera a la conclusión en background
    
    if(m_cBlocks_p) delete m_cBlocks_p;     //elimina la clase Blocks, si existe
    if(m_cImage_p) delete m_cImage_p;       //elimina la clase Image, si existe
    if(m_dialogs_p) delete m_dialogs_p;           //elimina la clase Image, si existe
    
    m_cImage_p=new Image;                   //crea la clase Image
    m_cBlocks_p=new Blocks(m_cImage_p);     //crea la clase Blocks
    m_dialogs_p=new Dialogs(m_cBlocks_p, &m_params);
    m_dialogs_p->m_pathMode  = false;
    m_dialogs_p->m_blockMode = false;

    if(m_cImage_p->newImage(m_graphicsFile)==-1) //presenta la imagen de fondo
    {
        sprintf(txt, "Error: the %s file does not exist.\nPlease, update the project file.", m_graphicsFile);
        QMessageBox::critical(this, "Project information", txt, QMessageBox::Close);
        return;
    }
    
    m_flecha=m_arrow[m_params.arrowType];   //tipo de flecha
    m_mFlecha=m_mArrow[m_params.arrowType]; //tipo de media flecha
    m_mFlechaL=m_mArrowL[m_params.arrowType]; //tipo de media flecha
    m_cBlocks_p->setArrow(m_flecha, m_mFlecha, m_mFlechaL); //establece los tipos de flecha
    setDelta_T(&m_flecha);                  //establece el tiempo de evaluación de caminos
    
    m_cBlocks_p->setParams(&m_params);      //se pasan los parámetros a la clase Blocks
    m_activeProject = true;                 //proyecto cargado
}

//carga de un proyecto ya existente
int Gui::openProject()
{
    Common tools;
    char txt[200];
    if (strlen(m_projectDir) > 3)  //si se ha inicializado
        strcpy(txt, m_projectDir);
    else
        strcpy(txt, (char*)"\\");
    FileNameDialog fileDiag((char*)"- open project -", txt); //diálogo
    fileDiag.exec();
    if (fileDiag.m_accept == 0) return -1;

    tools.QlineEditToChar(fileDiag.fileValue, m_projectName); //nombre del proyecto
    if(!tools.existFile(m_projectName))
    {
        sprintf(txt, "Warning: the %s file does not exist", m_projectName);
        QMessageBox::warning(this, "Project information", txt, QMessageBox::Close);
        return -1;
    }

    m_timer->stop();                            //se detiene el tiempo
    while (m_timerEvent);                        //espera a la conclusión en background

    if (m_cBlocks_p) delete m_cBlocks_p;         //elimina la clase Blocks, si existe
    if (m_cImage_p)  delete m_cImage_p;          //elimina la clase Image, si existe
    if (m_dialogs_p) delete m_dialogs_p;         //elimina la clase Image, si existe

    m_cImage_p  = new Image;                      //crea la clase Image
    m_cBlocks_p = new Blocks(m_cImage_p);         //crea la clase Blocks
    m_dialogs_p = new Dialogs(m_cBlocks_p, &m_params);
    m_dialogs_p->m_pathMode  = false;
    m_dialogs_p->m_blockMode = false;

    Project project(this);
    if (project.loadProject() == -1) return -1;  //carga del proyecto

    if (!tools.existFile(m_graphicsFile))
    {
        while(true)
        {
            sprintf(txt, "Warning: the %s file does not exist", m_graphicsFile);
            QMessageBox::warning(this, "Project information", txt, QMessageBox::Close);
            FileNameDialog fileDiag((char*)"- open graphics file -", m_projectDir); //diálogo
            fileDiag.exec();
            if (fileDiag.m_accept == 0) return -1;
            tools.QlineEditToChar(fileDiag.fileValue, m_graphicsFile); //nombre del proyecto
            if (!tools.existFile(m_graphicsFile)) continue;
        break;
        }
    }

    if (m_cImage_p->newImage(m_graphicsFile) == -1)//presenta la imagen de fondo
        {
        sprintf(txt, "Warning: the %s file does not exist.\nPlease, update the project file", m_graphicsFile);
        QMessageBox::warning(this, "Project information", txt, QMessageBox::Close);
        return -1;
        }
    QPointF P(0,0);
    for (int path = 0; path < MAX_PATHS; path++)
        for (int i = 0; i < m_cBlocks_p->m_paths_p[path].nSegments; i++)
            {
            m_cBlocks_p->m_paths_p[path].segment_p[i].pIni += P;// m_cImage_p->m_offset;
            m_cBlocks_p->m_paths_p[path].segment_p[i].pEnd += P;// m_cImage_p->m_offset;
            }

        
    m_flecha=m_arrow[m_params.arrowType];       //tipo de flecha
    m_mFlecha=m_mArrow[m_params.arrowType];     //tipo de media flecha
    m_mFlechaL=m_mArrowL[m_params.arrowType];     //tipo de media flecha
    m_cBlocks_p->setArrow(m_flecha, m_mFlecha, m_mFlechaL); //establece los tipos de flecha
    setDelta_T(&m_flecha);                      //establece el tiempo de evaluación de caminos
    
    m_cBlocks_p->setParams(&m_params);          //se pasan los parámetros a la clase Blocks
    
    //creación y visualización (o no) de la información por partes
    for(int bk=0; bk<MAX_BLOCKS; bk++)
        {
        QPointF point=m_cBlocks_p->drawRectBlock(bk);           //rectángulos alrededor de los bloques
        if(m_cBlocks_p->m_blocks_p[bk].type== (char)Type::TAB)
            {
            int path=m_cBlocks_p->m_blocks_p[bk].pathIn[0];
            m_cBlocks_p->setFork(point, path, bk);            
            }
        }
//    m_cBlocks_p->drawAllRectBlocks();           //rectángulos alrededor de los bloques
    m_cBlocks_p->setRectBlocksVisibles(m_params.viewRectBlocks);

    m_cBlocks_p->drawAllNodeBlocksName();       //nombres de bloques tipo nodo
    m_cBlocks_p->setNodeBlocksNameVisibles(m_params.viewNodeBlocksName);

    m_cBlocks_p->drawAllPortsBlocksName();      //nombres de bloques tipo Port
    m_cBlocks_p->setPortBlocksNameVisibles(m_params.viewPortBlocksName);
    
    m_cBlocks_p->drawAllOthersBlocksName();     //nombres de otros tipos de bloques     
    m_cBlocks_p->setOthersBlocksNameVisibles(m_params.viewOthersBlocksName);
    
    m_cBlocks_p->drawAllPathsLines();           //líneas de caminos
    m_cBlocks_p->setPathsVisibles(m_params.viewPaths);
    
    m_cBlocks_p->drawAllPathsNames();           //nombres de caminos
    m_cBlocks_p->setPathsNamesVisibles(m_params.viewPathsName);
                    
    //info para la ventana de menún
  
    m_textEdit->clear(); // unless you know the editor is empty
    m_textEdit->appendPlainText(QString("Simulator for digital circuits"));
    m_textEdit->appendPlainText(QString("------------------------------"));
    sprintf(txt,"Project name: %s", m_projectName);
    m_textEdit->appendPlainText(QString(txt));

    m_activeProject = true;     //proyecto cargado
    return 0;
}

//Se guarda a fichero el proyecto actual
int Gui::saveProject()  
{
    if(!m_activeProject) 
        {
        QMessageBox::warning(this, "dCir", "Warning: no project loaded",
            QMessageBox::Close);
        return 0;
        }

    FileNameDialog fileDiag((char*)"- save project -", m_projectDir); //diálogo
    fileDiag.fileValue->setText(m_projectName); //nombre actual del fichero de proyecto
    
    fileDiag.exec();
    if(fileDiag.m_accept==0) return -1;
    Common tool;
    tool.QlineEditToChar(fileDiag.fileValue, m_projectName); //nuevo nombre de fichero
    
    FILE *fp=0;
     //si un fichero con ese nombre existe en el directorio actual, se pide confirmación
    if((fp=fopen(m_projectName, "r"))!=0)
        {
        char txt[200];
        sprintf(txt, "The file %s exist.\nDo you want to overwrite it?", m_projectName);
        QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Message",
                                            tr(txt),
                                            QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                            QMessageBox::Yes);
        if (fp) fclose(fp);
        if (resBtn != QMessageBox::Yes)
            return 0;
        }
    
    Project project(this);
    if(project.saveProject()==-1) return -1; //se guarda
    
    return 0;
}

//cierre del proyecto actual
void Gui::closeProject()
{
    if (!m_activeProject)
    {
        QMessageBox::warning(this, "dCir", "Warning: no project loaded",
            QMessageBox::Close);
        return;
    }

    m_startSimulation=false;                    //se detiene la simulación
    m_timer->stop();                            //se detiene al reloj
    while(m_timerEvent);                        //espera a la conclusión en background
    if(m_cBlocks_p) {delete m_cBlocks_p; m_cBlocks_p=0;} //se eliminan las clases
    if(m_cImage_p)  {delete m_cImage_p;  m_cImage_p=0;}
}

//llamada al ensamblador
void Gui::ASM()
{
    char fileASM[200];
    FILE *fp=0;
    FileNameDialog fileDiag((char*)"- load ASM file -", m_projectDir); //diálogo
    while(true)
    {
    bool hit=true;
//    fileDiag.fileValue->setText(m_projectName); //nombre actual del fichero de proyecto
    
    fileDiag.exec();
    if(fileDiag.m_accept==0) return;
    Common tool;
    tool.QlineEditToChar(fileDiag.fileValue, fileASM); //nuevo nombre de fichero
    
     //si un fichero con ese nombre no existe o no se puede abrir
    if((fp=fopen(fileASM, "r"))==0)
        {
        char txt[200];
        sprintf(txt, "The file %s does not exist.\nPlease, update the project file", fileASM);
        hit=false;
        }
    if(fp) fclose(fp);
    if(hit) break;
    }
    Files file;
    file.ensambla(fileASM);      
}

//Convierte de QlineEdit a array de char
void Gui::QlineEditToChar(QLineEdit* Qline, char *str)
  {
  QString Qstr;
  Qstr.resize(200);
  QByteArray ba;
  ba.resize(200);
  Qstr=Qline->text();
  ba = Qstr.toLatin1();
  strcpy(str, ba.data()); 
  }

