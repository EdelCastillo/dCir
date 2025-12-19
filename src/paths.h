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

#ifndef PATHS_H
#define PATHS_H

#include "types.h"
#include "image.h"
#include <QMessageBox>

class Paths
{
public:    
    //constructor: recibe un puntero a la clase Image para poder dibujar sobre la imagen de fondo
    Paths(class Image *image_p);  
    
    //destructor
    ~Paths();
    
    //inicializa un caminos, si está activo, con un valor dado y bloquea su flujo de información
    int initPath(int path, int value, bool unKnow=false);
    
    //inicializa los caminos activos con valor de inicio y fin = 'x'
    int initAllPath(int value, bool unKnow=false);
    
    //inicializa los caminos activos con valor de inicio y fin = 'x'
    void initAllPathUnknown();
    
    //inicializa un camino, si activo, a un valor dado y permite que fluya la información
    int  startPath(int path, int value, bool unKnow);
    
    //copia un puntero a los parámetros
    void setParams(PARAMS *params);
    
    //se actualiza la información dependiente de la flecha
    void setArrow(QPolygonF arrow, QPolygonF mArrow, QPolygonF mArrowL);
    
    //crea todos los caminos y los inicializa a 'x'
    //reserva memoria si no está ya reservada
    void createAllPaths();
    
    //Recibe control del timer cada m_params.delta_T (ms) con resolución m_params.deltaT_blocks
    //Se actualizan las flechas sobre todos los caminos activos y se visualiza el valor de la información
    void evaluatePaths();
    
    //inicializa el almacenamiento temporal de los caminos mientras se crean
    void initGetPointsPath(int pathNumber, int pathBits);
    
    //hace uso de la información del almacenamiento temporal y la traslada a la estructura PATH
    int  finishGetPointsPath(int pathNumber, int pathBits);
    
    //Recibe control por ratón (botón izquierdo)
    //acumula puntos e items de cada segmento que va dibujando sobre la marcha
    void drawLineSegment();
    
    void drawTmpLineSegment();
    
    //retorna true si el camino ya posee un nombre
    bool existPath(int path);
    
    //retorna true si el camino ya posee un nombre
    bool existNamePath(int path);
    
    //evalua la lista de PATHs a la búsqueda de la primera entrada libre
    //Retorna el índice a la primera entrada libre o -1 si no existe o no está creada la entrada en m_paths_p
    int  getFreePath();
    
    //crea una nueva ruta y la inicializa a partir de los puntos dados
    //Si no existe la estructura de rutas, la crea
    //determina la posición donde se visualizarán los valores de inicio y fin del camino
    int  newPath(int path, int pathBits, QPointF *point_p, int nPoints);
    //se visualiza la linea de los caminos
    //si existe, la elimina y vuelve a crear
    void drawPathLines(int path);
    
    //visualiza el nombre del camino
    //lo posiciona a la mitad del primer segmento
    void drawPathName(int path);
    
    //dibuja todos los caminos
    void drawAllPathsLines();
    
    //visualiza el nombre de todos los caminos
    void drawAllPathsNames();
    
    //elimina el camino en construcción
    void removeTmpPath();
    
    //Elimina la memoria reservada de todos los caminos y la estructura
    int  removePath(int path);
    
    //Elimina la memoria reservada de todos los caminos y la estructura
    void removeAllPath();
    
    //elimina todas las flechas y mantiene su memoria
    void removeAllPathsArrowItems();
    
    //elimina todas las líneas de camino y mantiene su memoria
    void removeAllPathsLineItems();
    
    //elimina todos los nombres de camino
    void removeAllPathsValueItems();
    
    //elimina todos los nombres de camino
    void removeAllPathsNameItems();
    
    //elimina todos los nombres de camino
    void removeAllPathsItems();
    
    //hace visibles todos los nombres de caminos
    void setPathsVisibles(bool visible);
    
    //hace visibles todos los nombres de caminos
    void setPathsNamesVisibles(bool visible);
    
    //hace visibles todos los valores finales de caminos
    void setPathsIniValuesVisibles(bool visible);
    
    //hace visibles todos los valores finales de caminos
    void setPathsEndValuesVisibles(bool visible);

    //Retorna un entero con nBits a '1' en su parte baja
    int  andMask(char nBits);
       
    //convierte un valor entero a cadena de texto adecuándolo al tamaño de destino
    void toText(int value, char *text, int nBits=8);
    
    //Crea o actualiza la estructura FORK en un segmento de path a partir de point
    //retorna -1 si no existe el tab y no se puede crear o no existe el camino
    int setFork(QPointF point, int path, int node);
    
    //Actualiza la información sobre número de bits del camino
    //retorna -1 si no existe el camino o el número de bits es negativo
    int updateBitsPath(int path, int bits);
    
    //revisa que todos los caminos activos sean correctos e informa si algo falla
    //retorna false si algo no está bien
    bool isAllPathsOK();
    
    //crea un nuevo polígono (flecha) en la posición, orientación y color dados
    //el color viene dado atendiendo al valor que debe representar
    //El relleno depende de la flecha
    //retorna el item
    QGraphicsPolygonItem    *newPolygon(QPolygonF poly, double angle, QPointF pos, char color);
    
    //crea un nuevo texto en la posición y orientación dada            
    QGraphicsSimpleTextItem *newText(char *txt, double angle, QPointF pos);
   
    //distancia Euclidea entre dos puntos
    float distance(QPointF p1, QPointF p2);

    PATH        *m_paths_p;             //puntero a array de PATHs
    PARAMS      *m_params_p;            //puntero a los parámetros del proyecto
    PATH_NEWS   m_pathsNews[MAX_PATHS]; //array de novedades sobre la información que circula por los caminos
    bool        m_openedPath;           //true si un camino nuevo se está creando
    
private:
    //retorna el valor de aplicar al poligono poly el incremento de posición p
    QPolygonF               translatePoly(QPolygonF poly, QPointF p);
    //dibuja una flecha sobre la imagen. También la puede rotar
    void                    updatePoly(QPolygonF poly, double angle, QPointF pos);
    //Dibuja un texto sobre la imagen. También lo puede rotar
    void                    updateText(QString txt, double angle, QPointF pos);
    //Dibuja un texto sobre la imagen. También lo puede rotar
    QPointF                 rotatePoint(QPointF p, double angle);
    
    class Image *m_image_p;             //puntero a clase Image
    QGraphicsLineItem **m_lineItems_p;  //puntero a array de items de camino (temporal para la construcción)
    QGraphicsLineItem * m_tmpLine_p;    //item a un segmento de linea. Utilizado durante la construcción del camino
    QPointF     *m_pathPoints_p;        //puntero a array de puntos de camino (temporal para la construcción)
    int         m_pathPointIndex,       //tamaño del array de puntos
                m_lineItemsIndex;       //tamaño del array de items
    
    int         m_pathNumber,           //mantiene la referencia al camino que se está creando
                m_pathBits;             //bits del camino. Utilizado durante la construcción del camino
    QPolygonF   m_arrow,                //flecha en uso
                m_mArrow,               //media flecha en uso
                m_mArrowL;              //parte izquierda de la flecha (rectángulo)
                
};
#endif
