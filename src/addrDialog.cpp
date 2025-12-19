/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 *
 *    Diálogo para info de direcciones de memoria
 
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
 
#include "addrDialog.h"
#include "moc_addrDialog.cpp"
 
AddrDialog::AddrDialog()
  {
//  setMinimumSize(650, 150);
  m_accept=false;  
  addr1Label = 	new QLabel("addrInit");
  addr2Label = 	new QLabel("addrEnd");
    
  addr1Value =	new QLineEdit;
  addr2Value =	new QLineEdit;
  
    HlayoutA= new QHBoxLayout();
	    HlayoutA->addWidget(addr1Label);
	    HlayoutA->addWidget(addr1Value);
	    HlayoutA->addWidget(addr2Label);
	    HlayoutA->addWidget(addr2Value);
    groupA= new QGroupBox();
	    groupA->setLayout(HlayoutA);

//   groupFile->setTitle(tr("- open project -"));
    
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
   VlayoutTotal->addWidget(groupA);
   VlayoutTotal->addWidget(groupB);

  groupTotal = 		new QGroupBox(this);
  groupTotal->setLayout(VlayoutTotal);

  connect(m_acceptButoon, SIGNAL (released()),this, SLOT (handleAcceptButton()));    
  connect(m_cancelButoon, SIGNAL (released()),this, SLOT (handleCancelButton()));  
  
    QSize size2=groupTotal->sizeHint(); 
    setMinimumSize(size2.width(), size2.height());
  }  
  
void AddrDialog::handleAcceptButton() 
{
  m_accept=true;
    done(1);
}

void AddrDialog::handleCancelButton() 
{
  m_accept=false;
    done(0);
}

