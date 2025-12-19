/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 *
 *    Dialogo para la gestion de caminos 
 *    Dialogo para la eleccion de caminos o bloques
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

#ifndef PATH_DIALOG_H
#define PATH_DIALOG_H

#include <QtWidgets>

//Clase para la presentacion y captacion de informacion sobre caminos
//
class PathDialog:public QDialog
{
  Q_OBJECT
  
  private slots:
    //Atencion al boton de inicio de camino
    void handleStartButton();

    //Atencion al boton de fin de camino
    void handleFinishButton();

    //Atencion al boton de eliminacion de camino
    void handleDeleteButton();

    //Atencion al boton de cancelar operacion
    void handleCancelButton();

    //Atencion al boton de actualizar los bits del camino
    void handleUpdateButton();
    
  public:
    PathDialog();
    
    QLabel      *pathLabel,
                *bitsLabel,
                *delPathLabel,
                *valueLabel;
    QLineEdit   *pathValue,
                *bitsValue,
                *delPathValue,
                *valueValue;
                
    QHBoxLayout 
		*HlayoutA,
		*HlayoutB,
		*HlayoutC,
		*HlayoutD,
        *HlayoutButton;
    QGroupBox 
		*groupA,
		*groupB,
		*groupC,
		*groupD,
		*groupTotal,
        *groupButton;
  QVBoxLayout 
		*VlayoutA,
		*VlayoutB;
    QPushButton 	
	    *m_deleteButton,
        *m_updateButton,
	    *m_finishButton,
        *m_startButton,
        *m_cancelButton;
    
    int m_result;
};

////////////////////////////////////////////////////////////////////
//Clase para la atencion a la pulsacion del boton derecho del raton
//Ventana para la eleccion de caminos o bloques

class BottonRightDialog:public QDialog
{
  Q_OBJECT
  
  private slots:
    //Atencion al boton de bloques
    void handleBlocksButton();

    //Atencion al boton de caminos
    void handlePathsButton();

    //Atencion al boton de cancelar
    void handleCancelButton();


  public:
    BottonRightDialog();
    
    QHBoxLayout 
		*HlayoutButton;
    QGroupBox 
		*groupButton;
    QVBoxLayout 
		*VlayoutA;
        
    
    QPushButton 	
        *m_cancelButton,
	    *m_blocksButton,
	    *m_pathsButton;
    
    int m_result;
};
#endif
