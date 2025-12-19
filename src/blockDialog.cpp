/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 *
 *     Dialogos sobre bloques
 * 
 *      This program is free software: you can redistribute it and/or modify
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

#include "blockDialog.h"
#include "moc_blockDialog.cpp"
 
//Constructor
BlockDialog::BlockDialog()
{
    m_result=0;
    m_flagOper=true;
    
    actionLabel =       new QLabel( "action");
    nameLabel =         new QLabel( "number");
    typeLabel =         new QLabel( "type");
    operationLabel =    new QLabel( "function");
    pathInLabel =       new QLabel( "inPaths");
    pathOutLabel =      new QLabel( "outPaths");
    enableLabel =       new QLabel( "enablePath");
    selectLabel =       new QLabel( "selectPaths");
    delayLabel =        new QLabel( "delay(s)");
    initLabel =         new QLabel( "initValue");
    FFLabel =           new QLabel( "bistable");
    TTfileLabel =       new QLabel( "trueTableFile");
    infoMemLabel =      new QLabel( "(addr, dataIn, ctr)");
    infoMuxLabel =      new QLabel( "(0, 1, ...)");
     
    nameValue =         new QLineEdit;
    pathInValue =       new QLineEdit;
    pathOutValue =      new QLineEdit;
    enableValue =       new QLineEdit;
    selectValue =       new QLineEdit;
    trueTableValue =    new QLineEdit;
    delBlockValue =     new QLineEdit;
    delayValue =        new QLineEdit;
    initValue =         new QLineEdit;
    TTfileValue =       new QLineEdit;
    
    //nameValue->minimumSizeHint();
    int width=23;
    nameValue   ->setMinimumHeight(width);
    pathInValue ->setMinimumHeight(width);
    pathOutValue->setMinimumHeight(width);
    enableValue ->setMinimumHeight(width);
    selectValue ->setMinimumHeight(width);
    trueTableValue->setMinimumHeight(width);
    delBlockValue->setMinimumHeight(width);
    delayValue  ->setMinimumHeight(width);
    initValue   ->setMinimumHeight(width);
    TTfileValue ->setMinimumHeight(width);

//    CkTrueTable = new QCheckBox("trueTable", this);
    CkEnableFF =  new QCheckBox("alwaysEnable",  this);
    
    Ctype= new QComboBox;
    Ctype->addItem(tr("combinational"));
    Ctype->addItem(tr("sequential"));
    Ctype->addItem(tr("node"));
    Ctype->addItem(tr("portIn"));
    Ctype->addItem(tr("portOut"));
    Ctype->addItem(tr("tab"));
    Ctype->addItem(tr("memory"));
    Ctype->setMinimumHeight(width);
/*    
    CinitValue= new QComboBox;
    CinitValue->addItem(tr("0"));
    CinitValue->addItem(tr("1"));
    CinitValue->addItem(tr("X"));
*/    
    CflipFlop= new QComboBox;
    CflipFlop->addItem(tr("FFD"));
    CflipFlop->addItem(tr("FFT"));
    CflipFlop->addItem(tr("FFJK"));
    CflipFlop->addItem(tr("latchD"));
    CflipFlop->addItem(tr("FSM"));
    CflipFlop->setMinimumHeight(width);

    Coperation= new QComboBox;
    Coperation->addItem(tr("not"));
    Coperation->addItem(tr("and"));
    Coperation->addItem(tr("or"));
    Coperation->addItem(tr("xor"));
    Coperation->addItem(tr("nand"));
    Coperation->addItem(tr("nor"));
    Coperation->addItem(tr("xnor"));
    Coperation->addItem(tr("add"));
    Coperation->addItem(tr("mux"));
     Coperation->addItem(tr("cmp"));
    Coperation->addItem(tr("others"));
    Coperation->addItem(tr("dmux"));
    Coperation->addItem(tr("decod"));
    Coperation->addItem(tr("decodn"));
    Coperation->setMinimumHeight(width);
    
    ClogicCmp= new QComboBox;
    ClogicCmp->addItem(tr("<"));
    ClogicCmp->addItem(tr("="));
    ClogicCmp->addItem(tr(">"));

    m_acceptButton = new QPushButton("Accept", this);
//    m_acceptButton->setGeometry(QRect( QPoint(100, 100), QSize(200, 50) ));
    m_cancelButton = new QPushButton("Cancel", this);
//    m_cancelButton->setGeometry(QRect( QPoint(100, 100), QSize(200, 50) ));
    m_removeButton = new QPushButton("Remove", this);
//    m_removeButton->setGeometry(QRect( QPoint(100, 100), QSize(200, 50) ));
    TTButton = new QPushButton("search", this);

    m_acceptButton->setMinimumHeight(width);
    m_cancelButton->setMinimumHeight(width);
    m_removeButton->setMinimumHeight(width);

//    nameValue->setMaximumWidth(50);
//    Ctype->setMinimumWidth(100);
    initValue   ->setMaximumWidth(50);
    delayValue  ->setMaximumWidth(50);
    pathInValue->setMinimumWidth(140);
    pathOutValue->setMinimumWidth(140);
    pathInValue ->setMaximumWidth(150);
    pathOutValue->setMaximumWidth(150);
    enableValue ->setMaximumWidth(100);
    selectValue ->setMaximumWidth(100);
    TTfileValue ->setMaximumWidth(300);
    nameValue   ->setMaximumWidth(50);
//    initLabel->setMaximumWidth(50);
    
    HlayoutA = new QHBoxLayout(this);
	    HlayoutA->addWidget(nameLabel);
	    HlayoutA->addWidget(nameValue);
	    HlayoutA->addWidget(typeLabel);
	    HlayoutA->addWidget(Ctype);
        groupA = new QGroupBox(this);
	    groupA->setLayout(HlayoutA);
        
        
    HlayoutB = new QHBoxLayout(this);
	    HlayoutB->addWidget(pathInLabel);
	    HlayoutB->addWidget(pathInValue);
	    HlayoutB->addWidget(infoMemLabel);
        HlayoutB->addWidget(infoMuxLabel);
        HlayoutB->addWidget(pathOutLabel);
	    HlayoutB->addWidget(pathOutValue);
	    groupB = new QGroupBox(this);
	    groupB->setLayout(HlayoutB);
                
    HlayoutC = new QHBoxLayout(this);
	    HlayoutC->addWidget(operationLabel);
	    HlayoutC->addWidget(Coperation);
	    groupC = new QGroupBox(this);
	    groupC->setLayout(HlayoutC);
        
    HlayoutF = new QHBoxLayout(this);
	    HlayoutF->addWidget(FFLabel);
	    HlayoutF->addWidget(CflipFlop);
	    groupF = new QGroupBox(this);
	    groupF->setLayout(HlayoutF);
        
        
    HlayoutE = new QHBoxLayout(this);
	    HlayoutE->addWidget(initLabel);
	    HlayoutE->addWidget(initValue);
	    groupE = new QGroupBox(this);
	    groupE->setLayout(HlayoutE);
    
    HlayoutG = new QHBoxLayout(this);
	    HlayoutG->addWidget(nameLabel);
	    HlayoutG->addWidget(nameValue);
	    HlayoutG->addWidget(typeLabel);
	    HlayoutG->addWidget(Ctype);
	    HlayoutG->addWidget(operationLabel);
	    HlayoutG->addWidget(Coperation);
	    HlayoutG->addWidget(initLabel);
	    HlayoutG->addWidget(initValue);
	    HlayoutG->addWidget(FFLabel);
	    HlayoutG->addWidget(CflipFlop);
        HlayoutG->addWidget(ClogicCmp);
//	    HlayoutG->addWidget(groupA);
//	    HlayoutG->addWidget(groupC);
//	    HlayoutG->addWidget(groupE);
//	    HlayoutG->addWidget(groupF);
	    groupG = new QGroupBox(this);
	    groupG->setLayout(HlayoutG);
    
    HlayoutH = new QHBoxLayout(this);
	    HlayoutH->addWidget(enableLabel);
	    HlayoutH->addWidget(enableValue);
	    HlayoutH->addWidget(selectLabel);
	    HlayoutH->addWidget(selectValue);
        HlayoutH->addWidget(CkEnableFF);
	    groupH = new QGroupBox(this);
	    groupH->setLayout(HlayoutH);
        
    HlayoutD = new QHBoxLayout(this);
//	    HlayoutD->addWidget(CkTrueTable);
	    HlayoutD->addWidget(TTfileLabel);
	    HlayoutD->addWidget(TTfileValue);
	    HlayoutD->addWidget(TTButton);
	    HlayoutD->addWidget(delayLabel);
	    HlayoutD->addWidget(delayValue);
	    groupD = new QGroupBox(this);
	    groupD->setLayout(HlayoutD);
    
    
    HlayoutButton = new QHBoxLayout(this);
    HlayoutButton->addWidget(m_acceptButton);
    HlayoutButton->addWidget(m_cancelButton);
    HlayoutButton->addWidget(m_removeButton);
        groupButton = new QGroupBox(this);
        groupButton->setLayout(HlayoutButton);
 
    VlayoutA = 	new QVBoxLayout(this);
//    VlayoutA->addWidget(groupA);
    VlayoutA->addWidget(groupG);
    VlayoutA->addWidget(groupB);
    VlayoutA->addWidget(groupH);
    VlayoutA->addWidget(groupD);
    VlayoutA->addWidget(groupButton);
        groupTotal = new QGroupBox(this);
        groupTotal->setLayout(VlayoutA);
    
   groupTotal->setTitle(tr("- BLOCK -"));
  
    connect(m_acceptButton, SIGNAL (released()),                this, SLOT (handleAcceptButton()));    
    connect(m_cancelButton, SIGNAL (released()),                this, SLOT (handleCancelButton()));  
    connect(m_removeButton, SIGNAL (released()),                this, SLOT (handleRemoveButton()));  
    connect(TTButton,       SIGNAL (released()),                this, SLOT (handleTTButton()));
    connect(Ctype,          SIGNAL (currentIndexChanged(int)),  this, SLOT (handleCurrentIndexBaseChanged(int)));  
    connect(Coperation,     SIGNAL (currentIndexChanged(int)),  this, SLOT (handleCurrentIndexCombiChanged(int)));  
    connect(CflipFlop,      SIGNAL (currentIndexChanged(int)),  this, SLOT (handleCurrentIndexFFChanged(int)));  
    connect(CkEnableFF,     SIGNAL (stateChanged(int)),         this, SLOT (handleCurrentCkEnableChanged()));
//    connect(CkTrueTable,    SIGNAL (stateChanged(int)),         this, SLOT (handleCkTrueTableChanged()));

//    HlayoutA->setAlignment(actionLabel,     Qt::AlignRight); //alinea a la dereccha  el campo nameLabel
    HlayoutG->setAlignment(nameLabel,       Qt::AlignRight); //alinea a la dereccha  el campo nameLabel
    HlayoutG->setAlignment(typeLabel,       Qt::AlignRight); //alinea a la dereccha  el campo nameLabel
    HlayoutG->setAlignment(operationLabel,  Qt::AlignRight); //alinea a la dereccha  el campo nameLabel
    HlayoutD->setAlignment(delayLabel,      Qt::AlignRight); //alinea a la dereccha  el campo nameLabel
    HlayoutB->setAlignment(pathInLabel,     Qt::AlignRight); //alinea a la dereccha  el campo nameLabel
    HlayoutB->setAlignment(pathOutLabel,    Qt::AlignRight); //alinea a la dereccha  el campo nameLabel
    HlayoutH->setAlignment(enableLabel,     Qt::AlignRight); //alinea a la dereccha  el campo nameLabel
    HlayoutH->setAlignment(selectLabel,     Qt::AlignRight); //alinea a la dereccha  el campo nameLabel
    
    operationLabel->setVisible(true);   Coperation->setVisible(true); //gC
    initLabel->setVisible(false);       initValue->setVisible(false); //gE
    FFLabel->setVisible(false);         CflipFlop->setVisible(false); //gF
    //groupC->setVisible(true);   
    //groupE->setVisible(false);    
    //groupF->setVisible(false);    
    
    QSize size=groupTotal->sizeHint(); //adeúa el tamaño de la ventana al contenido
    setMinimumSize(size);
//    updateGeometry();

}

//Botón aceptar
void BlockDialog::handleAcceptButton()
{
  m_result=0x01;
    done(1);
}

//Botón cancelar
void BlockDialog::handleCancelButton()
{
  m_result=0x04;
    done(0);
}

//Botón eliminar
void BlockDialog::handleRemoveButton()
{
  m_result=0x03;
    done(1);
}

//actuación sobre el combo principal (tipo de objeto) ante cambios 
void BlockDialog::handleCurrentIndexBaseChanged(int index)
{
    //valores por defecto
    pathInLabel->setVisible(true);
    pathInValue->setVisible(true);
    pathOutLabel->setVisible(true);
    pathOutValue->setVisible(true);   
    selectLabel->setVisible(false);
    selectValue->setVisible(false);
    enableLabel->setVisible(false);
    enableValue->setVisible(false);
    CkEnableFF->setVisible(false);
    TTfileLabel->setVisible(false);
    TTfileValue->setVisible(false);
    TTButton->setVisible(false);
    infoMemLabel->setVisible(false);
    infoMuxLabel->setVisible(false);
    groupH->setVisible(false);
    ClogicCmp->setVisible(false);
    
    //Combinacional
    if(index== (char)Type::COMBI)
        {
        operationLabel->setVisible(true);   Coperation->setVisible(true); //gC
        initLabel->setVisible(false);       initValue->setVisible(false); //gE
        FFLabel->setVisible(false);         CflipFlop->setVisible(false); //gF    
        }
    
    //combinacional multiplexor
    if(index== (char)Type::COMBI && (
        Coperation->currentIndex() == (char)Oper::MUX ||
        Coperation->currentIndex() == (char)Oper::DMUX || 
        Coperation->currentIndex() == (char)Oper::DECOD ||
        Coperation->currentIndex() == (char)Oper::DECODN))
        {
        groupH->setVisible(true);
        selectLabel->setVisible(true);
        selectValue->setVisible(true);
        infoMuxLabel->setVisible(true);
        }

    if (index == (char)Type::COMBI && (
         Coperation->currentIndex() == (char)Oper::DMUX ||
        Coperation->currentIndex() == (char)Oper::DECOD ||
        Coperation->currentIndex() == (char)Oper::DECODN))
    {
        pathInLabel->setVisible(false);
        pathInValue->setVisible(false);

    }
    //Combinacional comparador
    if(index== (char)Type::COMBI && Coperation->currentIndex()== (char)Oper::CMP)
        {
        ClogicCmp->setVisible(true);
        }
        
    //(Combinacional y (memoria u otros)) o (secuencial y FSM)
    if((index== (char)Type::COMBI && (Coperation->currentIndex()== (char)Type::MEMORY || Coperation->currentIndex()== (char)Oper::OTHERS)) ||
        (index== (char)Type::SEQ && CflipFlop->currentIndex()== (char)FF::FSM))
        {
        TTfileLabel->setVisible(true);
        TTfileValue->setVisible(true);
        TTButton->setVisible(true);
        }

    //Combinacional && memoria
    if(index== (char)Type::COMBI && Coperation->currentIndex()== (char)Type::MEMORY)
         infoMemLabel->setVisible(true);
     
    //Puerto de entrada
    else if(index== (char)Type::IN_PORT)
        {
        operationLabel->setVisible(false);   Coperation->setVisible(false); //gC
        initLabel->setVisible(true);        initValue->setVisible(true); //gE
        FFLabel->setVisible(false);           CflipFlop->setVisible(false); //gF
        //groupC->setVisible(false);    
        //groupF->setVisible(false);    
        //groupE->setVisible(true);  
        pathInLabel->setVisible(false);
        pathInValue->setVisible(false);
        pathOutLabel->setVisible(true);
        pathOutValue->setVisible(true);    
        }

    //puerto de salida
    else if(index== (char)Type::OUT_PORT)
        {
        operationLabel->setVisible(false);   Coperation->setVisible(false); //gC
        initLabel->setVisible(false);        initValue->setVisible(false); //gE
        FFLabel->setVisible(false);           CflipFlop->setVisible(false); //gF
        //groupC->setVisible(false);    
        //groupF->setVisible(false);    
        //groupE->setVisible(false);  
        pathInLabel->setVisible(true);
        pathInValue->setVisible(true);
        pathOutLabel->setVisible(false);
        pathOutValue->setVisible(false);    
        }

    //nodo o pestaña
    else if(index== (char)Type::NODE || index== (char)Type::TAB)
        {
        operationLabel->setVisible(false);   Coperation->setVisible(false); //gC
        initLabel->setVisible(false);        initValue->setVisible(false); //gE
        FFLabel->setVisible(false);           CflipFlop->setVisible(false); //gF
        //groupC->setVisible(false);    
        //groupE->setVisible(false);    
        //groupF->setVisible(false);    
        }

    //secuencial
    else if(index== (char)Type::SEQ)
        {
        groupH->setVisible(true);
        operationLabel->setVisible(false);   Coperation->setVisible(false); //gC
        initLabel->setVisible(true);        initValue->setVisible(true); //gE
        FFLabel->setVisible(true);           CflipFlop->setVisible(true); //gF
        //groupC->setVisible(false);    
        //groupE->setVisible(true);    
        //groupF->setVisible(true); 
        enableLabel->setVisible(true);
        enableValue->setVisible(true);   
        CkEnableFF->setVisible(true);
        if(CkEnableFF->checkState()==Qt::Checked) 
            {
            enableLabel->setVisible(false);
            enableValue->setVisible(false);   
            }
        }

    //Memoria
    else if(index== (char)Type::MEMORY)
        {
        TTfileLabel->setVisible(true);
        TTfileValue->setVisible(true);
        TTButton->setVisible(true);
        infoMemLabel->setVisible(true);
        operationLabel->setVisible(false);   Coperation->setVisible(false); //gC
        initLabel->setVisible(false);        initValue->setVisible(false); //gE
        FFLabel->setVisible(false);          CflipFlop->setVisible(false); //gF
        //groupC->setVisible(false);    
        }

    //cualquier otro caso no considerado
    else
        {
        operationLabel->setVisible(true);   Coperation->setVisible(true); //gC
        initLabel->setVisible(false);       initValue->setVisible(false); //gE
        FFLabel->setVisible(false);         CflipFlop->setVisible(false); //gF
        //groupE->setVisible(false);    
        //groupF->setVisible(false);    
        //groupC->setVisible(true);    
        }  

    //ajuste de tamaño de ventana
    QSize size = groupTotal->sizeHint();
    resize(size); //adecúa el tamaño de la ventana
}

//actuación sobre el check de camino de validación
void BlockDialog::handleCurrentCkEnableChanged()
{
        if(CkEnableFF->checkState()== Qt::Checked) 
            {
            enableLabel->setVisible(false);
            enableValue->setVisible(false);   
            }
        else    
            {
            enableLabel->setVisible(true);
            enableValue->setVisible(true);   
            }
}

//actuación sobre el combo de combinacionales
void BlockDialog::handleCurrentIndexCombiChanged(int index)
{
    //por defecto
    groupH->setVisible(false);
    infoMemLabel->setVisible(false);
    infoMuxLabel->setVisible(false);
    ClogicCmp->setVisible(false);

    //multiplexor
    if(index == (char)Oper::MUX   || index == (char)Oper::DMUX ||
       index == (char)Oper::DECOD || index == (char)Oper::DECODN)
        {
        groupH->setVisible(true);
        selectLabel->setVisible(true);
        selectValue->setVisible(true);        
        infoMuxLabel->setVisible(true);
        }
    else
        {
        selectLabel->setVisible(false);
        selectValue->setVisible(false);        
        }

    if (index == (char)Oper::DECOD || index == (char)Oper::DECODN)
    {
        pathInLabel->setVisible(false);
        pathInValue->setVisible(false);
    }
    else
    {
        pathInLabel->setVisible(true);
        pathInValue->setVisible(true);
    }
    //Comparador
    if(index== (char)Oper::CMP)
        {
        ClogicCmp->setVisible(true);
        }

    //Otros
    if(index== (char)Oper::OTHERS)
        {
        TTfileLabel->setVisible(true);
        TTfileValue->setVisible(true);
        TTButton->setVisible(true);
        }
    else
        {
        TTfileLabel->setVisible(false);
        TTfileValue->setVisible(false);
        TTButton->setVisible(false);
        }
}

//actuación sobre el combo de biestables
void BlockDialog::handleCurrentIndexFFChanged(int index)
{
    //latches
    if(index== (char)FF::LATCH)  //new
        {
        enableLabel->setVisible(true);
        enableValue->setVisible(true);
        CkEnableFF->setVisible(false);
        }
    else        
         {
        CkEnableFF->setVisible(true);
        handleCurrentCkEnableChanged();             
        }
    //Máquina de estados finita
    if(index== (char)FF::FSM)
        {
        TTfileLabel->setVisible(true);
        TTfileValue->setVisible(true);
        TTButton->setVisible(true);
        }
    else
        {
        TTfileLabel->setVisible(false);
        TTfileValue->setVisible(false);
        TTButton->setVisible(false);
        }
}

//botón de acceso a ficheros *.tt
void BlockDialog::handleTTButton() 
{
    QString graphicsFile = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/", tr("Text Files (*.tt *.txt)"));
    if(graphicsFile !="")
    TTfileValue->setText(graphicsFile);  
}

