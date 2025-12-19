/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 *
 *    Tratamiento de caminos entre bloques
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

#include "paths.h"

//constructor: recibe un puntero a la clase Image para poder dibujar sobre la imagen de fondo
Paths::Paths(class Image *image_p)
{
    if(image_p==0) 
    {
        QMessageBox::warning(0, "Paths", "class Image must exist before calling class Paths", QMessageBox::Cancel);
        return;
    }
    m_image_p=image_p;
    m_paths_p=0;    //puntero al array de estructuras PATHS
    m_pathNumber=-1;
    m_params_p=0;   //puntero a la extructura PARAMS
    
    //info temporal para crear los caminos
    m_lineItems_p=0;
    m_lineItems_p=new QGraphicsLineItem*[MAX_POINTS_PATH];//array de items a cada segmento del camino
    m_pathPoints_p=0;
    m_pathPoints_p=new QPointF[MAX_POINTS_PATH];//array de puntos que conforman el camino
    m_lineItemsIndex=0;
    m_pathPointIndex=0;
    m_lineItemsIndex=0;
    m_tmpLine_p = 0;
    
    createAllPaths(); //crea todos los caminos posibles y los inicializa como desactivados
    m_openedPath=false;  //ningún camino abierto
}

//destructor
Paths::~Paths()
{
//printf("init destructor Paths class\n");
    //info temporal de caminos
    if(m_pathPoints_p) {delete [] m_pathPoints_p; m_pathPoints_p=0;}
    if(m_lineItems_p)  {delete [] m_lineItems_p;  m_lineItems_p=0;}
    //se eliminan todos los caminos incluida su memoria reservada
    removeAllPath();
//printf("finist destructor Paths class\n");
}

//Retorna un entero con nBits a '1' en su parte baja
int Paths::andMask(char nBits)
{
    int mask=1, ret=0;
    for(int i=0; i<nBits; i++) ret|=mask<<i;
    return ret;        
}

//inicializa un caminos, si está activo, con un valor dado y bloquea su flujo de información
int Paths::initPath(int path, int value, bool unKnow)
{
    if(!existPath(path)) return -1;
    int mask=1;
    mask=andMask(m_paths_p[path].nBits);
    value&=mask;
    m_paths_p[path].currentSeg=0;
    m_paths_p[path].valueIni.value=value;  
    m_paths_p[path].valueIni.unKnow=unKnow;  
    m_paths_p[path].endPath=false;  
    m_paths_p[path].start=false;
    for(int s=0; s<m_paths_p[path].nSegments; s++) //Para cada segmento
        m_paths_p[path].segment_p[s].currentDeltaSeg=0;        
    return 0;
}

//inicializa los caminos activos con valor de inicio y fin = 'x'
void Paths::initAllPathUnknown()
{
    for(int path=0; path<MAX_PATHS; path++)
        {
        if(!existPath(path)) continue;
        m_paths_p[path].currentSeg=0;
        m_paths_p[path].valueIni.unKnow=true;  
        m_paths_p[path].valueEnd.unKnow=true;  
        m_paths_p[path].endPath=true;  
        m_paths_p[path].start=true;
        for(int s=0; s<m_paths_p[path].nSegments; s++) //Para cada segmento
            {
            m_paths_p[path].segment_p[s].currentDeltaSeg=0;
            for(int i=0; i<m_paths_p[path].segment_p[s].nForks; i++)
                m_paths_p[path].segment_p[s].forks[i].pass=false;
            }
        }    
}

//inicializa todos los caminos activos a un valor dado
int Paths::initAllPath(int value, bool unKnow)
{
    for(int p=0; p<MAX_PATHS; p++)
        if(initPath(p, value, unKnow)==-1) return -1;
    return 0;
}

//inicializa un camino, si activo, a un valor dado y permite que fluya la información
int Paths::startPath(int path, int value, bool unKnow)
{
    initPath(path, value, unKnow); //inicialización
    m_paths_p[path].start=true; //arranque
        
    return 0;
}

//crea todos los caminos y los inicializa a 'x'
//reserva memoria si no está ya reservada
void Paths::createAllPaths()
{
    if(m_paths_p==0)
        m_paths_p=new PATH[MAX_PATHS];
    for(int i=0; i<MAX_PATHS; i++)  //se inicializan las rutas
        {
        m_paths_p[i].nBits=1;
        m_paths_p[i].segment_p=0;
        m_paths_p[i].nSegments=0;
        m_paths_p[i].nameItem=0;
        m_paths_p[i].iniItem=0;
        m_paths_p[i].endItem=0;
        m_paths_p[i].start=false;
        m_paths_p[i].endPath=false;  
        m_paths_p[i].valueIni.unKnow=true;  
        m_paths_p[i].valueEnd.unKnow=true; 
        m_paths_p[i].blockIni=-1;
        m_paths_p[i].blockEnd=-1;
        m_pathsNews[i].news=false; //sin novedad
        }
  
}

//copia un puntero a los parámetros
void Paths::setParams(PARAMS *params)
{
    m_params_p=params;
}

//se actualiza la información dependiente de la flecha
//Se espera una estructura de caminos ya creada
//arrow=flecha a usar
//mArrow=media flecha
void Paths::setArrow(QPolygonF arrow, QPolygonF mArrow, QPolygonF mArrowL)
{
    m_arrow=arrow;
    m_mArrow=mArrow;
    m_mArrowL=mArrowL;
    int sizeArrow;
    
    //tamaño de la flecha
    qreal x, y, w, h;
    QRectF rec=m_arrow.boundingRect();
    rec.getRect(&x, &y , &w, &h);       //rectángulo que circunscribe a la flecha
    
//    w>h? sizeArrow=(int)w:sizeArrow=(int)h;
    sizeArrow = (int)w;
    sizeArrow*=1.2; //20% más: asegura espacio entre flechas
    if ((sizeArrow - (int)w) <= 2) sizeArrow = (int)w + 2;
    SEGMENT *segment_p;
    QPointF segment, deltaSeg;
    int nDeltaSeg;
    double angle;
    for(int path=0; path<MAX_PATHS; path++)
        {
        //para cada segmento del camino: un segmento está deliminato por dos puntos consecutivos
        for(int seg=0; seg<m_paths_p[path].nSegments; seg++)
            {
            segment_p= m_paths_p[path].segment_p+seg;   //puntero a cada uno de los segmentos del camino
            //se determina el ángulo del segmento, en grados
            segment=segment_p->pEnd - segment_p->pIni;
            if(segment.x()==0)
                {
                if(segment.y()>0)
                    angle=90;
                else
                    angle=-90;
                }
            else
                angle=360.0/(2*M_PI)*atan(segment.y()/segment.x());
            if(segment.x()<0) angle+=180;
            
            //se determina su longitud para definir la cantidad de flechas que caben y el incremento en su posición
            double sizeSeg=(segment.x()*segment.x()+segment.y()*segment.y()); //hipotenusa
            if(sizeSeg>0) sizeSeg=sqrt(sizeSeg);
            else sizeSeg=0;
            nDeltaSeg=(int)(sizeSeg/sizeArrow); //número de flechas que pueden dibujarse en el camino
            if(nDeltaSeg==0)
                {deltaSeg=QPointF(0,0); nDeltaSeg=1;}
            else
                deltaSeg=segment/nDeltaSeg;  //ajuste
                    
            segment_p->angle=angle;
            
            //puede que no se requiera eliminar las flechas existentes
            //permite simultanear dos tipos de flecha a la vez si ambas tienen la misma longitud
            if(segment_p->arrowItem==0 || segment_p->deltaSeg!=deltaSeg) 
                {         
                removeAllPathsArrowItems(); //se elimina la info anterior de flechas
                nDeltaSeg++;  //requerido para que operen los nodos de las esquinas   
                segment_p->nDeltaSeg=nDeltaSeg; //debe estar después de removeAllPathsArrowItems();

                if(segment_p->arrowItem) //si existen flechas de antes, se eliminan
                    {delete []segment_p->arrowItem; segment_p->arrowItem=0;}
                if(nDeltaSeg>0) 
                    {
                    segment_p->arrowItem=new QGraphicsPolygonItem *[nDeltaSeg]; //memoria para el item de cada flecha                
                    for(int i=0; i<nDeltaSeg; i++)
                        segment_p->arrowItem[i]=0; //se inicializa
                    }            
                segment_p->deltaSeg=deltaSeg; //incremento posicional de cada flecha respecto a la anterior
                }

            }
        }
}

//Crea o actualiza la estructura FORK en un segmento de path a partir de point
//Se busca en todos los segmentos de path si el bloque tab pasado ya existe, sino, se determina el segmento
//más próximo a point y se crea ahí indicando la cantidad de flechas que caben desde
//el inicio del segmento y hasta llegar al nodo
//requiere de la existencia de la información sobre la flecha
//retorna -1 si no existe el tab y no se puede crear o no existe el camino
int Paths::setFork(QPointF point, int path, int tab)
{
    int seg=0, fork=0, nArrows=0;
    bool hit=false, newFork;
    float minDistance = 1e10;
    if (existPath(path))
    {
        for (int s = 0; s < m_paths_p[path].nSegments; s++) //para todos los segmentos
            if (m_paths_p[path].segment_p[s].nForks > 0) //si existe algún fork previo
            {
                for (int f = 0; f < m_paths_p[path].segment_p[s].nForks; f++)  //para todos los fork existentes              
                    if (m_paths_p[path].segment_p[s].forks[f].tab == tab)  //si este segmento ya contiene al nodo pasado
                    {
                        newFork = false; hit = true; seg = s; fork = f; break;
                    }
            }
        if (!hit)  //nuevo nodo
        {
            //se determina el segmento más próximo al punto
            for (int s = 0; s < m_paths_p[path].nSegments; s++)
            {
                QPointF pIni = m_paths_p[path].segment_p[s].pIni;
                QPointF pEnd = m_paths_p[path].segment_p[s].pEnd;
                float dist;                    //distancia punto a punto
                QPointF newPoint = pIni;
                for (int i = 0; i < m_paths_p[path].segment_p[s].nDeltaSeg; i++)
                {
                    dist = distance(newPoint, point);
                    if (dist < minDistance) { minDistance = dist; seg = s; nArrows = i; }
                    newPoint += m_paths_p[path].segment_p[s].deltaSeg;
                }
            }
            int fork = m_paths_p[path].segment_p[seg].nForks;
            if (seg == m_paths_p[path].nSegments - 1 && nArrows == m_paths_p[path].segment_p[seg].nDeltaSeg - 1)
                nArrows--;
            if (nArrows < 0) nArrows = 0;
            m_paths_p[path].segment_p[seg].forks[fork].pos = nArrows;
            m_paths_p[path].segment_p[seg].forks[fork].tab = tab;
            m_paths_p[path].segment_p[seg].forks[fork].pass = false;
            m_paths_p[path].segment_p[seg].nForks++;
        }
    }
    else return -1;
return 0;        
}
        
//Recibe control del timer cada m_params.delta_T (ms) con resolución m_params.deltaT_blocks
//Se actualizan las flechas sobre todos los caminos activos y se visualiza el valor de la información
void Paths::evaluatePaths()
{
    char txt[20];
    double angle;
    int currentDeltaSeg;
    QPointF deltaSeg;
    QPointF pIni;
    
    for(int path=0; path<MAX_PATHS; path++) //para todos los caminos posibles
        {
        PATH *path_p=m_paths_p+path;                //puntero a la estructrua PATH actual
        if(m_paths_p==0) continue;                  //no existe ningún camino
        if(path_p->segment_p==0) continue;          //sin segmentos en el camino
        if(path_p->start==false) continue;          //si camino no iniciado
        int currentSeg=path_p->currentSeg;          //segmento en curso
        if(currentSeg>=path_p->nSegments) continue; //si se llegó al final del camino
        bool hit;
    
        //para escribir menos
        angle=path_p->segment_p[currentSeg].angle;
        currentDeltaSeg=path_p->segment_p[currentSeg].currentDeltaSeg;
        deltaSeg=path_p->segment_p[currentSeg].deltaSeg;
        pIni=path_p->segment_p[currentSeg].pIni;
        
        QPolygonF arrow;
        char color;
        //para cada deltaSeg se dibuja una nueva flecha, salvo la primera, que se omite para no invadir al bloque
        if(path_p->segment_p[currentSeg].arrowItem && path_p->segment_p[currentSeg].nDeltaSeg>0 && !(currentSeg==0 && currentDeltaSeg==0))
            {
            QGraphicsPolygonItem *arrowItem1, *arrowItem2;
            int nextDelta, nextSegment;
        
            //si ya existen las flechas en el camino, se borra la siguiente y se actualiza la actual
            //eso hace el efecto de avance, cuando la información no ha cambiado.
            //Se requiere determinar la siguiente flecha a borrar
            hit=true;
            if(currentDeltaSeg+1<path_p->segment_p[currentSeg].nDeltaSeg)   //en el propio segmento 
                {nextSegment=currentSeg; nextDelta=currentDeltaSeg+1;}
            else if(currentSeg+1<path_p->nSegments)                         //en el siguiente segmento
                {nextSegment=currentSeg+1; nextDelta=0;}
            else hit=false; //no existe ninguna flecha a borrar
            
            if(hit)
                {
                arrowItem1=arrowItem2=0;
                arrowItem1=path_p->segment_p[currentSeg].arrowItem[currentDeltaSeg]; //item a la flecha
                arrowItem2=path_p->segment_p[nextSegment].arrowItem[nextDelta]; //item a la flecha
                if(arrowItem2) //si existe la flecha se elimina
                    {m_image_p->removePolygonItem(arrowItem2); path_p->segment_p[nextSegment].arrowItem[nextDelta]=0;}
                if(arrowItem1) //si existe la flecha se elimina
                    {m_image_p->removePolygonItem(arrowItem1); path_p->segment_p[currentSeg].arrowItem[currentDeltaSeg]=0;}
                    
                //si es el comienzo de un segmento, salvo el primero, se dibuja media flecha
                if(currentSeg>=0 && path_p->segment_p[currentSeg].currentDeltaSeg==0) arrow=m_mArrow;
                //else if(currentSeg>=0 && path_p->segment_p[currentSeg].currentDeltaSeg+1==path_p->segment_p[currentSeg].nDeltaSeg) arrow=m_mArrowL;
                else    arrow=m_arrow;
                
                //se presenta una nueva flecha con el color adecuado
                //existen tres colores: info=0x00, info!=0x00 y desconocida
                int maxValue=andMask(path_p->nBits);
                if(path_p->valueIni.unKnow==true) color=3;
                else if (path_p->valueIni.value == 0) color = 0;
                else if(path_p->valueIni.value>=maxValue) color=1;
                else color=2;
                if(path_p->segment_p[currentSeg].currentDeltaSeg+1!=path_p->segment_p[currentSeg].nDeltaSeg)
                    {
                    arrowItem1=newPolygon(arrow, angle, pIni+deltaSeg*currentDeltaSeg, color);
                    path_p->segment_p[currentSeg].arrowItem[currentDeltaSeg]=arrowItem1; //se toma nota
                    }
                }
            //se observa si se alcanzó un punto de bifurcación
            for(int i=0; i<path_p->segment_p[currentSeg].nForks; i++)
                {
                if(path_p->segment_p[currentSeg].forks[i].pos==currentDeltaSeg) 
                    {
                    path_p->segment_p[currentSeg].forks[i].pass=true;
                    m_pathsNews[path].news=true;        //novedades
                    m_pathsNews[path].type=2;           //bifurcación
                    m_pathsNews[path].block=path_p->segment_p[currentSeg].forks[i].tab;  //bloque que bifurca
                    }
                }
            }    
            
        //Si estamos al comienzo del camino, se presenta  su valor inicial
        if(currentSeg==0 && path_p->segment_p[currentSeg].currentDeltaSeg==0) 
            {            
            if(m_params_p->viewIniValuePaths) //si se desea visualizar
                {
                int value=path_p->valueIni.value; //valor a visualizar
                if(path_p->valueIni.unKnow==true) 
                    sprintf(txt, "(x)");
                else toText(value, txt, path_p->nBits);
                
                if(path_p->iniItem) //si existe de antes
                    path_p->iniItem->setText(txt); //se actualiza su valor
                else
                    path_p->iniItem=newText(txt, 0, path_p->valueIni.pos); //se crea nuevo item y se visualiza                    
                }
            }         
        //Si estamos al final del camino se presenta su valor
        if(currentSeg==path_p->nSegments-1 && 
           currentDeltaSeg==path_p->segment_p[currentSeg].nDeltaSeg-1) 
            {
            int value=path_p->valueIni.value; //valor a visualizar
            path_p->valueEnd.value=value;  //se igualan los valores de inicio y fin
            path_p->valueEnd.unKnow=path_p->valueIni.unKnow;
            if(m_params_p->viewEndValuePaths) //si se desea visualizar
                {
                if(path_p->valueIni.unKnow==true) 
                    sprintf(txt, "(x)");
                else toText(value, txt, path_p->nBits);
                
                if(path_p->endItem)  //si existe de antes
                    path_p->endItem->setText(txt); //se actualiza su valor
                else
                     path_p->endItem=newText(txt, 0, path_p->valueEnd.pos); //se crea nuevo item y se visualiza      
                }
            path_p->endPath=true;  //la flecha llegó al final del camino. Sirve para alertar a los bloques
            path_p->start=false;   //se detiene el movimiento de la flecha            
            m_pathsNews[path].news=true;
            m_pathsNews[path].type=1;  //fin de camino
            m_pathsNews[path].block=path_p->blockEnd;  //fin de camino
            }
        //se actualiza la posición en el segmento y, si se llegó al final, se actualiza el segmento
        if(++path_p->segment_p[currentSeg].currentDeltaSeg >= path_p->segment_p[currentSeg].nDeltaSeg) 
            ++path_p->currentSeg;
        }
    
}

//retorna true si existe. Un camino existe si contiene segmentos
bool Paths::existPath(int path)
{
    if(path<0 || path>MAX_PATHS) return false;
    if(m_paths_p && m_paths_p[path].segment_p) //existe
        return true;
    return false;
}

//retorna true si el camino ya posee un nombre
bool Paths::existNamePath(int path)
{
    if(path<0 || path>MAX_PATHS) return false;
    if(m_paths_p && m_paths_p[path].nameItem) //existe
        return true;
    return false;
}

//revisa que todos los caminos activos sean correctos
//retorna false si algo no está bien
bool Paths::isAllPathsOK()
{
    char txt[200];
    for(int path=0; path<MAX_PATHS; path++) //para todos los caminos posibles
        {
        if(!existPath(path)) continue;
        if(m_paths_p[path].blockIni==-1 || m_paths_p[path].blockEnd==-1) 
            {
                sprintf(txt, "Warning: the information about the path p(%d) is incomplete.\nIt may not be fully connected to both blocks", path);
                QMessageBox::warning(0, "Paths", txt, QMessageBox::Cancel);
                return false;
            }
        if(m_paths_p[path].nSegments<=0) 
            {
            sprintf(txt, "Warning: path p(%d) has no segments", path);
            QMessageBox::warning(0, "Paths", txt, QMessageBox::Cancel);
            return false;
            }
        if(m_paths_p[path].nBits<=0)
            {
            sprintf(txt, "Warning: the number of bits in path p(%d) is unknown", path);
            QMessageBox::warning(0, "Paths", txt, QMessageBox::Cancel);
            return false;
            }
}
   return true; 
}

//Recibe control por ratón (botón izquierdo)
//acumula puntos e items de cada segmento que va dibujando sobre la marcha
//lo hace en un almacenamiento temporal
void Paths::drawLineSegment()
{
    char txt[200];
    if(m_pathPointIndex==MAX_PATHS) //cantidad máxima de segmentos en un camino
    {
        sprintf(txt, "Warning: max segments in path are %d", MAX_PATHS);
        QMessageBox::warning(0, "Paths", txt, QMessageBox::Cancel);
        return;
    }
    if(m_pathPoints_p==0) //si el array de puntos no existe
    {
        QMessageBox::warning(0, "Paths", "Warning: path points array no initialized\n", QMessageBox::Cancel);
        return;
    }

    //Toma nota de los puntos de botton left clik
    m_pathPoints_p[m_pathPointIndex++]=m_image_p->mouseLeftBottonPoint();

    
    //dibuja linea discontinua y guarda la referencia a su item
    if(m_pathPointIndex>1) //se requiere más de un punto
        {
        QPen pen; 
        pen.setStyle(Qt::SolidLine); pen.setColor(m_params_p->pathColor); 
        if(m_pathBits<3) pen.setWidth(m_pathBits); //anchura ajustada a los bits del camino
        else pen.setWidth(3); //ancho máximo
            
        m_lineItems_p[m_lineItemsIndex++]=m_image_p->addLine(QLineF(m_pathPoints_p[m_pathPointIndex-1], m_pathPoints_p[m_pathPointIndex-2]), pen);
        m_lineItems_p[m_lineItemsIndex-1]->setFlag(QGraphicsItem::ItemIsSelectable, false);
        }
    if (m_tmpLine_p) { m_image_p->removeLineItem(m_tmpLine_p); m_tmpLine_p = 0; }
}

void Paths::drawTmpLineSegment()
{
    if (m_pathPoints_p && m_pathPointIndex>0)
    {
        QPointF point = m_image_p->mouseDragPoint();
        QPen pen;
        pen.setStyle(Qt::SolidLine); pen.setColor(m_params_p->pathColor);
        if (m_pathBits < 3) pen.setWidth(m_pathBits);
        else pen.setWidth(3);
        if (m_tmpLine_p)
            m_image_p->removeLineItem(m_tmpLine_p);
        m_tmpLine_p = m_image_p->addLine(QLineF(point, m_pathPoints_p[m_pathPointIndex-1]), pen);
    }
}

//inicializa el almacenamiento temporal de los caminos mientras se crean
void Paths::initGetPointsPath(int pathNumber, int pathBits)
{
    m_lineItemsIndex=0;
    m_pathPointIndex=0;
    m_tmpLine_p = 0;
    m_pathNumber=pathNumber; //referencia al primer camino libre
    m_pathBits=pathBits;
    m_openedPath=true;
}

//hace uso de la información del almacenamiento temporal y la traslada a la estructura PATH
//Tiene acceso al terminar la creación del camino
int Paths::finishGetPointsPath(int pathNumber, int pathBits)
{
    char txt[200];

    if(m_pathNumber<0 || m_pathNumber>MAX_PATHS) return -1;
    m_openedPath=false;
    PATH *path_p=m_paths_p+m_pathNumber;        //puntero a la estructura PATH             
    if (m_tmpLine_p) 
        {   QLineF line = m_tmpLine_p->line();
            m_pathPoints_p[m_pathPointIndex++] = line.p1(); 
            m_lineItems_p[m_lineItemsIndex++] = m_tmpLine_p;
            m_tmpLine_p = 0;
        }
    if (m_pathPointIndex < 2 || m_pathNumber < 0)    //se requieren dos o más puntos
        return -1;
    if(newPath(m_pathNumber, pathBits, m_pathPoints_p, m_pathPointIndex)==-1) //nuevo camino
    {
        sprintf(txt, "Warning: It is not possible to create the path p(%d)", m_pathNumber);
        QMessageBox::warning(0, "Paths", txt, QMessageBox::Cancel);
    }
    else
        {
        if(m_arrow.count()!=7 && m_arrow.count()!=4) //la flecha requiere de 7 puntos
        {
            QMessageBox::warning(0, "Paths", "warning: an arrow is necesary (setArrow(QPolygonF arrow))", QMessageBox::Cancel);
            return -1;
        }

        setArrow(m_arrow, m_mArrow, m_mArrowL); //establece en la estructura la info relacionada con la flecha
        
        //En m_lineItems_p[] están los items a los segmentos que componen el camino
        //se establecen al visualizar los caminos creados manualmente
        for(int s=0; s<m_lineItemsIndex; s++) 
                path_p->segment_p[s].lineItem=m_lineItems_p[s]; //se copian a la estructura PATH
        //Se visualiza el nombre del camino
        char txt[20];
        QPointF p1=m_pathPoints_p[0];
        QPointF p2=m_pathPoints_p[1];
        QPointF txtPos=(p1+p2)/2.0;
        txtPos.setY(txtPos.y()-0);
        sprintf(txt, "p(%d,%d)", m_pathNumber, pathBits);
        if(path_p->nameItem)                //si ya existe
            path_p->nameItem->setText(txt); //se actualiza
        else
            path_p->nameItem=newText(txt, 0, txtPos); //se crea y visualiza
        }
    return 0;
}

//Actualiza la información sobre número de bits del camino
//retorna -1 si no existe el camino o el número de bits es negativo
int Paths::updateBitsPath(int path, int bits)
{
    if(!existPath(path) || bits<1) return -1;
    m_paths_p[path].nBits=bits;
    char txt[20];
    sprintf(txt, "p(%d,%d)", path, bits);
    if(m_paths_p[path].nameItem)                //si ya existe
        m_paths_p[path].nameItem->setText(txt); //se actualiza
    return 0;
}


//evalua la lista de PATHs a la búsqueda de la primera entrada libre
//Retorna el índice a la primera entrada libre o -1 si no existe o no está creada la entrada en m_paths_p
int Paths::getFreePath()
{
    if(m_paths_p==0) return -1;
    int p;
    for(p=0; p<MAX_PATHS; p++)
        if(!existPath(p)) return p;
    if(p==MAX_PATHS) return -1;
    return -1;
}

//crea una nueva ruta y la inicializa a partir de los puntos dados
//Si no existe la estructura de rutas, la crea
//determina la posición donde se visualizarán los valores de inicio y fin del camino
//path = camino
//point_p = puntero a array de puntos que indican el camino de nPoints
//nPoints = #puntos en el camino
int Paths::newPath(int path, int pathBits, QPointF *point_p, int nPoints)
{
    if(path<0 || path>MAX_PATHS) return -1; //control de límites
    if(existPath(path)) return -1;          //si ya existe el camino
    if(pathBits<=0 || pathBits>8*sizeof(int)) pathBits=1;
    
    PATH *path_p=m_paths_p+path;            //puntero a la estructura PATH     
    
    if(m_paths_p==0)                        //crea la estructura si no existe
        {
        m_paths_p=new PATH[MAX_PATHS];
        for(int i=0; i<MAX_PATHS; i++)      //se inicializa la ruta
            m_paths_p[i].segment_p=0;
        }
        
        path_p->segment_p=new SEGMENT[nPoints-1];   //memoria para alojar los segmentos
        path_p->nSegments=nPoints-1;                //num de segmentos
        path_p->currentSeg=0;                       //segmento en curso
        path_p->nBits=pathBits;                     //bits del camino
        path_p->nameItem=0;                         //item para nombre de camino
        path_p->iniItem=0;                          //item para valor inicial 
        path_p->endItem=0;                          //item para valor final
        path_p->valueIni.unKnow=false;              //valor inicial '0'
        path_p->valueIni.value=0;                   //valor inicial
        path_p->valueIni.angle=0;                   //ángulo del texto inicial              
        path_p->valueEnd.unKnow=true;               //valor final 'x'
        path_p->valueEnd.angle=0;                   //ángulo del texto final          
        
        for(int seg=0; seg<nPoints-1; seg++)        //para cada segmento
            {
            path_p->segment_p[seg].currentDeltaSeg=0;   //delta segmento actual
            path_p->segment_p[seg].nDeltaSeg=0;         //cantidad de delta segmentos
            path_p->segment_p[seg].nForks=0;            //puntos de bifurcación
            path_p->segment_p[seg].pIni=point_p[seg];   //punto inicial
            path_p->segment_p[seg].pEnd=point_p[seg+1]; //punto final
            path_p->segment_p[seg].lineItem=0;          //item al segmento
            path_p->segment_p[seg].arrowItem=0;         //array de items a las flechas
            }
            
        //se determinan las posiciones para ubicar los textos de los valores iniciales y finales
        //se considera la pendiente y el sentido del comino
        //para el valor inicial
        QPointF pEnd=path_p->segment_p[path_p->nSegments-1].pEnd; //para escribir menos
        QPointF pIni=path_p->segment_p[path_p->nSegments-1].pIni;
        
        double dx, dy, angle;
        dx=pEnd.x()-pIni.x(); 
        dy=pEnd.y()-pIni.y();        
        if(dx!=0)angle=atan(dy/dx);
        else angle=M_PI/2;
        angle*=180/M_PI; //de radianes a grados
        QPointF valuePos=pEnd;
        if(fabs(angle)>45) //pendiente
            {
            if(pEnd.y()>pIni.y())   valuePos.setY(pEnd.y()-25); //sentido del camino
            else                    valuePos.setY(pEnd.y()+10);
            }
        else
            {
            if(pEnd.x()>pIni.x())   valuePos.setX(pEnd.x()-25);
            else                    valuePos.setX(pEnd.x()+9);
            }
         path_p->valueEnd.pos=valuePos;
        
        //para el valor final
        pEnd=path_p->segment_p[0].pEnd;
        pIni=path_p->segment_p[0].pIni;
        
        dx=pEnd.x()-pIni.x(); //dx*=dx;
        dy=pEnd.y()-pIni.y(); //dy*=dy;        
        if(dx!=0)angle=atan(dy/dx);
        else angle=M_PI/2; 
        angle*=180/M_PI; //de radianes a grados
        
        valuePos=pIni;
        if(angle>45) //pendiente
            {
            if(pEnd.y()>pIni.y())   valuePos.setY(pIni.y()+10); //sentido del camino
            else                    valuePos.setY(pIni.y()-25);
            }
        else
            {
            if(pEnd.x()>pIni.x())   valuePos.setX(pIni.x()+9);
            else                    valuePos.setX(pIni.x()-11);
            }
        path_p->valueIni.pos=valuePos;
return 0;
}

//elimina el camino en construcción
void Paths::removeTmpPath()
{
    if (m_tmpLine_p)
    {
        QLineF line = m_tmpLine_p->line();
        m_pathPoints_p[m_pathPointIndex++] = line.p2();
        m_lineItems_p[m_lineItemsIndex++]= m_tmpLine_p;
        m_tmpLine_p = 0;
    }
    for(int i=0; i<m_lineItemsIndex; i++)
        if(m_lineItems_p[i]) m_image_p->removeLineItem(m_lineItems_p[i]); 
    m_lineItemsIndex=0;
}

//Elimina la memoria reservada del camino y lo inicializa
//retorna -1 si no existe el path
int Paths::removePath(int path)
{
    if(path<0 || path>MAX_PATHS) return -1;
    if(m_paths_p==0) return -1; //no existe de path
    if(!existPath(path)) return -1;
    
    for(int s=0; s<m_paths_p[path].nSegments; s++) //para cada segmento
        {
        m_paths_p[path].segment_p[s].nForks=0;    
        if(m_paths_p[path].segment_p[s].lineItem)   //si existe el array de items de segmentos, se elimina
            {m_image_p->removeLineItem(m_paths_p[path].segment_p[s].lineItem); m_paths_p[path].segment_p[s].lineItem=0;}
        if(m_paths_p[path].segment_p[s].arrowItem)  //si existe el array de items a flechas
            {
            for(int j=0; j<m_paths_p[path].segment_p[s].nDeltaSeg; j++) //para cada flecha
                if(m_paths_p[path].segment_p[s].arrowItem[j]) //si existe, se elimina
                    {
                    m_image_p->removePolygonItem(m_paths_p[path].segment_p[s].arrowItem[j]);
                    m_paths_p[path].segment_p[s].arrowItem[j]=0;
                    }
            delete[] m_paths_p[path].segment_p->arrowItem; m_paths_p[path].segment_p[s].arrowItem=0; //memoria de flechas
            }
        }
    delete[] m_paths_p[path].segment_p; m_paths_p[path].segment_p=0; //memoria de segmentos
                                
    if(m_paths_p[path].iniItem) //si existe el item del valor inical, se borra
                {
                m_image_p->removeTextItem(m_paths_p[path].iniItem);
                m_paths_p[path].iniItem=0;   
                }
    if(m_paths_p[path].endItem) //si existe el item del valor final, se borra
                {
                m_image_p->removeTextItem(m_paths_p[path].endItem);
                m_paths_p[path].endItem=0; 
                }
    if(m_paths_p[path].nameItem) //si existe el item del nombre, se borra
                {
                m_image_p->removeTextItem(m_paths_p[path].nameItem);
                m_paths_p[path].nameItem=0;  
                }
    m_paths_p[path].segment_p=0;            //camino desactivado
    m_paths_p[path].currentSeg=0;           //segmento en curso
    m_paths_p[path].nSegments=0;            //# segmentos
    m_paths_p[path].valueIni.unKnow=true;   //valor inical 'x'
    m_paths_p[path].valueEnd.unKnow=true;   //valor final 'x'
    m_paths_p[path].blockIni=-1;            //bloque al inicio del camino
    m_paths_p[path].blockEnd=-1;            //bloque al fin del camino
    m_pathsNews[path].news=false;
    
   return 0;
}

//Elimina la memoria reservada de todos los caminos y la estructura
void Paths::removeAllPath()
{
    if(m_paths_p)
        {
        for(int p=0; p<MAX_PATHS; p++)
            removePath(p);
        delete[] m_paths_p; m_paths_p=0;
        }
    
}

//elimina todas las flechas y mantiene su memoria
void Paths::removeAllPathsArrowItems()
{
    for(int p=0; p<MAX_PATHS; p++)                      //para todos los caminos
        {
        if(!existPath(p)) continue;                     //si no existe el camino
        for(int s=0; s<m_paths_p[p].nSegments; s++)     //para todos los segmentos
            {
            if(m_paths_p[p].segment_p[s].arrowItem)     //si existe el array de flechas
                {
                for(int f=0; f<m_paths_p[p].segment_p[s].nDeltaSeg; f++) //para todas las flechas
                    if(m_paths_p[p].segment_p[s].arrowItem[f])          //si existe esta flecha, se elimina
                        {
                        m_image_p->removePolygonItem(m_paths_p[p].segment_p[s].arrowItem[f]);
                        m_paths_p[p].segment_p[s].arrowItem[f]=0;
                        }
                }
            }
        }
}

//elimina todas las líneas de camino y mantiene su memoria
void Paths::removeAllPathsLineItems()
{
    for(int p=0; p<MAX_PATHS; p++)                  //para todos los caminos
        {
        if(!existPath(p)) continue;                 //si no existe el camino
        for(int s=0; s<m_paths_p[p].nSegments; s++) //para todos los segmentos
            {
            if(m_paths_p[p].segment_p[s].lineItem)  //si existe el array de segementos 
                {
                m_image_p->removeLineItem(m_paths_p[p].segment_p[s].lineItem); //se elimina el segmento
                m_paths_p[p].segment_p[s].lineItem=0;
                }
            }
        }    
}

//elimina todos los valores de inicio y fin de camino
void Paths::removeAllPathsValueItems()
{
    for(int p=0; p<MAX_PATHS; p++)      //para todos los caminos
        {
        if(!existPath(p)) continue;     //si no existe el camino
        if(m_paths_p[p].iniItem)        //si existe el item del valor inicial 
            {
            m_image_p->removeTextItem(m_paths_p[p].iniItem);
            m_paths_p[p].iniItem=0;                    
            }
        if(m_paths_p[p].endItem)        //si existe el item del valor final
            {
            m_image_p->removeTextItem(m_paths_p[p].endItem);
            m_paths_p[p].endItem=0;                 
            }
        }
}

//elimina todos los nombres de camino
void Paths::removeAllPathsNameItems()
{
    for(int p=0; p<MAX_PATHS; p++)
        {
        if(!existPath(p)) continue;     //si no existe el camino
        if(m_paths_p[p].nameItem)       //si existe el nombre, se elimina
            {
            m_image_p->removeTextItem(m_paths_p[p].nameItem);
            m_paths_p[p].nameItem=0;                 
            }
        }            
}

//Elimina todos los items de todos los caminos
//retorna -1 si no existe el path
void Paths::removeAllPathsItems()
{
        for(int p=0; p<MAX_PATHS; p++)                  //para todos los caminos
            {
            if(!existPath(p)) continue;                 //si no existe el camino          
            for(int s=0; s<m_paths_p[p].nSegments; s++) //para todos los segmentos
                {
                if(m_paths_p[p].segment_p[s].arrowItem) //si existe el array de flechas
                    {
                    for(int f=0; f<m_paths_p[p].segment_p[s].nDeltaSeg; f++) //para todas las flechas
                        if(m_paths_p[p].segment_p[s].arrowItem[f]) //si existe, se elimina
                            {
                            m_image_p->removePolygonItem(m_paths_p[p].segment_p[s].arrowItem[f]);
                            m_paths_p[p].segment_p[s].arrowItem[f]=0;
                            }
                    }
                if(m_paths_p[p].segment_p[s].lineItem)  //si existe el camino, se elimina
                    { 
                    m_image_p->removeLineItem(m_paths_p[p].segment_p[s].lineItem);
                    m_paths_p[p].segment_p[s].lineItem=0;                        
                    }
                }
              
             if(m_paths_p[p].nameItem)  //si existe el nombre del camino, se elimina
                {
                m_image_p->removeTextItem(m_paths_p[p].nameItem); 
                m_paths_p[p].nameItem=0;                 
                }
             if(m_paths_p[p].iniItem)   //si existe el valor inicial del camino, se elimina  
                {
                m_image_p->removeTextItem(m_paths_p[p].iniItem); 
                m_paths_p[p].iniItem=0;                 
                }
             if(m_paths_p[p].endItem)   //si existe el valor finial del camino, se elimina    
                {
                m_image_p->removeTextItem(m_paths_p[p].endItem); 
                m_paths_p[p].endItem=0;                 
                }
            }        
}

//se visualiza la linea de los caminos
//si existe, la elimina y vuelve a crear
void Paths::drawPathLines(int path)
{ 
    if(!existPath(path)) return;
//    QPen pen(Qt::DashLine); //linea entrecortada
    QPen pen; 
    pen.setColor(m_params_p->pathColor);
    pen.setStyle(Qt::SolidLine); pen.setColor(m_params_p->pathColor); 
    if(m_paths_p[path].nBits<3) pen.setWidth(m_paths_p[path].nBits);
    else pen.setWidth(3);
        
        
    for(int seg=0; seg<m_paths_p[path].nSegments; seg++) //para todos los segmentos
        {
        QPointF p1, p2;
        p1=m_paths_p[path].segment_p[seg].pIni; //extremos del segmento
        p2=m_paths_p[path].segment_p[seg].pEnd;
    
        if(m_paths_p[path].segment_p[seg].lineItem) m_image_p->removeLineItem(m_paths_p[path].segment_p[seg].lineItem);
        m_paths_p[path].segment_p[seg].lineItem= m_image_p->addLine(QLineF(p1, p2), pen);  
        }
}

//visualiza el nombre del camino
//lo posiciona a la mitad del primer segmento
void Paths::drawPathName(int path)
{ 
        if(!existPath(path)) return;
        char txt[20];
        sprintf(txt, "p(%d,%d)", path, m_paths_p[path].nBits);
        QPointF p1=m_paths_p[path].segment_p[0].pIni; //extremos del primer segmento
        QPointF p2=m_paths_p[path].segment_p[0].pEnd;
        QPointF txtPos=(p1+p2)/2.0; //punto medio
        txtPos.setY(txtPos.y()-20);
        if(m_paths_p[path].nameItem) //si existe el item, lo actualiza
            m_paths_p[path].nameItem->setText(txt);
        else  //si no existe, lo crea
            m_paths_p[path].nameItem=newText(txt, 0, txtPos);
} 

//visualiza el nombre de todos los caminos
void Paths::drawAllPathsNames()
{
        for(int p=0; p<MAX_PATHS; p++)
                drawPathName(p);
}


//dibuja todos los caminos
void Paths::drawAllPathsLines()
{
    for(int path=0; path<MAX_PATHS; path++)
        {
        if(!existPath(path)) continue;                
        for(int s=0; s<m_paths_p[path].nSegments; s++)
            {
//            if(!m_paths_p[path].segment_p[s].lineItem) 
                    drawPathLines(path);
            }                
        }
}

//hace visibles todos los caminos
void Paths::setPathsVisibles(bool visible)
{
    for(int path=0; path<MAX_PATHS; path++)
        {
        if(!existPath(path)) continue;
        for(int s=0; s< m_paths_p[path].nSegments; s++)
            if(m_paths_p[path].segment_p[s].lineItem) m_paths_p[path].segment_p[s].lineItem->setVisible(visible);     
        }
}

//hace visibles todos los nombres de caminos
void Paths::setPathsNamesVisibles(bool visible)
{
    if(m_paths_p)
        for(int path=0; path<MAX_PATHS; path++)
            if(existPath(path) && m_paths_p[path].nameItem) m_paths_p[path].nameItem->setVisible(visible);     
}

//hace visibles todos los valores iniciales de caminos
void Paths::setPathsIniValuesVisibles(bool visible)
{
    if(m_paths_p)
        for(int path=0; path<MAX_PATHS; path++)
            if(existPath(path) && m_paths_p[path].iniItem) m_paths_p[path].iniItem->setVisible(visible);     
}

//hace visibles todos los valores finales de caminos
void Paths::setPathsEndValuesVisibles(bool visible)
{
    if(m_paths_p)
        for(int path=0; path<MAX_PATHS; path++)
            if(existPath(path) && m_paths_p[path].endItem) m_paths_p[path].endItem->setVisible(visible);     
}
                        
//crea un nuevo texto en la posición y orientación dada            
QGraphicsSimpleTextItem *Paths::newText(char *txt, double angle, QPointF pos)
{
    QGraphicsSimpleTextItem *txtItem= m_image_p->addText(txt);
    txtItem->setRotation(angle);
    txtItem->moveBy(pos.x(), pos.y());  
    return(txtItem);
}

//retorna el valor de aplicar al poligono poly el incremento de posición p
QPolygonF Paths::translatePoly(QPolygonF poly, QPointF p)
{
    QPolygonF poly2=poly;
    for (int i=0; i<poly.size(); i++)
       {
        poly2[i]=poly[i]+p;
       }
    return poly2;     
}

//crea un nuevo polígono (flecha) en la posición, orientación y color dados
//el color viene dado atendiendo al valor que debe representar
//El relleno depende de la flecha
//retorna el item
QGraphicsPolygonItem *Paths::newPolygon(QPolygonF poly, double angle, QPointF pos, char color)
{
    QPen pen; QBrush brush;
    if(m_params_p->arrowType==0) //grande
        brush.setStyle(Qt::Dense4Pattern); //relleno
    else 
        brush.setStyle(Qt::SolidPattern);

    if(color==0) //para '0'
        {
        pen.setStyle(Qt::SolidLine);// pen.setBrush(QBrush(m_params_p->arrowColor1, Qt::SolidPattern));//pen.setColor(m_params.arrowColor); 
        brush.setColor(m_params_p->arrowColor0); 
        }
    else if(color==1) //para '1'
        {
        pen.setStyle(Qt::SolidLine); //pen.setBrush(QBrush(m_params_p->arrowColor0, Qt::SolidPattern));//pen.setColor(m_params.arrowColor); 
        brush.setColor(m_params_p->arrowColor1); 
        }
    else if (color == 2) //para '2' //bus con ceros y unos
    {
        int H, S, V;
        QColor color = m_params_p->arrowColor1;
        color.getHsv(&H, &S, &V);
        S/=2;
        color.setHsv(H, S, V);
        pen.setStyle(Qt::SolidLine); //pen.setBrush(QBrush(m_params_p->arrowColor0, Qt::SolidPattern));//pen.setColor(m_params.arrowColor); 
        brush.setColor(color);
    }
    else //para 'x' (color=3)
        {
        pen.setStyle(Qt::SolidLine); pen.setBrush(QBrush(QColor(0,0,0), Qt::SolidPattern));//pen.setColor(m_params.arrowColor); 
        brush.setColor(QColor(255, 255, 255)); 
        }
    QGraphicsPolygonItem *arrowItem= m_image_p->addPolygon(poly, pen, brush);
    arrowItem->setRotation(angle);
    arrowItem->moveBy(pos.x(), pos.y());
    return(arrowItem);
}


//dibuja una flecha sobre la imagen. También la puede rotar
//uso de painter
void Paths::updatePoly(QPolygonF poly, double angle, QPointF pos)
{
    m_image_p->m_painter_p->save();
    m_image_p->m_painter_p->rotate(angle);  
    pos=rotatePoint(pos, -angle);
    m_image_p->m_painter_p->translate(pos.x(), pos.y());
    m_image_p->m_painter_p->drawPolygon(poly);
    m_image_p->m_painter_p->restore();
}

//Dibuja un texto sobre la imagen. También lo puede rotar
//uso de painter
void Paths::updateText(QString txt, double angle, QPointF pos)
{
    m_image_p->m_painter_p->save();
    m_image_p->m_painter_p->rotate(angle);  
    pos=rotatePoint(pos, -angle);
    m_image_p->m_painter_p->translate(pos.x(), pos.y());
    m_image_p->m_painter_p->drawText(0, 0, txt);
    m_image_p->m_painter_p->restore();
}

//rota un punto un ángulo (grados) y lo retorna
QPointF Paths::rotatePoint(QPointF p, double angle)
{
    QPointF p2;
    angle=2.0*M_PI*angle/360.0; //pasamos de grados a radianes
    double x1=p.x();
    double y1=p.y();
    double x2, y2;
    x2=x1*cos(angle)-y1*sin(angle); //rotación
    y2=x1*sin(angle)+y1*cos(angle);
    p2.setX(x2); //nuevo valor
    p2.setY(y2);
    return p2;
}

//convierte un valor entero a cadena de texto adecuándolo al tamaño de destino
//hay un problema con los valores negativos que se extienden al tamaño del entero y se debe corregir
//para que sean dos caracteres
void Paths::toText(int value, char *text, int nBits)
{
    if(nBits<0) nBits=0;
    else if(nBits>8*sizeof(int)) nBits=8*sizeof(int);
    if((value>>nBits)==0) //valor positivo
        sprintf(text, "0x%0X", value);
    else
        {
        char txt2[10];
        sprintf(txt2, "%0X", value);
        int size=(4*strlen(txt2)-nBits)/4; //número de nibles a eliminar
        sprintf(text, "0x%s", txt2+size);
        }
}

float Paths::distance(QPointF p1, QPointF p2)
{
    double x1 = p1.x(), x2 = p2.x(), y1 = p1.y(), y2 = p2.y();
    return((float)sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}


