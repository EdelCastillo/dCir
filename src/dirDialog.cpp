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

#include "dirDialog.h"
#include "moc_dirDialog.cpp"
 
//Constructor
//Acepta una etiqueta para la ventana
DirDialog::DirDialog(char *title, char *baseDir)
  {
//  setMinimumSize(650, 150);
  m_accept=false;  
  m_baseDir_p = baseDir;

  char str[300];
  sprintf(str, "Project directory");
  dirLabel = 	new QLabel( str);
  dirValue =	new QLineEdit;
  dirButoon = 	new QPushButton("Directory", this);
  dirValue->setMinimumWidth(500);
 
    HlayoutDir= new QHBoxLayout();
	    HlayoutDir->addWidget(dirLabel);
	    HlayoutDir->addWidget(dirValue);
	    HlayoutDir->addWidget(dirButoon);
    groupDir= new QGroupBox();
	    groupDir->setLayout(HlayoutDir);

//   groupFile->setTitle(tr("- open project -"));

    dirButoon->setDefault(false);
    dirButoon->setAutoDefault(false);
    dirButoon->setDefault(false);
    dirButoon->setAutoDefault(false);
    
    connect(dirButoon, SIGNAL (released()),this, SLOT (handleBaseDirButton())); 
    
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
   VlayoutTotal->addWidget(groupDir);
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
void DirDialog::handleAcceptButton()
{
  m_accept=true;
    done(1);
}

//atencion a la pulsacion del boton cancelar
void DirDialog::handleCancelButton() 
{
  m_accept=false;
    done(0);
}

//selecciona un directorio luego de navegar por la estructura de ficheros
void DirDialog::handleBaseDirButton() 
{
  QString dir = QFileDialog::getExistingDirectory(0, tr("Open Directory"),
                                                m_baseDir_p,
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);
  if(dir!="")
  dirValue->setText(dir);  
//QFileDialog::DontUseNativeDialog | QFileDialog::ReadOnly | QFileDialog::ShowDirsOnly
}
 
