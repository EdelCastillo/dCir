/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 *
 *    Diálogo para info de direcciones de memoria
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

#ifndef ADDR_DIALOG_H
#define ADDR_DIALOG_H

#include <QtWidgets>

class AddrDialog : public QDialog 
{
    Q_OBJECT

private slots:
  void handleAcceptButton();
  void handleCancelButton();
    
public:
    AddrDialog();
bool		m_accept;
    QLabel  
	    *addr1Label,
	    *addr2Label;
    QLineEdit 
	    *addr1Value,
	    *addr2Value;
    QHBoxLayout 
	    *HlayoutA,
	    *HlayoutB;
    QVBoxLayout 
	    *VlayoutA;
    QGroupBox 
	    *groupA,
 	    *groupB,
	    *Vgroup;
   
  QGroupBox 	*groupTotal;
  QVBoxLayout 	*VlayoutTotal;
  QPushButton 	*m_acceptButoon,
		*m_cancelButoon;
};
#endif
  
  
