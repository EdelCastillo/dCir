/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 *
 *    Dialogo para la gestion de caminos
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

#include "pathDialog.h"
#include "moc_pathDialog.cpp"
 
PathDialog::PathDialog()
{
    m_result=0;
    pathLabel =     new QLabel( "new_Path");
    bitsLabel =     new QLabel( "bits");
    delPathLabel =  new QLabel( "update/remove_Path");
//    valueLabel =    new QLabel( "initValue");
    
    pathValue =     new QLineEdit;
    bitsValue =    new QLineEdit;
    delPathValue =  new QLineEdit;

    m_deleteButton = new QPushButton("remove", this);
    m_updateButton = new QPushButton("update", this);
//    m_deleteButton->setGeometry(QRect( QPoint(100, 100), QSize(200, 50) ));
    m_finishButton = new QPushButton("close", this);
//    m_finishButton->setGeometry(QRect( QPoint(100, 100), QSize(200, 50) ));
    m_startButton = new QPushButton("open", this);
//    m_startButton->setGeometry(QRect( QPoint(100, 100), QSize(200, 50) ));
    m_cancelButton = new QPushButton("Cancel", this);
//    m_cancelButton->setGeometry(QRect( QPoint(100, 100), QSize(200, 50) ));
    
    HlayoutA = new QHBoxLayout();
	    HlayoutA->addWidget(pathLabel);
	    HlayoutA->addWidget(pathValue);
	    HlayoutA->addWidget(bitsLabel);
	    HlayoutA->addWidget(bitsValue);
	    groupA = new QGroupBox();
	    groupA->setLayout(HlayoutA);
        
    HlayoutB = new QHBoxLayout();
	    HlayoutB->addWidget(delPathLabel);
	    HlayoutB->addWidget(delPathValue);
        HlayoutB->addWidget(m_deleteButton);
        HlayoutB->addWidget(m_updateButton);
	    groupB = new QGroupBox();
	    groupB->setLayout(HlayoutB);
        
    pathValue->setMaximumWidth(60);
//    valueValue->setMaximumWidth(60);
    delPathValue->setMaximumWidth(60);
    
    HlayoutButton = new QHBoxLayout();
    HlayoutButton->addWidget(m_startButton);
    HlayoutButton->addWidget(m_finishButton);
    HlayoutButton->addWidget(m_cancelButton);
    //HlayoutButton->addWidget(m_deleteButton);
        groupButton = new QGroupBox();
        groupButton->setLayout(HlayoutButton);
 
    VlayoutA = 	new QVBoxLayout(this);
    VlayoutA->addWidget(groupA);
    VlayoutA->addWidget(groupButton);
        groupC = new QGroupBox(this);
        groupC->setLayout(VlayoutA);
//   groupC->setTitle(tr("- New Path -"));
    
    VlayoutB = 	new QVBoxLayout(this);
    VlayoutB->addWidget(groupC);
    VlayoutB->addWidget(groupB);
        groupTotal = new QGroupBox(this);
        groupTotal->setLayout(VlayoutB);
    
   groupTotal->setTitle(tr("- PATH -"));
   
    HlayoutA->setAlignment(pathLabel,       Qt::AlignRight); //alinea a la dereccha  el campo nameLabel
    HlayoutA->setAlignment(bitsLabel,       Qt::AlignRight); //alinea a la dereccha  el campo nameLabel
    groupA->setAlignment(Qt::AlignCenter); //alinea a la dereccha  el campo nameLabel
        
   
    bitsValue->setMaximumWidth(40);
    delPathValue->setMaximumWidth(50);
  
    connect(m_startButton,  SIGNAL (released()),this, SLOT (handleStartButton()));  
    connect(m_finishButton, SIGNAL (released()),this, SLOT (handleFinishButton()));  
    connect(m_deleteButton, SIGNAL (released()),this, SLOT (handleDeleteButton()));    
    connect(m_cancelButton, SIGNAL (released()),this, SLOT (handleCancelButton()));  
    connect(m_updateButton, SIGNAL (released()),this, SLOT (handleUpdateButton()));  
    
    
    QSize size2=groupTotal->sizeHint(); 
    setMinimumSize(size2.width(), size2.height());
}

//Atencion al boton de inicio de camino
void PathDialog::handleStartButton()
{
  m_result=0x01;
    done(1);
}

//Atencion al boton de fin de camino
void PathDialog::handleFinishButton()
{
  m_result=0x02;
    done(1);
}

//Atencion al boton de eliminacion de camino
void PathDialog::handleDeleteButton()
{
  m_result=0x03;
    done(1);
}

//Atencion al boton de cancelar operacion
void PathDialog::handleCancelButton()
{
  m_result=0x04;
    done(0);
}

//Atencion al boton de actualizar los bits del camino
void PathDialog::handleUpdateButton()
{
  m_result=0x05;
    done(0);
}

/////////////////////////////////////////////////////
// Definicion de clase de atencion al boton derecho del raton
// 
//Constructor
BottonRightDialog::BottonRightDialog()
{
    m_blocksButton = new QPushButton("Blocks", this);
    m_blocksButton->setGeometry(QRect( QPoint(100, 100), QSize(200, 50) ));
    m_pathsButton = new QPushButton("Paths", this);
    m_pathsButton->setGeometry(QRect( QPoint(100, 100), QSize(200, 50) ));
    m_cancelButton = new QPushButton("Cancel", this);
    m_cancelButton->setGeometry(QRect( QPoint(100, 100), QSize(200, 50) ));
    
    HlayoutButton = new QHBoxLayout();
    HlayoutButton->addWidget(m_pathsButton);
    HlayoutButton->addWidget(m_blocksButton);
    HlayoutButton->addWidget(m_cancelButton);
        groupButton = new QGroupBox();
        groupButton->setLayout(HlayoutButton);
 
    VlayoutA = 	new QVBoxLayout(this);
    VlayoutA->addWidget(groupButton);
    connect(m_blocksButton, SIGNAL (released()),this, SLOT (handleBlocksButton()));  
    connect(m_pathsButton,  SIGNAL (released()),this, SLOT (handlePathsButton()));  
    connect(m_cancelButton, SIGNAL (released()),this, SLOT (handleCancelButton()));  
}

//Atencion al boton de bloques
void BottonRightDialog::handleBlocksButton()
{
  m_result=0x01;
    done(0);
    
}

//Atencion al boton de caminos
void BottonRightDialog::handlePathsButton()
{
  m_result=0x02;
    done(0);
    
}

//Atencion al boton de cancelar
void BottonRightDialog::handleCancelButton()
{
  m_result=0x04;
    done(0);
    
}
