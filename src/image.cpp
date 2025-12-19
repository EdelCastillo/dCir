/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 *
 *    Visualización
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

#include "image.h"

//Constructor: inicialización
Image::Image()
{
    m_painter_p=0;
    m_view_p=0;
    m_image_p=0;
    m_scene_p=0;
    m_pixmapItem=0;
}

//Destructor: libera la memoria reservada
Image::~Image()
{
    removeImage();
}

//se destruye todo lo creado
void Image::removeImage()
{
    if(m_pixmapItem) m_scene_p->removeItem(m_pixmapItem); //imagen del circuito digital de fondo
    if(m_painter_p) delete m_painter_p; 
    if(m_image_p)   delete m_image_p;
    if(m_scene_p)   delete m_scene_p;
    if(m_view_p)    delete m_view_p;
}

//visualiza la imagen de base
int Image::newImage(const char *baseImageFile)
{
    FILE *fp;
    char txt[200];

    //intenta abrir el fichero con el circuito digital
    if((fp=fopen(baseImageFile, "r"))==0)
             {
                sprintf(txt, "Warning: could not open '%s' file.\nupdate the project file", baseImageFile);
                QMessageBox::warning(0, "Image", txt, QMessageBox::Cancel);
                return -1;
            }
   fclose(fp);
    
    removeImage();                  //elimina la imagen anterior, si existe
    m_view_p  = new QGraphicsView;    //clase para la vista    
    m_scene_p = new Mouse(m_view_p);  //clase para la escena

    m_view_p->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QPixmap imagePx(baseImageFile);
    if(imagePx.width()==0 || imagePx.height()==0)
    {
        sprintf(txt, "Warning: file '%s' not found", baseImageFile);
        QMessageBox::warning(0, "Image", txt, QMessageBox::Cancel);
        return -1;
    }
    QRectF rec(0,0, imagePx.width(), imagePx.height());
    QRect rg(0, 0, imagePx.width(), imagePx.height());
    m_view_p->setGeometry(rg);  //geometría adecuada a la ventana
    m_scene_p->setSceneRect(rec);   //dimensionado de la ventana

    m_view_p->setScene(m_scene_p);  //se conecta la vista con la escena
    m_image_p=new QImage(rec.width(), rec.height(), QImage::Format_ARGB32); 
    m_painter_p=new QPainter(m_image_p); //para dibujar sobre la ventana
    m_painter_p->drawRect (rec);
    m_painter_p->eraseRect(rec);    //borra su contenido
    
    m_painter_p->drawPixmap(rec, imagePx, rec);  //visualiza es esquema digital
    QPixmap pixmap=(QPixmap::fromImage(*m_image_p));
    m_pixmapItem=m_scene_p->addPixmap(pixmap);

    m_view_p->show();
    return 0;
}

//genera una imagen en blanco en la ruta absoluta dada
void Image::blankImage(int width, int height, char *fileName)
{
    char txt[200];
    QImage image(width, height, QImage::Format_ARGB32);
    QPainter painter(&image);
    sprintf(txt, "dCir blank image (%dx%d)", width, height);
    painter.fillRect(image.rect(), Qt::white);
    painter.drawText(image.rect(), Qt::AlignHCenter | Qt::AlignTop, txt);
    if(!fileName)
        image.save((char*)"blankImage.png");   
    else
        image.save(fileName);   
}

//se elimina la imagen anterior y se restablece
void Image::updateBaseImage(const char *baseImageFile)
{
    QPixmap pixmap(QPixmap::fromImage(*m_image_p));
    QRectF rec=m_view_p->sceneRect();                   //tamaño de la vista
    m_painter_p->eraseRect(rec);                        //borrado
            
    QPixmap imagePx(baseImageFile);
            
    QRectF rec2(0,0, imagePx.width(), imagePx.height()); //tamaño anterior
    m_painter_p->drawPixmap(rec2, imagePx, rec2);        //se dibuja la imagen de fondo
    m_pixmapItem->setPixmap(pixmap);    
}

//retorna el top item situado bajo el mouse
QGraphicsItem* Image::getUnderMouseItem()
{
    //Conversión de coordenadas de mouse a view
    QPoint p=m_view_p->mapFromScene(m_scene_p->m_clicLeftPoint);
    
    //Detección del item superior en esa posición
    return (m_view_p->itemAt(p.x(), p.y()));
}

//Retorna las coordenadas del mouse al pulsar el botón izquierdo
QPointF Image::mouseLeftBottonPoint()
{
 return(m_scene_p->m_clicLeftPoint);   
}

//Retorna las coordenadas del mouse al pulsar el botón derecho
QPointF Image::mouseRightBottonPoint()
{
 return(m_scene_p->m_clicRightPoint);   
}

//Retorna las coordenadas del mouse al arrastrar
QPointF Image::mouseDragPoint()
{
    return(m_scene_p->m_dragPoint);
}

//añade una nueva linea a la escena
//Retorna su item
QGraphicsLineItem *Image::addLine(QLineF line, QPen pen)
{
  return(m_scene_p->addLine(line, pen));  
}

//añade un rectángulo a la escena
//Retorna su item
QGraphicsRectItem *Image::addRect(QRect rec, QPen pen, QBrush brush)
{
  return(m_scene_p->addRect(rec, pen, brush));  
}

//añade un polígono a la escena
//Retorna su item
QGraphicsPolygonItem *Image::addPolygon(QPolygonF poly, QPen pen, QBrush brush)
{
  return(m_scene_p->addPolygon(poly, pen, brush));  
    
}

//añade un texto a la escena
//Retorna su item
QGraphicsSimpleTextItem *Image::addText(char *text)
{
  return(m_scene_p->addSimpleText(text));  
}

//mueve un QGraphicsItem
void Image::moveQGraphicsItem(QGraphicsItem *gItem)
{
    QPointF p=gItem->mapFromScene(m_scene_p->m_dragPoint); //Nueva posición
    gItem->moveBy(p.x(), p.y()); //desplazamiento
}

//elimina una linea a partir de su item
//retorna -1 si no existe el item
int Image::removeLineItem(QGraphicsLineItem *lineItem)
{
    
    if(lineItem==0) return -1; 
    m_scene_p->removeItem(lineItem);
    return 0;
}

//elimina un rectángulo a partir de su item
//retorna -1 si no existe el item
int Image::removeRectItem(QGraphicsRectItem *recItem)
{
    
    if(recItem==0) return -1; 
    m_scene_p->removeItem(recItem);
    return 0;
}

//elimina un texto a partir de su item
//retorna -1 si no existe el item
int Image::removeTextItem(QGraphicsSimpleTextItem *textItem)
{
    if(textItem==0) return -1; 
    m_scene_p->removeItem(textItem);
    return 0;    
}

//elimina un polígono a partir de su item
//retorna -1 si no existe el item
int Image::removePolygonItem(QGraphicsPolygonItem *polyItem)
{
    if(polyItem==0) return -1; 
    m_scene_p->removeItem(polyItem);
    return 0;    
}

