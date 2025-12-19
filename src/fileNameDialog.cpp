/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 *
 *    Dialogo para acceso a ficheros
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

#include "fileNameDialog.h"
#include "moc_fileNameDialog.cpp"
 
//Constructor
//Acepta una etiqueta para la ventana
FileNameDialog::FileNameDialog(char *title, char* baseDir)
  {
//  setMinimumSize(650, 150);
  m_accept=false;  
  m_baseDir_p = baseDir;

  char str[300];
  sprintf(str, "Project file");
  fileLabel = 	new QLabel( str);
  fileValue =	new QLineEdit(baseDir);
  fileButoon = 	new QPushButton("File", this);
  fileValue->setMinimumWidth(500);
 
    HlayoutFile= new QHBoxLayout();
	    HlayoutFile->addWidget(fileLabel);
	    HlayoutFile->addWidget(fileValue);
	    HlayoutFile->addWidget(fileButoon);
    groupFile= new QGroupBox();
	    groupFile->setLayout(HlayoutFile);

//   groupFile->setTitle(tr("- open project -"));

    fileButoon->setDefault(false);
    fileButoon->setAutoDefault(false);
    fileButoon->setDefault(false);
    fileButoon->setAutoDefault(false);
    
    connect(fileButoon, SIGNAL (released()),this, SLOT (handleBaseDirButton())); 
    
    //botones
    m_acceptButoon = new QPushButton("Accept", this);
    m_acceptButoon->setGeometry(QRect( QPoint(100, 100), QSize(200, 50) ));
    m_cancelButoon = new QPushButton("Cancel", this);
    m_cancelButoon->setGeometry(QRect( QPoint(100, 100), QSize(200, 50) ));

  HlayoutB = 	new QHBoxLayout();
	    HlayoutB->addWidget(m_acceptButoon);
	    HlayoutB->addWidget(m_cancelButoon);
	    groupB = new QGroupBox();
	    groupB->setLayout(HlayoutB);
	    groupB->setTitle(tr("- Action -"));

   VlayoutTotal = 	new QVBoxLayout(this);
   VlayoutTotal->addWidget(groupFile);
   VlayoutTotal->addWidget(groupB);

  groupTotal = 		new QGroupBox(this);
  groupTotal->setLayout(VlayoutTotal);
  
  groupTotal->setTitle(tr(title));
  m_acceptButoon->setDefault(true);
  m_acceptButoon->setAutoDefault(true);

  //conectamos eventos con funciones
  connect(m_acceptButoon, SIGNAL (released()),this, SLOT (handleAcceptButton()));    
  connect(m_cancelButoon, SIGNAL (released()),this, SLOT (handleCancelButton()));  
  
    QSize size2=groupTotal->sizeHint(); 
    setMinimumSize(size2.width(), size2.height());
  }  
  
//atencion a la pulsacion del boton aceptar
void FileNameDialog::handleAcceptButton()
{
  m_accept=true;
    done(1);
}

//atencion a la pulsacion del boton cancelar
void FileNameDialog::handleCancelButton() 
{
  m_accept=false;
    done(0);
}

//selecciona un fichero luego de navegar por la estructura de ficheros
//se visualizan los ficheros con extension *.prj *.asm *.txt *.png *.jpg *.tt
void FileNameDialog::handleBaseDirButton() 
{
//QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
//                                                "/home",
//                                                tr("Images (*.png *.xpm *.jpg)"));
QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), m_baseDir_p, 
                                                tr("Text Files (*.prj *.asm *.txt *.png *.jpg *.tt)"));
if(fileName!="")
  fileValue->setText(fileName);  
}

/*
void OpenProjectDialog::handleBaseDirButton() 
{
  QString file = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                "/",
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);
  if(file!="")
  fileValue->setText(file);  
//QFileDialog::DontUseNativeDialog | QFileDialog::ReadOnly | QFileDialog::ShowDirsOnly
}
*/  
