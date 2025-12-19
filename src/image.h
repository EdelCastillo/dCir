/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 *
 *    Visualizacion
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

#ifndef IMAGE_H
#define IMAGE_H

#include <QtGui>
#include <mouse.h>
#include <QMessageBox>

class Image{
public:    
    //Constructor: inicialización
    Image();
    
    //Destructor: libera la memoria reservada
    ~Image();
    
    //se destruye todo lo creado
    void removeImage();
    
    //visualiza la imagen de base
    int newImage(const char *baseImageFile);
    
    //se elimina la imagen anterior y se restablece
    void updateBaseImage(const char *baseImageFile);
    
    //retorna el top item situado bajo el mouse
    QGraphicsItem* getUnderMouseItem();
    
    //Retorna las coordenadas del mouse al pulsar el botón izquierdo
    QPointF mouseLeftBottonPoint();
    
    //Retorna las coordenadas del mouse al pulsar el botón derecho
    QPointF mouseRightBottonPoint();

    //Retorna las coordenadas del mouse al arrastrar
    QPointF mouseDragPoint();

    //añade una nueva linea a la escena
    //Retorna su item
    QGraphicsLineItem *addLine(QLineF, QPen pen);
    
    //añade un rectángulo a la escena
    //Retorna su item
    QGraphicsRectItem *addRect(QRect rec, QPen pen, QBrush);
    
    //añade un polígono a la escena
    //Retorna su item
    QGraphicsPolygonItem *addPolygon(QPolygonF poly, QPen pen, QBrush brush);
    
    //añade un texto a la escena
    //Retorna su item
    QGraphicsSimpleTextItem *addText(char *text);
    
    //mueve un QGraphicsItem
    void moveQGraphicsItem(QGraphicsItem *gItem);
    
    //elimina una linea a partir de su item
    //retorna -1 si no existe el item
    int removeLineItem(QGraphicsLineItem *lineItem);
    
    //elimina un rectángulo a partir de su item
    //retorna -1 si no existe el item
    int removeRectItem(QGraphicsRectItem *recItem);
    
    //elimina un texto a partir de su item
    //retorna -1 si no existe el item
    int removeTextItem(QGraphicsSimpleTextItem *textItem);
    
    //elimina un polígono a partir de su item
    //retorna -1 si no existe el item
    int removePolygonItem(QGraphicsPolygonItem *polyItem);

    //genera una imagen en blanco en la ruta absoluta dada
    void blankImage(int width, int height, char *absPath);
    
    class Mouse         *m_scene_p;     //puntero a clase para la escena
    QGraphicsView       *m_view_p;      //puntero a clase para la vista
    QPainter            *m_painter_p;   //puntero a clase para dibujar
    QImage              *m_image_p;     //puntero a clase 
    QGraphicsPixmapItem *m_pixmapItem;  //puntero a item de la imagen de fondo
    QPointF              m_offset;
};

#endif
