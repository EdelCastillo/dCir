/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 *
 *    Dialogo para la creacion de un nuevo proyecto
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

#include "newProjectDialog.h"
#include "moc_newProjectDialog.cpp"
 
//Constructor
//acepta un puntero a la estructura PARAMS dado que la puede actualizar
NewProjectDialog::NewProjectDialog(PARAMS *params_p)
  {
//  this->setMinimumSize(700, 195);
  m_params_p=params_p;
  m_accept=false;  
    
  baseDirLabel = 	new QLabel( "Project file directory");
  baseDirValue =	new QLineEdit;
  baseDirButton = 	new QPushButton("Directory", this);
  graphicButton = 	new QPushButton("graphicFile", this);
  paramsButton = 	new QPushButton("parameters", this);
  nameLabel = 	    new QLabel( "Project name");
  graphicLabel =    new QLabel( "background graphics file");
  widthLabel=       new QLabel("W");
  heightLabel=      new QLabel("H");
  graphicValue =	new QLineEdit;
  nameValue =	    new QLineEdit;
  widthValue =	    new QLineEdit;
  heightValue =	    new QLineEdit;
  
    CkBlankImage=  new QCheckBox("blankImage",  this);
  
  baseDirValue->setMinimumWidth(400);
  nameValue->setMinimumWidth(200);
  graphicValue->setMaximumWidth(400);
  widthValue->setMaximumWidth(50);
  heightValue->setMaximumWidth(50);
  
  baseDirValue->setReadOnly(true);
  

    HlayoutA= new QHBoxLayout();
	    HlayoutA->addWidget(baseDirLabel);
	    HlayoutA->addWidget(baseDirValue);
	    HlayoutA->addWidget(baseDirButton);
    groupA= new QGroupBox();
	    groupA->setLayout(HlayoutA);
//   groupA->setTitle(tr("- New project -"));
   
    baseDirButton->setDefault(false);
    baseDirButton->setAutoDefault(false);
    baseDirButton->setDefault(false);
    baseDirButton->setAutoDefault(false);
        
 
    HlayoutB= new QHBoxLayout();
	    HlayoutB->addWidget(nameLabel);
	    HlayoutB->addWidget(nameValue);
	    HlayoutB->addWidget(CkBlankImage);
	    HlayoutB->addWidget(widthLabel);
	    HlayoutB->addWidget(widthValue);
	    HlayoutB->addWidget(heightLabel);
	    HlayoutB->addWidget(heightValue);
	    HlayoutB->addWidget(paramsButton);
    groupB= new QGroupBox();
	    groupB->setLayout(HlayoutB);
        
    HlayoutD= new QHBoxLayout();
	    HlayoutD->addWidget(graphicLabel);
	    HlayoutD->addWidget(graphicValue);
	    HlayoutD->addWidget(graphicButton);
    groupD= new QGroupBox();
	    groupD->setLayout(HlayoutD);
//   groupD->setTitle(tr("- back ground graphics file -"));
   
    graphicButton->setDefault(false);
    graphicButton->setAutoDefault(false);
    graphicButton->setDefault(false);
    graphicButton->setAutoDefault(false);
    
    connect(baseDirButton, SIGNAL (released()),this, SLOT (handleBaseDirButton())); 
    connect(graphicButton, SIGNAL (released()),this, SLOT (handleGraphicButton())); 
    connect(paramsButton,  SIGNAL (released()),this, SLOT (handleParamsButton())); 
    connect(CkBlankImage,  SIGNAL (stateChanged(int)), this, SLOT (handleBlankImage()));
    

m_acceptButton = new QPushButton("Accept", this);
m_acceptButton->setGeometry(QRect( QPoint(100, 100), QSize(200, 50) ));
m_cancelButton = new QPushButton("Cancel", this);
m_cancelButton->setGeometry(QRect( QPoint(100, 100), QSize(200, 50) ));
     
  HlayoutC = 	new QHBoxLayout();
	    HlayoutC->addWidget(m_acceptButton);
	    HlayoutC->addWidget(m_cancelButton);
	    groupC = new QGroupBox();
	    groupC->setLayout(HlayoutC);
//	    groupC->setTitle(tr("- Action -"));

   VlayoutTotal = 	new QVBoxLayout(this);
   VlayoutTotal->addWidget(groupA);
   VlayoutTotal->addWidget(groupB);
   VlayoutTotal->addWidget(groupD);
   VlayoutTotal->addWidget(groupC);

  groupTotal = 		new QGroupBox(this);
  groupTotal->setLayout(VlayoutTotal);
  groupTotal->setTitle(tr("- new project -"));
  m_acceptButton->setDefault(true);
  m_acceptButton->setAutoDefault(true);

  connect(m_acceptButton, SIGNAL (released()),this, SLOT (handleAcceptButton()));    
  connect(m_cancelButton, SIGNAL (released()),this, SLOT (handleCancelButton()));  
  
    HlayoutB->setAlignment(nameLabel,    Qt::AlignRight); //alinea a la dereccha  el campo nameLabel
    HlayoutB->setAlignment(nameValue,    Qt::AlignLeft); //alinea a la dereccha  el campo nameLabel
    HlayoutB->setAlignment(paramsButton,    Qt::AlignRight); //alinea a la dereccha  el campo nameLabel
//    HlayoutB->setAlignment(Qt::AlignCenter); //alinea a la dereccha  el campo nameLabel
    HlayoutD->setAlignment(graphicLabel, Qt::AlignCenter); 
  
    QSize size2=groupTotal->sizeHint(); 
    setMinimumSize(size2.width(), size2.height());
    CkBlankImage->setCheckState(Qt::Unchecked);
    handleBlankImage();
}

//Atencion al boton de aceptar
void NewProjectDialog::handleAcceptButton() 
{
  m_accept=true;
    done(1);
}

//Atencion al boton de cancelar
void NewProjectDialog::handleCancelButton()
{
  m_accept=false;
    done(0);
}

//Atencion al boton de directorio del proyecto
void NewProjectDialog::handleBaseDirButton() 
{
    baseDirValue->setReadOnly(false);
    QString baseDir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                "/home/",
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);
    if(baseDir!="")
        {
        baseDirValue->setText(baseDir);  
        graphicValue->setText(baseDir);  
        }
    baseDirValue->setReadOnly(true);
    //QFileDialog::DontUseNativeDialog | QFileDialog::ReadOnly | QFileDialog::ShowDirsOnly
}
 
//Atencion al boton de busqueda del fichero grafico
void NewProjectDialog::handleGraphicButton()
{
//    QString graphicsFile = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/", tr("Graphics Files (*.jpg *.bmp *.png *.gif)"));
    QString graphicsFile = QFileDialog::getOpenFileName(this, tr("Open File"), baseDirValue->text(), tr("Graphics Files (*.jpg *.bmp *.png *.gif)"));
    if(graphicsFile !="")
    graphicValue->setText(graphicsFile);  
}

//Atencion al boton de actualizar parametros
void NewProjectDialog::handleParamsButton()
{
    ParamsDialog2 paramsDiag;
    char txt[200];
    Common tool;

    //se itera hasta que los datos introducidos sean adecuados
    while(true)
    {
    //Se muestra el valor actual

    tool.QlineEditToChar(graphicValue, txt);
    paramsDiag.graphicValue->setText(txt);
//    paramsDiag.graphicValue=graphicValue;
    paramsDiag.graphicValue->setReadOnly(true);
    paramsDiag.graphicButton->setVisible(false);
    
    sprintf(txt,"%d", m_params_p->velocity);
    paramsDiag.velocityValue->setText(txt);
    
    paramsDiag.arrowType->setCurrentIndex(m_params_p->arrowType);
   
    int R, G, B;
    m_params_p->arrowColor0.getRgb(&R, &G, &B);
    sprintf(txt,"%d, %d, %d", R, G, B);
    paramsDiag.arrowColor0Value->setText(txt);
    
    m_params_p->arrowColor1.getRgb(&R, &G, &B);
    sprintf(txt,"%d, %d, %d", R, G, B);
    paramsDiag.arrowColor1Value->setText(txt);
    
    m_params_p->pathColor.getRgb(&R, &G, &B);
    sprintf(txt,"%d, %d, %d", R, G, B);
    paramsDiag.pathColorValue->setText(txt);
    
    m_params_p->blockColor.getRgb(&R, &G, &B);
    sprintf(txt,"%d, %d, %d", R, G, B);
    paramsDiag.blockColorValue->setText(txt);
    
    paramsDiag.viewPaths->setCheckState(Qt::Unchecked);
    if(m_params_p->viewPaths)     paramsDiag.viewPaths->setCheckState(Qt::Checked);

    paramsDiag.viewPathsName->setCheckState(Qt::Unchecked);
    if(m_params_p->viewPathsName)     paramsDiag.viewPathsName->setCheckState(Qt::Checked);

    paramsDiag.viewIniValuePaths->setCheckState(Qt::Unchecked);
    if(m_params_p->viewIniValuePaths)     paramsDiag.viewIniValuePaths->setCheckState(Qt::Checked);

    paramsDiag.viewEndValuePaths->setCheckState(Qt::Unchecked);
    if(m_params_p->viewEndValuePaths)     paramsDiag.viewEndValuePaths->setCheckState(Qt::Checked);

    paramsDiag.viewNodeBlocksName->setCheckState(Qt::Unchecked);
    if(m_params_p->viewNodeBlocksName)     paramsDiag.viewNodeBlocksName->setCheckState(Qt::Checked);
    
    paramsDiag.viewPortBlocksName->setCheckState(Qt::Unchecked);
    if(m_params_p->viewPortBlocksName)     paramsDiag.viewPortBlocksName->setCheckState(Qt::Checked);
    
    paramsDiag.viewOthersBlocksName->setCheckState(Qt::Unchecked);
    if(m_params_p->viewOthersBlocksName)     paramsDiag.viewOthersBlocksName->setCheckState(Qt::Checked);
    
    paramsDiag.viewRectBlocks->setCheckState(Qt::Unchecked);
    if(m_params_p->viewRectBlocks)     paramsDiag.viewRectBlocks->setCheckState(Qt::Checked);
    
    /////////////////////////////////////
    //Visualizacion de la ventana
    paramsDiag.exec();
    ////////////////////////////////////
    if(paramsDiag.m_accept==0) return;
    
    //verificacion y captacion de valores si correctos
    Common tool;
    tool.QlineEditToChar(paramsDiag.graphicValue, (char*)m_params_p->graphicFile);

    m_params_p->velocity=paramsDiag.velocityValue->text().toInt();
    if (m_params_p->velocity <= 0) m_params_p->velocity = 1;
    else if (m_params_p->velocity > 100) m_params_p->velocity = 100;

    //se establece el tiempo entre evaluaciones de bloques (max=1s, min=10 ms)
    //m_params.deltaT_blocks = 1000 - (1000 - 10) * m_params.velocity / 100; //lineal
    m_params_p->deltaT_blocks = 1000 * exp(-m_params_p->velocity / 22.0); //exponencial

    int rgb[3];
    tool.QlineEditToChar(paramsDiag.arrowColor0Value, txt);
    if(tool.strToArray(txt, rgb, 3)!=3) continue;
    m_params_p->arrowColor0.setRgb(rgb[0], rgb[1], rgb[2]);
    
    tool.QlineEditToChar(paramsDiag.arrowColor1Value, txt);
    if(tool.strToArray(txt, rgb, 3)!=3) continue;
    m_params_p->arrowColor1.setRgb(rgb[0], rgb[1], rgb[2]);
    
    tool.QlineEditToChar(paramsDiag.pathColorValue, txt);
    if(tool.strToArray(txt, rgb, 3)!=3) continue;
    m_params_p->pathColor.setRgb(rgb[0], rgb[1], rgb[2]);
    
    tool.QlineEditToChar(paramsDiag.blockColorValue, txt);
    if(tool.strToArray(txt, rgb, 3)!=3) continue;
    m_params_p->blockColor.setRgb(rgb[0], rgb[1], rgb[2]);
    
    m_params_p->arrowType=paramsDiag.arrowType->currentIndex();
    
    if(paramsDiag.viewPaths->isChecked())           m_params_p->viewPaths=true;
    else m_params_p->viewPaths=false;
    
    if(paramsDiag.viewPathsName->isChecked())       m_params_p->viewPathsName=true;
    else m_params_p->viewPathsName=false;
    
    if(paramsDiag.viewIniValuePaths->isChecked())   m_params_p->viewIniValuePaths=true;
    else m_params_p->viewIniValuePaths=false;
    
    if(paramsDiag.viewEndValuePaths->isChecked())   m_params_p->viewEndValuePaths=true;
    else m_params_p->viewEndValuePaths=false;
    
    if(paramsDiag.viewNodeBlocksName->isChecked())  m_params_p->viewNodeBlocksName=true;
    else m_params_p->viewNodeBlocksName=false;

    if(paramsDiag.viewPortBlocksName->isChecked())  m_params_p->viewPortBlocksName=true;
    else m_params_p->viewPortBlocksName=false;

    if(paramsDiag.viewOthersBlocksName->isChecked()) m_params_p->viewOthersBlocksName=true;
    else m_params_p->viewOthersBlocksName=false;

    if(paramsDiag.viewRectBlocks->isChecked())      m_params_p->viewRectBlocks=true;
    else m_params_p->viewRectBlocks=false;
   
    break;
    }
}
 
//maneja el checkBox que permite generar ficheros graficos vacios
void NewProjectDialog::handleBlankImage()
{
        if(CkBlankImage->checkState()== Qt::Checked) 
            {
            widthLabel->setVisible(true); //anchura de la imagen
            heightLabel->setVisible(true);//altura
            widthValue->setVisible(true);
            heightValue->setVisible(true
            );
            groupD->setVisible(false);
            }
        else    
            {
            widthLabel->setVisible(false);
            heightLabel->setVisible(false);
            widthValue->setVisible(false);
            heightValue->setVisible(false);
            groupD->setVisible(true);
            }
    
}
