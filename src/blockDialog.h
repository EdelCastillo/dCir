/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 *
 *     Dialogos sobre bloques
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

#ifndef BLOCK_DIALOG_H
#define BLOCK_DIALOG_H

#include <QtWidgets>
#include "types.h"


class BlockDialog:public QDialog
{
  Q_OBJECT
   
  private slots:
    //Boton aceptar
    void handleAcceptButton(); 

    //Boton cancelar
    void handleCancelButton(); 

    //Boton eliminar
    void handleRemoveButton(); 

    //actuacion sobre el combo de biestables
    void handleCurrentIndexFFChanged(int); 

public slots:
    //actuacion sobre el combo principal (tipo de objeto) ante cambios 
    void handleCurrentIndexBaseChanged(int); 

    //actuacion sobre el combo de combinacionales
    void handleCurrentIndexCombiChanged(int);

    //actuacion sobre el check de camino de validacion
    void handleCurrentCkEnableChanged();

    //boton de acceso a ficheros *.tt
    void handleTTButton();
    
  public:
    //Constructor
    BlockDialog();
    
    QLabel      *nameLabel,
                *typeLabel,
                *operationLabel,
                *pathInLabel,
                *pathOutLabel,
                *enableLabel,
                *selectLabel,
                *trueTableLabel,
                *delBlockLabel,
                *actionLabel,
                *delayLabel,
                *initLabel,
                *FFLabel,
                *TTfileLabel,
                *infoMemLabel,
                *infoMuxLabel,
                *logicCmpLabel;
    QLineEdit   *nameValue,
                *pathInValue,
                *pathOutValue,
                *enableValue,
                *selectValue,
                *trueTableValue,
                *delBlockValue,
                *delayValue,
                *initValue,
                *TTfileValue;
                
  QCheckBox     //*CkTrueTable,
                *CkEnableFF;
  QComboBox     *Ctype,
                *Coperation,
                *CflipFlop,
                *ClogicCmp;
    QPushButton 	
                *TTButton;
            
    QHBoxLayout 
		*HlayoutA,
		*HlayoutB,
		*HlayoutC,
		*HlayoutD,
		*HlayoutE,
		*HlayoutF,
		*HlayoutG,
		*HlayoutH,
        *HlayoutButton;
    QGroupBox 
		*groupA,
		*groupB,
		*groupC,
		*groupD,
		*groupE,
		*groupF,
		*groupG,
		*groupH,
		*groupTotal,
        *groupButton;
  QVBoxLayout 
		*VlayoutA,
		*VlayoutB;
    QPushButton 	
        *m_acceptButton,
        *m_cancelButton,
        *m_removeButton;
    
    int m_result;
    bool m_flagOper;
};

#endif
