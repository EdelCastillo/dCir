/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 *
 *    Manejo del raton
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

#ifndef MOUSE_H
#define MOUSE_H

#include <QtWidgets>

class Mouse : public QGraphicsScene
{
    Q_OBJECT
public slots:
    //Recibe control al arrastrar el ratón con un botón pulsado
    void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
    
    //Actualiza las coordenadas del píxel en mouseX y mouseY
    //Actualiza el nombre del objecto (m_viewName) y el número de la ventana (m_viewNumber) que recibe el clic
    //Si hay problemas m_viewNumber=-1 y m_viewName=NULL
    //Se espera el formato en objectName(): "name_number"
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    
    //liberación de la techa  
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

signals:
      void bottonRight();
      void bottonLeft();
      void dragMouse();
public:
    // Constructor
    Mouse(QObject *parent = 0);
          
    QObject 	*m_parent;          //ventana padre
    QPointF	    m_clicLeftPoint,    //posición del ratón al pulsar el botón izquierdo
                m_clicRightPoint;   //posición del ratón al pulsar el botón derecho
    QPointF	    m_dragPoint;        //posición del ratón al arrastrar
};
#endif
