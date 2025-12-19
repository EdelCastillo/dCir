/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 *
 *    Dialogo para establecimiento de parametros de operacian
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

#ifndef PARAMS_DIALOG_H
#define PARAMS_DIALOG_H

#include "types.h"

//Clase de dialogo para establecimiento de parametros de operacian
class ParamsDialog2:public QDialog
{
  Q_OBJECT
  
private slots:
    //Atencion al boton de aceptar
    void handleAcceptButton();

    //Atencion al boton de cancelar
    void handleCancelButton();

    //Atencion al boton de conseguir el fichero grafico
    void handleGraphicButton();
 
public:
    //Constructor
    ParamsDialog2();
    
    QLabel      *graphicLabel,
                *velocityLabel,
                *arrowTypeLabel,
                *arrowColor0Label,
                *arrowColor1Label,
                *pathColorLabel,
                *blockColorLabel,
                *deltaTLabel;
                
    QLineEdit   *graphicValue,
                *velocityValue,
                *arrowColor0Value,
                *arrowColor1Value,
                *pathColorValue,
                *blockColorValue;
                                
  QCheckBox     *viewPaths,
                *viewPathsName,
                *viewIniValuePaths,
                *viewEndValuePaths,
                *viewNodeBlocksName,
                *viewPortBlocksName,
                *viewOthersBlocksName,
                *viewRectBlocks,
                *viewNames;
                
    QComboBox   *arrowType,
                *deltaTValue;
            
    QPushButton 	
        *graphicButton;
        
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
};
#endif
