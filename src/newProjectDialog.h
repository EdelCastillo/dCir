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

#ifndef NEW_PROJECT_DIALOG_H
#define NEW_PROJECT_DIALOG_H

#include "types.h"
#include "common.h"
#include "paramsDialog.h"

//Dialogo para la creacion de un nuevo proyecto
class NewProjectDialog:public QDialog
{
  Q_OBJECT
  
private slots:
    //Atencion al boton de aceptar
    void handleAcceptButton();

    //Atencion al boton de cancelar
    void handleCancelButton();

    //Atencion al boton de directorio del proyecto
    void handleBaseDirButton();

    //Atencion al boton de busqueda del fichero grafico
    void handleGraphicButton();

    //Atencion al boton de actualizar parametros
    void handleParamsButton();
    
    void handleBlankImage();
    
public:
    //Constructor
    //acepta un puntero a la estructura PARAMS dado que la puede actualizar
    NewProjectDialog(PARAMS *params_p);
    
    QLabel      *baseDirLabel,
                *nameLabel,
                *graphicLabel,
                *widthLabel,
                *heightLabel;
    QLineEdit   *baseDirValue,
                *nameValue,
                *graphicValue,
                *widthValue,
                *heightValue;
    QPushButton 	
	    *baseDirButton,
        *graphicButton,
        *paramsButton;
                
    QCheckBox   *CkTrueTable,
                *CkBlankImage;
            
    QHBoxLayout 
		*HlayoutA,
		*HlayoutB,
		*HlayoutC,
		*HlayoutD,
		*HlayoutE,
		*HlayoutF,
		*HlayoutG,
        *HlayoutButton;
    QGroupBox 
		*groupA,
		*groupB,
		*groupC,
		*groupD,
		*groupE,
		*groupF,
		*groupG,
		*groupTotal,
        *groupButton;
  QVBoxLayout 
		*VlayoutTotal,
		*VlayoutB;
    QPushButton 	
        *m_acceptButton,
        *m_cancelButton,
        *m_removeButton;
    int m_accept;
    PARAMS *m_params_p;
    
};

#endif
