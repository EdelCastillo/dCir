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

#ifndef DIR_DIALOG_H
#define DIR_DIALOG_H


#include <QtWidgets>
//#include <QDialog>
//#include <QtGui>
//#include <QPushButton> 

class DirDialog : public QDialog 
{
    Q_OBJECT

private slots:
	//atencion a la pulsacion del boton aceptar
	void handleAcceptButton();

	//atencion a la pulsacion del boton aceptar
	void handleCancelButton();

	//atencion a la pulsacion del boton de navegar por la estructura de ficheros
	void handleBaseDirButton();

    
public:
	//Constructor
	//Acepta una etiqueta para la ventana
	DirDialog(char *title, char *base_dir);
bool			m_accept;
    QLabel		*dirLabel;
    QLineEdit 
				*dirValue;
    QPushButton 	
				*dirButoon;
    QHBoxLayout 
				*HlayoutDir,
				*HlayoutInfoDir;
    QVBoxLayout 
				*VlayoutDir;
    QGroupBox 
				*groupDir,
 				*groupInfoDir,
				*VgroupDir;
   
  QHBoxLayout 
				*HlayoutB;
  QGroupBox 
				*groupB;
  QGroupBox 	*groupTotal;
  QVBoxLayout 	*VlayoutTotal;
  QPushButton 	*m_acceptButoon,
				*m_cancelButoon;
  char			*m_baseDir_p;       //puntero a directorio base
};
#endif
  
  
