/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 *
 *    Dialogo para establecimiento de parametros de operacion
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

#include "paramsDialog.h"
#include "moc_paramsDialog.cpp"
 
//Constructor
ParamsDialog2::ParamsDialog2()
  {
//  this->setMinimumSize(700, 195);
  m_accept=false;  
    graphicLabel= 	    new QLabel("graphicsFileName");
    velocityLabel =     new QLabel("velocity(%)");
    arrowTypeLabel =    new QLabel("arrowType");
    arrowColor0Label =  new QLabel("arrowColor'0'(RGB)");
    arrowColor1Label =  new QLabel("arrowColor'1'(RGB)");
    pathColorLabel =    new QLabel("pathColor(RGB)");
    blockColorLabel =   new QLabel("blockColor(RGB)");
    deltaTLabel =       new QLabel("deltaT(ms)");

    graphicButton = 	new QPushButton("graphicFile", this);
                
    graphicValue =	    new QLineEdit;
    velocityValue =	    new QLineEdit;
    arrowColor0Value =	new QLineEdit;
    arrowColor1Value =	new QLineEdit;
    pathColorValue =	new QLineEdit;
    blockColorValue =	new QLineEdit;
    
    viewPaths= new QCheckBox("viewPath", this);
    viewPathsName= new QCheckBox("viewPathNames", this);
    viewIniValuePaths= new QCheckBox("viewIniValuePaths", this);
    viewEndValuePaths= new QCheckBox("viewEndValuePaths", this);
    viewNodeBlocksName= new QCheckBox("viewNodeBlocksName", this);
    viewPortBlocksName= new QCheckBox("viewPortBlocksName", this);
    viewOthersBlocksName= new QCheckBox("viewOthersBlocksNames", this);
    viewRectBlocks= new QCheckBox("viewRectBlocks", this);
    
    arrowType= new QComboBox;
    arrowType->addItem(tr("huge"));
    arrowType->addItem(tr("large"));
    arrowType->addItem(tr("medium"));
    arrowType->addItem(tr("small"));
    arrowType->addItem(tr("tiny"));

 //   deltaTValue = new QComboBox;
 //   deltaTValue->addItem(tr("100"));
 //   deltaTValue->addItem(tr("50"));
 //   deltaTValue->addItem(tr("25"));
 //   deltaTValue->addItem(tr("10"));


  graphicValue->setMaximumWidth(400);
  graphicValue->setMinimumWidth(300);
  velocityValue->setMaximumWidth(75);
  arrowColor0Value->setMaximumWidth(110);
  arrowColor1Value->setMaximumWidth(110);
  
    m_acceptButton = new QPushButton("Accept", this);
    m_acceptButton->setGeometry(QRect( QPoint(100, 100), QSize(200, 50) ));
    m_cancelButton = new QPushButton("Cancel", this);
    m_cancelButton->setGeometry(QRect( QPoint(100, 100), QSize(200, 50) ));

    HlayoutA= new QHBoxLayout();
	    HlayoutA->addWidget(graphicLabel);
	    HlayoutA->addWidget(graphicValue);
	    HlayoutA->addWidget(graphicButton);
    groupA= new QGroupBox();
	    groupA->setLayout(HlayoutA);
    HlayoutA->setAlignment(graphicLabel, Qt::AlignRight); //alinea a la dereccha
    HlayoutA->setAlignment(graphicValue, Qt::AlignLeft); //alinea a la izquierda
    HlayoutA->setAlignment(Qt::AlignCenter); //alinea a la dereccha

    HlayoutB= new QHBoxLayout();
	    HlayoutB->addWidget(arrowColor0Label);
	    HlayoutB->addWidget(arrowColor0Value);
	    HlayoutB->addWidget(arrowColor1Label);
	    HlayoutB->addWidget(arrowColor1Value);
	    HlayoutB->addWidget(pathColorLabel);
	    HlayoutB->addWidget(pathColorValue);
	    HlayoutB->addWidget(blockColorLabel);
	    HlayoutB->addWidget(blockColorValue);
    groupB= new QGroupBox();
	    groupB->setLayout(HlayoutB);
            
    HlayoutC= new QHBoxLayout();
       // HlayoutC->addWidget(deltaTLabel);
       // HlayoutC->addWidget(deltaTValue);
        HlayoutC->addWidget(velocityLabel);
	    HlayoutC->addWidget(velocityValue);
	    HlayoutC->addWidget(viewPaths);
	    HlayoutC->addWidget(viewPathsName);
	    HlayoutC->addWidget(viewIniValuePaths);
	    HlayoutC->addWidget(viewEndValuePaths);
    groupC= new QGroupBox();
	    groupC->setLayout(HlayoutC);
    HlayoutC->setAlignment(deltaTLabel, Qt::AlignRight); //alinea a la dereccha
    HlayoutC->setAlignment(deltaTValue, Qt::AlignLeft); //alinea a la dereccha
    HlayoutC->setAlignment(velocityLabel, Qt::AlignRight); //alinea a la dereccha
    HlayoutC->setAlignment(velocityValue, Qt::AlignLeft); //alinea a la dereccha
    HlayoutC->setAlignment(Qt::AlignCenter); //alinea a la dereccha

    HlayoutD= new QHBoxLayout();
	    HlayoutD->addWidget(arrowTypeLabel);
	    HlayoutD->addWidget(arrowType);
	    HlayoutD->addWidget(viewNodeBlocksName);
	    HlayoutD->addWidget(viewPortBlocksName);
	    HlayoutD->addWidget(viewOthersBlocksName);
	    HlayoutD->addWidget(viewRectBlocks);
    groupD= new QGroupBox();
	    groupD->setLayout(HlayoutD);
    HlayoutD->setAlignment(arrowTypeLabel, Qt::AlignRight); //alinea a la dereccha
    HlayoutD->setAlignment(arrowType, Qt::AlignLeft); //alinea a la izquierda
    HlayoutD->setAlignment(Qt::AlignCenter); //alinea a la dereccha

    HlayoutE= new QHBoxLayout();
	    HlayoutE->addWidget(m_acceptButton);
	    HlayoutE->addWidget(m_cancelButton);
    groupE= new QGroupBox();
	    groupE->setLayout(HlayoutE);
        
   VlayoutTotal = 	new QVBoxLayout(this);
        VlayoutTotal->addWidget(groupA);
        VlayoutTotal->addWidget(groupB);
        VlayoutTotal->addWidget(groupC);
        VlayoutTotal->addWidget(groupD);
        VlayoutTotal->addWidget(groupE);
  groupTotal = 	new QGroupBox(this);
        groupTotal->setLayout(VlayoutTotal);
         
  m_acceptButton->setDefault(true);
  m_acceptButton->setAutoDefault(true);

  groupTotal->setTitle(tr("-Parameters-"));
//   groupA->setTitle(tr("- Parameters -"));
   
    connect(m_acceptButton, SIGNAL (released()),this, SLOT (handleAcceptButton()));    
    connect(m_cancelButton, SIGNAL (released()),this, SLOT (handleCancelButton()));  
    connect(graphicButton,  SIGNAL (released()),this, SLOT (handleGraphicButton())); 
  
    QSize size2=groupTotal->sizeHint(); 
    setMinimumSize(size2);
  }

//Atencion al boton de aceptar
void ParamsDialog2::handleAcceptButton()
{
  m_accept=true;
    done(1);
}

//Atencion al boton de cancelar
void ParamsDialog2::handleCancelButton()
{
  m_accept=false;
    done(0);
}

//Atencion al boton de conseguir el fichero grafico
void ParamsDialog2::handleGraphicButton() 
{
    QString graphicsFile = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/", tr("Graphics Files (*.png *.jpg)"));
    if(graphicsFile !="")
    graphicValue->setText(graphicsFile);  
}
 
