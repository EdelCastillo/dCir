/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 *
 *    Manejo del ratón
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

#include <QtGui>
#include "mouse.h"
#include "moc_mouse.cpp"
#include "dCir.h"

#include <QGraphicsSceneMouseEvent>

extern class Gui *extGui_p;  //puntero global a la clase Gui

// Constructor
Mouse::Mouse(QObject *parent)
{ 
    m_parent=parent;
   
    if(extGui_p) //se conectan diferentes slots que conectan con funciones de la clase Gui
        {
        connect(this, SIGNAL(bottonRight()), extGui_p, SLOT(bottonRightAttn())); //attn al botón derecho
        connect(this, SIGNAL(bottonLeft()),  extGui_p, SLOT(bottonLeftAttn()));  //attn al botón izquierdo
        connect(this, SIGNAL(dragMouse()),   extGui_p, SLOT(dragMouseAttn()));   //attn al arrastre
    //  ...  
        }
}

//Recibe control al arrastrar el ratón con un botón pulsado
void Mouse::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{  
    m_dragPoint=mouseEvent->scenePos(); //posición del ratón a cada llamada
    emit dragMouse();                   //conecta con Gui
    return;
}
 
//Actualiza las coordenadas del píxel en mouseX y mouseY
//Actualiza el nombre del objecto (m_viewName) y el número de la ventana (m_viewNumber) que recibe el clic
//Si hay problemas m_viewNumber=-1 y m_viewName=NULL
//Se espera el formato en objectName(): "name_number"
void Mouse::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  if (event->button() & Qt::RightButton)
    {
    m_clicRightPoint=event->scenePos(); //posición del ratón
    emit bottonRight();                 //conecta con Gui
    }    
  if (event->button() & Qt::LeftButton)
  {
    m_clicLeftPoint=event->scenePos();  //posición del ratón
   emit bottonLeft();                   //conecta con Gui  
  }
}

//liberación de la techa  
void Mouse::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;
    this->update();
}
