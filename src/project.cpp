/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 * 
 *   guarda a un fichero el proyecto actual 
 *   carga un proyecto desde un fichero *.prj
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

/*
se hace uso de un procedimiento particular en formato texto
existe tres tipos de información: general, de caminos y de bloques

INFORMACIÓN GENERAL
===================
La información general la componen las palabras clase siguientes:

PROJECT_DIR             = directorio del projecto
GRAPHICS_FILE           = path desde la raiz al fichero .png que contiene el gráfico digital de base
VELOCITY                = velocidad de avance de las flechas por los caminos (pixeles/s)
ARROW_TYPE              = type de flecha (0->grande; 1->mediana; 2->pequeña)
ARROW_COLOR0_RGB        = color para el valor lógico '0' en formato RGB
ARROW_COLOR1_RGB        = color para el valor lógico '1' en formato RGB
PATH_COLOR_RGB          = color para el camino (líneas discontinuas) en formato RGB
BLOCK_COLOR_RGB         = color para el rectángulo que delimita la zona sensible al ratón en un bloque
VIEW_PATHS              = visualiza los caminos
VIEW_PATHS_NAME         = visualizalos nombres de los caminos
VIEW_INI_VALUE_PATHS    = visualiza los valores iniciales de los caminos
VIEW_END_VALUE_PATHS    = visualiza los valores finales de los caminos
VIEW_NODE_BLOCKS_NAME   = visualiza el nombre sobre los nodos (nudos de caminos)
VIEW_PORT_BLOCKS_NAME   = visualiza el nombre sobre los puertos de entrada y salida
VIEW_OTHERS_BLOCKS_NAME = visualiza el nombre sobre el resto de bloques
VIEW_RECT_BLOCKS        = visualiza los rectángulos sobre los bloques (no afecta a los puertos)

CAMINOS
=======
Cada camino está conformado por una sucesión de puntos.
La información de caminos tiene la expresión siguiente:

PATH:ref1:POINT:ref2:XY = coordX coordY
    PATH:, :POINT: y :XY son palabras clave
    ref1 = referencia numérica al camino
    ref2 = referencia numérica a cada punto del camino
    coordX = coordenada X del punto
    coordY = coordenada Y del punto
PATH:ref1:BLOCK_INI = ref_block (referencia al bloque que inica  el camino)
PATH:ref1:BLOCK_END = ref_block (referencia al bloque que termina el camino)
PATH:ref1:BITS = nBits (bits del camino)

BLOQUES
=======
Un bloque es un elemento que procesa la información que llega por sus entradas y la propaga a sus salidas
La información es particular para cada tipo de bloque

    TODOS LOS BLOQUES
    =================
    BLOCK:ref:TYPE                = tipo de bloque: COMBI(0), SEQ(1), NODE(2), IN_PORT(3), OUT_PORT(4), TAB(5), MEMORY(6)
    BLOCK:ref:ENABLE              = true=operativo
    BLOCK:ref:DELAY               = retardo en segundos desde que cambia una entrada hasta que cambia la salida
    BLOCK:ref:TT_FILE             = si != 0: path absoluto a un fichero true table
    BLOCK:ref:PATH_IN             = secuencia de referencias a los caminos que entran al bloque
    BLOCK:ref:PATH_OUT            = secuencia de referencias a los caminos que salen del bloque
    BLOCK:ref:BITS_PATH_OUT:index = <bits_del_camino> <bit_más_sig> <bit_menos_sig>
    BLOCK:ref:BITS_PATH_IN:index  = <bits_del_camino> <bit_más_sig> <bit_menos_sig>

    BLOQUE COMBINACIONAL
    ====================
    BLOCK:ref:COMBI:TYPE          = Tipo de bloque combinacional: NOT(0), AND(1), OR(2), XOR(3), NAND(4), NOR(5), XNOR(6), 
                                                                  ADD(7), MUX(8), CMP(10), OTHERS(11), DMUX(9), DECOD(10), DECODN(11)
    BLOCK:ref:COMBI:CMP_TYPE      = Tipo de comparador (< = >)
    BLOCK:ref:COMBI:TRUE_TABLE    = tabla de la verdad asociada al bloque

    BLOQUE SECUENCIAL
    =================
    BLOCK:ref:FF:TYPE             = Tipo de bloque secuencial: FFD(0), FFT(1), FFJK(2), LATCH(3), FSM(4)
    BLOCK:ref:FF:RESET_VALUE      = valor inicial del bloque
    BLOCK:ref:FF:ENABLE           = TRUE si siempre está activado (sin camino de validación)

    BLOQUES IN_PORT
    ===============
    BLOCK:ref:INIT_PORT_VALUE     = valor inicial
    
    'ref' es la referencia numérica que identifica al elemento
    El caracter de asignación '=' es condicional
    Para los elementos binarios, se acepta TRUE y YES. Cualquier otra expresión se asocia a FALSE
    No diferencia mayúsculas de minúsculas
    La información a la derecha del caracter '#' se considera comentario
*/

#include "project.h"

//Constructor
//requiere un puntero a clase Gui con la que interactua
Project::Project(Gui *gui_p)
{
    m_gui_p=gui_p;                      //puntero a clase Gui
    m_cBlocks_p=gui_p->m_cBlocks_p;     //puntero a clase Blocks
    m_blocks_p=m_cBlocks_p->m_blocks_p; //puntero a array de BLOCKs
    m_paths_p=m_cBlocks_p->m_paths_p;   //puntero a array de PATHs
    
    //inicialización
    for(int i=0; i<MAX_PATHS; i++)
        {
        m_allPathsPoints_p[i]=0;    //puntero para alojar temporalmente los puntos del camino
        m_nAllPathsPoints[i]=0;     //# puntos
        m_nPathBits[i]=0;
        }
}

//Destructor
//Elimina la memoria reservada
Project::~Project()
{
//    printf("init Project destructor\n");
    for(int i=0; i<MAX_PATHS; i++)
        {
        if(m_allPathsPoints_p[i]) delete[] m_allPathsPoints_p[i];
        }
//    printf("finish Project destructor\n");
}


//guarda al fichero m_gui_p->m_projectName el proyecto actual
int Project::saveProject()
{
    FILE *fp=0;
    int ret; 
    char errMessage[200];
    char txt[200], txt2[200];
    Common tools;
    const char *blockType[]={"COMBI", "SEQ", "NODE", "IN_PORT", "OUT_PORT", "TAB", "MEMORY"};
    const char *combiType[]={"NOT", "AND", "OR", "XOR", "NAND", "NOR", "XNOR", "ADD", "MUX", "CMP", "OTHERS", "DMUX", "DECOD", "DECODN"};
    const char *seqType[]={"FFD", "FFT", "FFJK", "LATCH", "FSM"};
    const char *cmpType[]={"<", "=", ">"};
    
    //intenta abrir el fichero de proyecto
    if((fp=fopen(m_gui_p->m_projectName, "w"))==0) 
    {
        sprintf(txt, "Warning: the %s file could not be created.", m_gui_p->m_projectName);
        QMessageBox::warning(0, "dCir", txt, QMessageBox::Close);
        return -1;
    }
    sprintf(errMessage, "Error writing to file %s", m_gui_p->m_projectName);

    //GENERIC KEY WORD
    //================
    fputs("#GENERIC KEY WORDS\n", fp);
    sprintf(txt, "PROJECT_DIR = \"%s\"\n", m_gui_p->m_projectDir);
    if (fputs(txt, fp) == EOF)
        {QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close);}
    //si el fichero gráfico está en el mismo directorio del proyecto
    //se copia sólo el nombre del fichero, no el directorio
    tools.getDirFromPath(m_gui_p->m_graphicsFile, txt2);
    if (strcmp(m_gui_p->m_projectDir, txt2) == 0)
    {
        tools.getFileNameFromPath(m_gui_p->m_graphicsFile, txt2);
        sprintf(txt, "GRAPHICS_FILE = \"%s\"\n", txt2);
    }
    else
        sprintf(txt, "GRAPHICS_FILE = \"%s\"\n", m_gui_p->m_graphicsFile);
    if(fputs(txt, fp)==EOF) 
        {QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close);}
    sprintf(txt, "DELTA_T = %d\n", m_gui_p->m_params.deltaT_blocks);
    if (fputs(txt, fp) == EOF)
        {QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close);}
    sprintf(txt, "VELOCITY = %d\n", m_gui_p->m_params.velocity);
    if (fputs(txt, fp) == EOF)
        {QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); }
    sprintf(txt, "ARROW_TYPE = %d\n", m_gui_p->m_params.arrowType);
    if (fputs(txt, fp) == EOF)
        {QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); }
    int R, G, B;
    m_gui_p->m_params.arrowColor0.getRgb(&R, &G, &B);
    sprintf(txt, "ARROW_COLOR0_RGB = %d %d %d\n", R, G, B);
    if (fputs(txt, fp) == EOF)
        { QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); }
    m_gui_p->m_params.arrowColor1.getRgb(&R, &G, &B);
    sprintf(txt, "ARROW_COLOR1_RGB = %d %d %d\n", R, G, B);
    if (fputs(txt, fp) == EOF)
        { QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); }
    m_gui_p->m_params.pathColor.getRgb(&R, &G, &B);
    sprintf(txt, "PATH_COLOR_RGB = %d %d %d\n", R, G, B);
    if (fputs(txt, fp) == EOF)
        { QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); }
    m_gui_p->m_params.blockColor.getRgb(&R, &G, &B);
    sprintf(txt, "BLOCK_COLOR_RGB = %d %d %d\n", R, G, B);
    if (fputs(txt, fp) == EOF)
        { QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); }

    if(m_gui_p->m_params.viewPaths) sprintf(txt, "VIEW_PATHS = TRUE\n");
    else sprintf(txt, "VIEW_PATHS = FALSE\n");
    if (fputs(txt, fp) == EOF)
        { QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); }

    if(m_gui_p->m_params.viewPathsName) sprintf(txt, "VIEW_PATHS_NAME = TRUE\n");
    else sprintf(txt, "VIEW_PATHS_NAME = FALSE\n");
    if (fputs(txt, fp) == EOF)
        { QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); }

    if(m_gui_p->m_params.viewIniValuePaths) sprintf(txt, "VIEW_INI_VALUE_PATHS = TRUE\n");
    else sprintf(txt, "VIEW_INI_VALUE_PATHS = FALSE\n");    
    if (fputs(txt, fp) == EOF)
        { QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); }

    if(m_gui_p->m_params.viewEndValuePaths) sprintf(txt, "VIEW_END_VALUE_PATHS = TRUE\n");
    else sprintf(txt, "VIEW_END_VALUE_PATHS = FALSE\n");
    if (fputs(txt, fp) == EOF)
        { QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); }

    if(m_gui_p->m_params.viewNodeBlocksName) sprintf(txt, "VIEW_NODE_BLOCKS_NAME = TRUE\n");
    else sprintf(txt, "VIEW_NODE_BLOCKS_NAME = FALSE\n");
    if (fputs(txt, fp) == EOF)
        { QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); }

    if(m_gui_p->m_params.viewPortBlocksName) sprintf(txt, "VIEW_PORT_BLOCKS_NAME = TRUE\n");
    else sprintf(txt, "VIEW_PORT_BLOCKS_NAME = FALSE\n");
    if (fputs(txt, fp) == EOF)
        { QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); }

    if(m_gui_p->m_params.viewOthersBlocksName) sprintf(txt, "VIEW_OTHERS_BLOCKS_NAME = TRUE\n");
    else sprintf(txt, "VIEW_OTHERS_BLOCKS_NAME = FALSE\n");
    if (fputs(txt, fp) == EOF)
        { QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); }

    if(m_gui_p->m_params.viewRectBlocks) sprintf(txt, "VIEW_RECT_BLOCKS = TRUE\n");
    else sprintf(txt, "VIEW_RECT_BLOCKS = FALSE\n");
    if (fputs(txt, fp) == EOF)
        { QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); }

    //KEY WORDS FOR PATHS
    //===================
    QPointF p;
    sprintf(txt, "\n#KEY WORDS FOR PATHS\n");
    if (fputs(txt, fp) == EOF)
    { QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); }
    for(int path=0; path<MAX_PATHS; path++) //para todos los caminos
        {
        if(m_cBlocks_p->existPath(path)) //si existe el camino
            {
            sprintf(txt, "PATH:%d:BITS = %d\n", path, m_cBlocks_p->m_paths_p[path].nBits);
            if (fputs(txt, fp) == EOF)
            { QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); }
            sprintf(txt, "PATH:%d:BLOCK_INI = %d\n", path, m_cBlocks_p->m_paths_p[path].blockIni);
            if (fputs(txt, fp) == EOF)
            { QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); }
            sprintf(txt, "PATH:%d:BLOCK_END = %d\n", path, m_cBlocks_p->m_paths_p[path].blockEnd);
            if (fputs(txt, fp) == EOF)
            { QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); }

            for(int seg=0; seg<m_paths_p[path].nSegments; seg++) //para todos los segmentos del camino
                {
                p=m_paths_p[path].segment_p[seg].pIni;          //punto inicial de cada segmento
                sprintf(txt, "PATH:%d:POINT:%d:XY = %d %d\n", path, seg, (int)p.x(), (int)p.y());
                if (fputs(txt, fp) == EOF)
                { QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); }
            }
            int seg=m_paths_p[path].nSegments;
            p=m_paths_p[path].segment_p[seg-1].pEnd;            //punto final del camino
            sprintf(txt, "PATH:%d:POINT:%d:XY = %d %d\n", path, seg, (int)p.x(), (int)p.y());
            if (fputs(txt, fp) == EOF)
            { QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); }
        }
        }
        
    //KEY WORDS FOR BLOCKS
    //====================
    sprintf(txt, "\n#KEY WORDS FOR BLOCKS\n");
    if (fputs(txt, fp) == EOF)
    { QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); }
    for(int bk=0; bk<MAX_BLOCKS; bk++) //para todos los bloques
        {
        if(m_cBlocks_p->existBlock(bk)) //si existe
            {
            //INFO COMÚN
            //==========
            sprintf(txt, "BLOCK:%d:TYPE = %d #%s\n", bk, m_blocks_p[bk].type, blockType[(int)m_blocks_p[bk].type]);    
            if (fputs(txt, fp) == EOF)
            { QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); }
            if(m_blocks_p[bk].enable==true)
                sprintf(txt, "BLOCK:%d:ENABLE = TRUE\n", bk);  
            else
                sprintf(txt, "BLOCK:%d:ENABLE = FALSE\n", bk);    
            if (fputs(txt, fp) == EOF)
            { QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); }
            //COMBINACIONAL
            //=============
            if(m_blocks_p[bk].type==0) //COMBI
                {
                sprintf(txt, "BLOCK:%d:COMBI:TYPE = %d #%s\n", bk, m_blocks_p[bk].combi.type, combiType[(int)m_blocks_p[bk].combi.type]);
                if (fputs(txt, fp) == EOF)
                    { QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); }
                if(m_blocks_p[bk].combi.type== (char)Oper::MUX || m_blocks_p[bk].combi.type == (char)Oper::DMUX ||
                    m_blocks_p[bk].combi.type == (char)Oper::DECOD || m_blocks_p[bk].combi.type == (char)Oper::DECODN) //caso de MUX o DMUX, se salva la info de selección
                    {
                    sprintf(txt, "BLOCK:%d:COMBI:PATH_SELECT = ", bk);
                    for(int p=0; p<m_blocks_p[bk].combi.nPathSelect; p++)
                        {
                        sprintf(txt + strlen(txt), "%d ", m_blocks_p[bk].combi.pathSelect[p]);                            
                        }
                    sprintf(txt + strlen(txt), "\n");
                    if (fputs(txt, fp) == EOF)
                        { QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); }
                    }
                if(m_blocks_p[bk].combi.type== (char)Oper::CMP) //caso de CMP se salva la info comparación
                    {
                    sprintf(txt, "BLOCK:%d:COMBI:CMP_TYPE = %d #%s\n", bk, m_blocks_p[bk].combi.logicCmp, cmpType[(int)m_blocks_p[bk].combi.logicCmp]);
                    if (fputs(txt, fp) == EOF)
                        { QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); }                    
                    }
            }
            //SECUENCIAL
            //==========
           else if(m_blocks_p[bk].type==1) //SEQ
                {
                if(m_blocks_p[bk].FF.type== (char)FF::LATCH || m_blocks_p[bk].FF.enable==false) //
                    {
                    sprintf(txt, "BLOCK:%d:PATH_ENABLE = ", bk);
                    sprintf(txt + strlen(txt), "%d \n", m_blocks_p[bk].pathEnable);                            
                    if (fputs(txt, fp) == EOF)
                        QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); 
                }
                 sprintf(txt, "BLOCK:%d:FF:TYPE = %d #%s\n", bk, m_blocks_p[bk].FF.type, seqType[(int)m_blocks_p[bk].FF.type]);
                 if (fputs(txt, fp) == EOF)
                    QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); 
                sprintf(txt, "BLOCK:%d:FF:RESET_VALUE = %d\n", bk, m_blocks_p[bk].FF.resetValue);
                if (fputs(txt, fp) == EOF)
                    QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); 
                if(m_blocks_p[bk].FF.enable==true)
                    sprintf(txt, "BLOCK:%d:FF:ENABLE = TRUE\n", bk);  
                else
                    sprintf(txt, "BLOCK:%d:FF:ENABLE = FALSE\n", bk);    
                if (fputs(txt, fp) == EOF)
                    QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); 
            }
            //PUERTO DE ENTRADA
            //=================
            else if(m_blocks_p[bk].type==3) //PORT_IN
                {
                sprintf(txt, "BLOCK:%d:INIT_PORT_VALUE = %d\n", bk, m_blocks_p[bk].initPortValue);    
                if (fputs(txt, fp) == EOF)
                    QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); 
                }
            
            sprintf(txt, "BLOCK:%d:DELAY = %d\n", bk, m_blocks_p[bk].delay);    
            if (fputs(txt, fp) == EOF)
                QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); 

            if(m_blocks_p[bk].TT.fileName==0)
                sprintf(txt, "BLOCK:%d:TT_FILE = 0\n", bk);   
            else
            {
                //si el fichero .tt está en el mismo directorio del proyecto
                //se copia sólo el nombre del fichero, no el directorio
                tools.getDirFromPath(m_blocks_p[bk].TT.fileName, txt2);
                if (strcmp(m_gui_p->m_projectDir, txt2) == 0)
                {
                    tools.getFileNameFromPath(m_blocks_p[bk].TT.fileName, txt2);
                    sprintf(txt, "BLOCK:%d:TT_FILE = \"%s\"\n", bk, txt2);
                }
                else
                    sprintf(txt, "BLOCK:%d:TT_FILE = \"%s\"\n", bk, m_blocks_p[bk].TT.fileName);
            }
            if (fputs(txt, fp) == EOF)
                QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close);

            sprintf(txt, "BLOCK:%d:PATH_IN = ", bk);
            if(m_blocks_p[bk].nPathIn==0) 
                sprintf(txt + strlen(txt), "-1 ");
            for(int path=0; path<m_blocks_p[bk].nPathIn; path++) //para todos los caminos de entrada
                {
                if(m_blocks_p[bk].type== (char)Type::NODE || m_blocks_p[bk].type== (char)Type::TAB)
                    sprintf(txt + strlen(txt), "%d ",m_blocks_p[bk].nodePathIn[path].path);
                else
                    sprintf(txt + strlen(txt), "%d ", m_blocks_p[bk].pathIn[path]);
                }
            sprintf(txt + strlen(txt), "\n");
            if (fputs(txt, fp) == EOF)
                QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); 

            sprintf(txt, "BLOCK:%d:PATH_OUT = ", bk);
            if(m_blocks_p[bk].nPathOut==0) 
                sprintf(txt + strlen(txt), "-1 ");
            for(int path=0; path<m_blocks_p[bk].nPathOut; path++) //para todos los caminos de salida
                {
                if(m_blocks_p[bk].type== (char)Type::NODE || m_blocks_p[bk].type== (char)Type::TAB)
                    sprintf(txt + strlen(txt), "%d ",m_blocks_p[bk].nodePathOut[path].path);
                else
                    sprintf(txt + strlen(txt), "%d ", m_blocks_p[bk].pathOut[path]);
                }
            sprintf(txt + strlen(txt), "\n");
            if (fputs(txt, fp) == EOF)
                QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); 

            if(m_blocks_p[bk].type== (char)Type::NODE || m_blocks_p[bk].type== (char)Type::TAB)
                {
                sprintf(txt, "BLOCK:%d:BITS_PATH_OUT:", bk);
                int size=strlen(txt);
                if(m_blocks_p[bk].nPathOut==0) 
                    {
                    sprintf(txt + strlen(txt), "-1 \n");
                    if (fputs(txt, fp) == EOF)
                        QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); 
                    }
                for(int path=0; path<m_blocks_p[bk].nPathOut; path++) //para todos los caminos de salida
                    {
                    sprintf(txt + size, "%d = %d %d %d\n", path, m_blocks_p[bk].nodePathOut[path].path, 
                            m_blocks_p[bk].nodePathOut[path].bitIni, m_blocks_p[bk].nodePathOut[path].bitEnd);
                        
                    if (fputs(txt, fp) == EOF)
                        QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); 
                    }
                    
                sprintf(txt, "BLOCK:%d:BITS_PATH_IN:", bk);
                size=strlen(txt);
                if(m_blocks_p[bk].nPathIn==0) 
                    {
                    sprintf(txt + strlen(txt), "-1 \n");
                    if (fputs(txt, fp) == EOF)
                        QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); 
                    }
                for(int path=0; path<m_blocks_p[bk].nPathIn; path++) //para todos los caminos de salida
                    {
                    sprintf(txt + size, "%d = %d %d %d\n", path, m_blocks_p[bk].nodePathIn[path].path, 
                            m_blocks_p[bk].nodePathIn[path].bitIni, m_blocks_p[bk].nodePathIn[path].bitEnd);
                        
                    if (fputs(txt, fp) == EOF)
                        QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); 
                    }
                }
//            if (fputs(txt, fp) == EOF)
//            { QMessageBox::warning(0, "Project", errMessage, QMessageBox::Close); }
            }
        }
    if(fp) fclose(fp);
    return 0;
}

//carga desde el fichero m_gui_p->m_projectName el proyecto actual
int Project::loadProject()
{
    FILE *fp;
    bool hit = true;
    char txt[200];
    //intenta abrir el fichero de proyecto
    if((fp=fopen(m_gui_p->m_projectName, "r"))==0)
        { 
        sprintf(txt, "Warning: the file '%s' does not exist\n", m_gui_p->m_projectName);
        QMessageBox::warning(0, "Project", txt, QMessageBox::Close); 
        return -1;
        }
    char line_p[MAX_SIZE_LINE];
    int ret;
     
    //aloja temporalmente los puntos de todos los camino
    for(int i=0; i<MAX_PATHS; i++)
        {
        m_allPathsPoints_p[i]=new QPointF[MAX_POINTS_PATH];
        m_allPathsPoints_p[i]->setX(0x7FFFFFFF); //se inicializa a un valor imposible
        m_nAllPathsPoints[i]=0;
        m_nPathBits[i]=0;
        }
        
    extractFromFile file; //clase para la extracción de la información desde fichero
    int lineCount=1;
    while(true)
        {
        ret=file.extractNextLine(fp, line_p, MAX_SIZE_LINE); //nueva linea 
        if(ret==-1) break; //fin de fichero
        else if(ret<-1) 
            {
            sprintf(txt, "Warning: some problem reading the %s file\n", m_gui_p->m_projectName);
            QMessageBox::warning(0, "Project", txt, QMessageBox::Close);
            return -1;
            }
        else if(strlen(line_p)<3 || line_p[0]=='#') continue;
        else
        {
            if(readOrder(line_p)==-1) //interpreta la linea y la carga en la estructura correspondiente (BLOCK/PATH)
            {
                hit = false; break;
            }
        }
        lineCount++;
        }
        
    //Se convierte de puntos a caminos
    for(int p=0; p<MAX_POINTS_PATH; p++)
        {
        if (m_allPathsPoints_p[p]->x() != 0x7FFFFFFF) //si existe el camino
        {
            m_cBlocks_p->newPath(p, m_nPathBits[p], m_allPathsPoints_p[p], m_nAllPathsPoints[p]);  //se crea y hace visible            
            m_cBlocks_p->m_paths_p[p].blockIni=m_pathBlockIni[p];
            m_cBlocks_p->m_paths_p[p].blockEnd=m_pathBlockEnd[p];
            }
        }
        
    //se determina el número de bits de cada bloque = bits máximo de sus caminos que entran o salen
    m_cBlocks_p->setAllBlockBits();
    if (fp) fclose(fp);
    if (!hit) return -1;
    return 0;
}
 
//interpretación de cada linea de texto
int Project::readOrder(char *line_p)
{
    char txt[200];
    int indexWord=0, wordCount=0, ret, ini;
    char *words_p[MAX_LINE_WORDS];
    char word_p[MAX_SIZE_LINE]; //pueden ser largas cadenas de caracteres (file paths, etc.)
    for(int i=0; i<MAX_LINE_WORDS; i++)
        words_p[i]=0;
    
    extractFromFile file;        
    //Se descompone la linea de entrada en palabras que se ubican en words_p[]
    for(int i=0; i<MAX_LINE_WORDS; i++)
        {
        ret=file.extractNextWord(line_p, &indexWord, word_p, ':'); //en word_p[] queda la siguienete palabra de la linea
        if (word_p[0] == '"') //Se eliminan las comillas
        {
            int i;
            for (i = 1; word_p[i] != '"'; i++)
                word_p[i - 1] = word_p[i];
            word_p[i-1] = 0;
        }
        if(ret==-1) break;                      //linea vacía o de comentario
        if(word_p[0]==0) break;                 //no más palabras
        int size=strlen(word_p)+1;              //espacio para el '\0'
        if(size>MAX_CHARACTER_WORD) 
        {
            sprintf(txt, "Warning: error on line %s\nmax characters in a word are %d", line_p, MAX_CHARACTER_WORD);
            QMessageBox::warning(0, "Project", txt, QMessageBox::Close);
            return -1;
        }
        words_p[wordCount]=new char[size];      //reserva para alojar la palabra clave
        strcpy(words_p[wordCount++], word_p);   //copia de la palabra al array de palabras
        }  
        
    char lowCaseWord[MAX_CHARACTER_WORD];   //array para alojar la primera palabra clave, en minúsculas
    char word2[MAX_CHARACTER_WORD];         //array para alojar la segunda palabra clave, en minúsculas
    if(words_p[0]!=0 && wordCount>1)        //si existen palabras clave y argumentos
        {
        file.strToLowCase(words_p[0], lowCaseWord);         //palabra a minúsculas
        if (strcmp(lowCaseWord, "project_dir") == 0)
        {
            Common tools;
            m_gui_p->m_projectDir[0] = 0;
            if (words_p[1][0] == '=') ini = 2; else ini = 1;       //considera si existe o no el caracter '='
            if (ini >= wordCount || strlen(words_p[ini]) == 0)
                return -1;

            if (wordCount > 3) //windows mode absolute path-> C:\... (':' es separador de campos)
            {
                strcpy(m_gui_p->m_projectDir, words_p[ini]);  //info a estructura del proyecto
                strcat(m_gui_p->m_projectDir, ":");
                strcat(m_gui_p->m_projectDir, words_p[ini + 1]);
            }
            else
                strcpy(m_gui_p->m_projectDir, words_p[ini]);  //info a estructura del proyecto
            
            char directory[200];
            if(tools.getDirFromPath(m_gui_p->m_projectName, directory)==false)
            {
                sprintf(txt, "Warning: %s  unknown directory.", directory);
                QMessageBox::warning(0, "Project information", txt, QMessageBox::Close);
                return -1;
            }
            else if(strcmp(directory, m_gui_p->m_projectDir)!=0) //distintos
            {
/*
                int ret = QMessageBox::warning(0, "Project information",
                    "The given directory does not match the original.\n"
                        "Do you want to use it?",
                    QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);  
                if (ret == QMessageBox::Yes)
*/
                    strcpy(m_gui_p->m_projectDir, directory);  //Se actualiza el directorio
               // else return -1;
                }

            if(!QDir(m_gui_p->m_projectDir).exists()) //si no existe el directorio se pide actulizar
            {
                while (true)
                {
                    char txt[200];
                    sprintf(txt, "Warning: the %s directory does not exist", m_gui_p->m_projectDir);
                    QMessageBox::warning(0, "Project information", txt, QMessageBox::Close);
                    DirDialog dirDiag((char*)"- open project directory -", m_gui_p->m_projectDir); //diálogo
                    dirDiag.exec();
                    if (dirDiag.m_accept == 0) return -1;  //se aborta -> fallo grave
                    tools.QlineEditToChar(dirDiag.dirValue, txt); //nombre del proyecto
                    if (QDir(txt).exists())
                    {
                        strcpy(m_gui_p->m_projectDir, txt);  //info a estructura del proyecto
                        break;
                    }
                }
            }
        }
        else if(strcmp(lowCaseWord, "graphics_file")==0)
            {
            Common tools;
            m_gui_p->m_graphicsFile[0] = 0;
            if(words_p[1][0]=='=') ini=2; else ini=1;       //considera si existe o no el caracter '='
            if(ini>=wordCount || strlen(words_p[ini]) == 0)
                return -1;
                
            if (wordCount > 3) //windows mode absolute path-> C:\... (':' es separador de campos)
                {
                strcpy(m_gui_p->m_graphicsFile, words_p[ini]);  //info a estructura del proyecto
                strcat(m_gui_p->m_graphicsFile, ":");
                strcat(m_gui_p->m_graphicsFile, words_p[ini + 1]);
                }
            else
                strcpy(m_gui_p->m_graphicsFile, words_p[ini]);  //info a estructura del proyecto

            if (!tools.existFile(m_gui_p->m_graphicsFile))
            {
                bool hit=tools.getFileNameFromPath(m_gui_p->m_graphicsFile, txt);
                if (hit)
                    {
                    sprintf(m_gui_p->m_graphicsFile, "%s/%s", m_gui_p->m_projectDir, txt);
                    if (!tools.existFile(m_gui_p->m_graphicsFile)) hit = false;
                    }
                if(hit==false)
                while (true)
                {
                    sprintf(txt, "Warning: the %s file does not exist", words_p[ini]);
                    QMessageBox::warning(0, "Project information", txt, QMessageBox::Close);
                    FileNameDialog fileDiag((char*)"- open graphics file -", m_gui_p->m_projectDir); //diálogo
                    fileDiag.exec();
                    if (fileDiag.m_accept == 0) //Creamos un fichero en blanco
                    {
                        sprintf(txt, "%s/blank.png", m_gui_p->m_projectDir);
                        strcpy(m_gui_p->m_graphicsFile, txt);
                        break;
                    }
                    tools.QlineEditToChar(fileDiag.fileValue, m_gui_p->m_graphicsFile); //nombre del proyecto
                    if (!tools.existFile(m_gui_p->m_graphicsFile)) continue;
                    break;
                }
            }
        }
        else if (strcmp(lowCaseWord, "delta_t") == 0)
            {
            if (words_p[1][0] == '=') ini = 2; else ini = 1;
            m_gui_p->m_params.deltaT_blocks = atoi(words_p[ini]);
            }
        else if(strcmp(lowCaseWord, "velocity")==0)
            {
            if(words_p[1][0]=='=') ini=2; else ini=1;
            m_gui_p->m_params.velocity=atoi(words_p[ini]);
            }
        else if(strcmp(lowCaseWord, "arrow_type")==0)
            {
            if(words_p[1][0]=='=') ini=2; else ini=1;
            m_gui_p->m_params.arrowType=atoi(words_p[ini]);
            }
        else if(strcmp(lowCaseWord, "arrow_color0_rgb")==0) //se extrae separadamente cada color
            {
            int R,G,B;
            if(words_p[1][0]=='=')  ini=2; else ini=1;
            R=atoi(words_p[ini]);G=atoi(words_p[ini+1]);B=atoi(words_p[ini+2]);
            m_gui_p->m_params.arrowColor0=QColor(R,G,B);    //info a estructura del proyecto
            }
        else if(strcmp(lowCaseWord, "arrow_color1_rgb")==0)
            {
            int R,G,B;
            if(words_p[1][0]=='=')  ini=2; else ini=1;
            R=atoi(words_p[ini]);G=atoi(words_p[ini+1]);B=atoi(words_p[ini+2]);
            m_gui_p->m_params.arrowColor1=QColor(R,G,B);
            }
        else if(strcmp(lowCaseWord, "path_color_rgb")==0)
            {
            int R,G,B;
            if(words_p[1][0]=='=') ini=2; else ini=1;
            R=atoi(words_p[ini]);G=atoi(words_p[ini+1]);B=atoi(words_p[ini+2]);
            m_gui_p->m_params.pathColor=QColor(R,G,B);
            }
        else if(strcmp(lowCaseWord, "block_color_rgb")==0)
            {
            int R,G,B;
            if(words_p[1][0]=='=') ini=2; else ini=1;
            R=atoi(words_p[ini]);G=atoi(words_p[ini+1]);B=atoi(words_p[ini+2]);
            m_gui_p->m_params.blockColor=QColor(R,G,B);
            }
        else if(strcmp(lowCaseWord, "view_paths")==0) 
            {
            if(words_p[1][0]=='=') ini=2; else ini=1;
            file.strToLowCase(words_p[ini], word2);
            if(strcmp(word2, "yes")==0 || strcmp(word2, "true")==0)
                m_gui_p->m_params.viewPaths=true;
            else
                m_gui_p->m_params.viewPaths=false;
            }
        else if(strcmp(lowCaseWord, "view_paths_name")==0) 
            {
            if(words_p[1][0]=='=') ini=2; else ini=1;
            file.strToLowCase(words_p[ini], word2);
            if(strcmp(word2, "yes")==0 || strcmp(word2, "true")==0)
                m_gui_p->m_params.viewPathsName=true;
            else
                m_gui_p->m_params.viewPathsName=false;
            }
        else if(strcmp(lowCaseWord, "view_ini_value_paths")==0) 
            {
            if(words_p[1][0]=='=') ini=2; else ini=1;
            file.strToLowCase(words_p[ini], word2);
            if(strcmp(word2, "yes")==0 || strcmp(word2, "true")==0)
                m_gui_p->m_params.viewIniValuePaths=true;
            else
                m_gui_p->m_params.viewIniValuePaths=false;
            }
        else if(strcmp(lowCaseWord, "view_end_value_paths")==0) 
            {
            if(words_p[1][0]=='=') ini=2; else ini=1;
            file.strToLowCase(words_p[ini], word2);
            if(strcmp(word2, "yes")==0 || strcmp(word2, "true")==0)
                m_gui_p->m_params.viewEndValuePaths=true;
            else
                m_gui_p->m_params.viewEndValuePaths=false;
            }
        else if(strcmp(lowCaseWord, "view_node_blocks_name")==0) 
            {
            if(words_p[1][0]=='=') ini=2; else ini=1;
            file.strToLowCase(words_p[ini], word2);
            if(strcmp(word2, "yes")==0 || strcmp(word2, "true")==0)
                m_gui_p->m_params.viewNodeBlocksName=true;
            else
                m_gui_p->m_params.viewNodeBlocksName=false;
            }
        else if(strcmp(lowCaseWord, "view_port_blocks_name")==0) 
            {
            if(words_p[1][0]=='=') ini=2; else ini=1;
            file.strToLowCase(words_p[ini], word2);
            if(strcmp(word2, "yes")==0 || strcmp(word2, "true")==0)
                m_gui_p->m_params.viewPortBlocksName=true;
            else
                m_gui_p->m_params.viewPortBlocksName=false;
            }
        else if(strcmp(lowCaseWord, "view_node_blocks_name")==0) 
            {
            if(words_p[1][0]=='=') ini=2; else ini=1;
            file.strToLowCase(words_p[ini], word2);
            if(strcmp(word2, "yes")==0 || strcmp(word2, "true")==0)
                m_gui_p->m_params.viewNodeBlocksName=true;
            else
                m_gui_p->m_params.viewNodeBlocksName=false;
            }
        else if(strcmp(lowCaseWord, "view_others_blocks_name")==0) 
            {
            if(words_p[1][0]=='=') ini=2; else ini=1;
            file.strToLowCase(words_p[ini], word2);
            if(strcmp(word2, "yes")==0 || strcmp(word2, "true")==0)
                m_gui_p->m_params.viewOthersBlocksName=true;
            else
                m_gui_p->m_params.viewOthersBlocksName=false;
            }
        else if(strcmp(lowCaseWord, "view_rect_blocks")==0) 
            {
            if(words_p[1][0]=='=') ini=2; else ini=1;
            file.strToLowCase(words_p[ini], word2);
            if(strcmp(word2, "yes")==0 || strcmp(word2, "true")==0)
                m_gui_p->m_params.viewRectBlocks=true;
            else
                m_gui_p->m_params.viewRectBlocks=false;
            }
        else if(strcmp(lowCaseWord, "path")==0)
            {
            int path, point; 
            file.strToLowCase(words_p[2], word2);  //palabra a minúsculas
            if(strcmp(word2, "bits")==0) 
                {
                path=atoi(words_p[1]); 
                if(words_p[3][0]=='=') ini=4; else ini=3;
                m_nPathBits[path]=atoi(words_p[ini]);                    
                }
            if(strcmp(word2, "block_ini")==0) 
                {
                path=atoi(words_p[1]); 
                if(words_p[3][0]=='=') ini=4; else ini=3;
                m_pathBlockIni[path]=atoi(words_p[ini]);                    
                }
            if(strcmp(word2, "block_end")==0) 
                {
                path=atoi(words_p[1]); 
                if(words_p[3][0]=='=') ini=4; else ini=3;
                m_pathBlockEnd[path]=atoi(words_p[ini]);                    
                }
            if(strcmp(word2, "point")==0) 
                {
                path=atoi(words_p[1]); point=atoi(words_p[3]);
                if(words_p[5][0]=='=') ini=6; else ini=5;
                m_allPathsPoints_p[path][point]=QPointF(atoi(words_p[ini]), atoi(words_p[ini+1]));   
                m_nAllPathsPoints[path]++;
                }
            }
        else if (strcmp(lowCaseWord, "block") == 0)
        {
            int block;
            block = atoi(words_p[1]);
            m_blocks_p[block].blockName.value = block;

            file.strToLowCase(words_p[2], word2);  //palabra a minúsculas

            if (strcmp(word2, "type") == 0)
            {
                if (words_p[3][0] == '=') ini = 4; else ini = 3;
                m_blocks_p[block].type = atoi(words_p[ini]);
            }
            else if (strcmp(word2, "enable") == 0)
            {
                if (words_p[3][0] == '=') ini = 4; else ini = 3;
                file.strToLowCase(words_p[ini], word2);
                if (strcmp(word2, "yes") == 0 || strcmp(word2, "true") == 0)
                    m_blocks_p[block].enable = true;
                else
                    m_blocks_p[block].enable = false;
            }
            else if (strcmp(word2, "delay") == 0)
            {
                if (words_p[3][0] == '=') ini = 4; else ini = 3;
                m_blocks_p[block].delay = atoi(words_p[ini]);
                m_blocks_p[block].currentDelay = atoi(words_p[ini]);
            }
            else if (strcmp(word2, "init_port_value") == 0)
            {
                if (words_p[3][0] == '=') ini = 4; else ini = 3;
                file.strToLowCase(words_p[ini], word2);  //palabra a minúsculas
                if (strcmp(word2, "x") == 0)
                    m_blocks_p[block].initPortValueUnknow = true;
                else
                {
                    m_blocks_p[block].initPortValueUnknow = false;
                    m_blocks_p[block].initPortValue = atoi(words_p[ini]);
                }
            }
            else if (strcmp(word2, "path_in") == 0)
            {
                if (words_p[3][0] == '=') ini = 4; else ini = 3;
                for (int i = ini, k = 0; i < wordCount; i++, k++)  //para cada camino de entrada
                    m_blocks_p[block].pathIn[k] = atoi(words_p[i]);
                if (m_blocks_p[block].pathIn[0] == -1)         //sin valores de entrada
                    m_blocks_p[block].nPathIn = 0;
                else m_blocks_p[block].nPathIn = wordCount - ini;
            }
            else if (strcmp(word2, "path_out") == 0)
            {
                if (words_p[3][0] == '=') ini = 4; else ini = 3;

                for (int i = ini, k = 0; i < wordCount; i++, k++)  //para cada camino de salida
                    m_blocks_p[block].pathOut[k] = atoi(words_p[i]);
                if (m_blocks_p[block].pathOut[0] == -1)         //sin valores de entrada
                    m_blocks_p[block].nPathOut = 0;
                else m_blocks_p[block].nPathOut = wordCount - ini;
            }
            else if (strcmp(word2, "bits_path_out") == 0)
            {
                int index = atoi(words_p[3]);
                if (index >= 0)
                {
                    if (words_p[4][0] == '=') ini = 5; else ini = 4;
                    m_blocks_p[block].nodePathOut[index].path = atoi(words_p[ini + 0]);
                    m_blocks_p[block].nodePathOut[index].bitIni = atoi(words_p[ini + 1]);
                    m_blocks_p[block].nodePathOut[index].bitEnd = atoi(words_p[ini + 2]);
                }
            }
            else if (strcmp(word2, "bits_path_in") == 0)
            {
                int index = atoi(words_p[3]);
                if (index >= 0)
                {
                    if (words_p[4][0] == '=') ini = 5; else ini = 4;
                    m_blocks_p[block].nodePathIn[index].path = atoi(words_p[ini + 0]);
                    m_blocks_p[block].nodePathIn[index].bitIni = atoi(words_p[ini + 1]);
                    m_blocks_p[block].nodePathIn[index].bitEnd = atoi(words_p[ini + 2]);
                }
            }
            else if (strcmp(word2, "path_enable") == 0)
            {
                if (words_p[4][0] == '=') ini = 5; else ini = 4;
                m_blocks_p[block].pathEnable = atoi(words_p[ini]);
            }
            else if (strcmp(word2, "combi") == 0)
            {
                char word3[MAX_CHARACTER_WORD];
                file.strToLowCase(words_p[3], word3);  //palabra a minúsculas
                if (strcmp(word3, "type") == 0)
                {
                    if (words_p[4][0] == '=') ini = 5; else ini = 4;
                    m_blocks_p[block].combi.type = atoi(words_p[ini]);
                }
                else if (strcmp(word3, "path_select") == 0)
                {
                    if (words_p[4][0] == '=') ini = 5; else ini = 4;
                    for (int i = ini, k = 0; i < wordCount; i++, k++)  //para cada camino de entrada
                    {
                        int path = atoi(words_p[i]);
                        m_blocks_p[block].combi.pathSelect[k] = path;
                        /*m_pathBlockIni[path]=-1;*/ m_pathBlockEnd[path] = block;
                    }
                    if (m_blocks_p[block].combi.pathSelect[0] == -1)         //sin valores de entrada
                        m_blocks_p[block].combi.nPathSelect = 0;
                    else m_blocks_p[block].combi.nPathSelect = wordCount - ini;
                }
                else if (strcmp(word3, "cmp_type") == 0)
                    {
                    if (words_p[4][0] == '=') ini = 5; else ini = 4;
                    m_blocks_p[block].combi.logicCmp=atoi(words_p[ini]);
                    }
            }
            else if (strcmp(word2, "ff") == 0)
            {
                char word3[MAX_CHARACTER_WORD];
                file.strToLowCase(words_p[3], word3);  //palabra a minúsculas
                if (strcmp(word3, "type") == 0)
                {
                    if (words_p[4][0] == '=') ini = 5; else ini = 4;
                    m_blocks_p[block].FF.type = atoi(words_p[ini]);
                }
                else if (strcmp(word3, "reset_value") == 0)
                {
                    if (words_p[4][0] == '=') ini = 5; else ini = 4;
                    file.strToLowCase(words_p[ini], word3);  //palabra a minúsculas
                    if (strcmp(word3, "x") == 0)
                        m_blocks_p[block].FF.resetValueUnknow = true;
                    else
                    {
                        m_blocks_p[block].FF.resetValueUnknow = false;
                        m_blocks_p[block].FF.resetValue = atoi(words_p[ini]);
                    }
                }
                else if (strcmp(word3, "enable") == 0)
                {
                    if (words_p[4][0] == '=') ini = 5; else ini = 4;
                    file.strToLowCase(words_p[ini], word3);  //palabra a minúsculas
                    if (strcmp(word3, "yes") == 0 || strcmp(word3, "true") == 0)
                        m_blocks_p[block].FF.enable = true;
                    else
                        m_blocks_p[block].FF.enable = false;
                }
            }
            else if (strcmp(word2, "tt_file") == 0)
            {
            // se aceptan los formatos de nombre de fichero:
            // a) absolute path entrecomillado
            // b) absolute path sin comillas (no se aceptan espacios ni tildes)
            // c) solo el nombre del fichero y se busca en el directorio del proyecto
                bool hit = false;
                Files file;
                if (words_p[3][0] == '=') ini = 4; else ini = 3;
                if (words_p[ini] == 0) 
                    {
                    sprintf(txt, "Warning: no tt_file in block %d.\nPlease, update project file", block);
                    QMessageBox::warning(0, "Project", txt, QMessageBox::Close);
                    }
                if (wordCount > 5 && strlen(words_p[ini + 1]) > 4) //windows mode absolute path-> C:\...
                {
                    m_blocks_p[block].TT.fileName = new char[strlen(words_p[ini + 1]) + 3];
                    strcpy(m_blocks_p[block].TT.fileName, words_p[ini]);  //info a estructura del proyecto
                    strcat(m_blocks_p[block].TT.fileName, ":");
                    strcat(m_blocks_p[block].TT.fileName, words_p[ini + 1]);
                    hit = true;
                }
                else if (wordCount <= 5 && strlen(words_p[ini]) > 4) //windows mode absolute path-> "C:\..."
                {
                    m_blocks_p[block].TT.fileName = new char[strlen(words_p[ini]) + 1];
                    strcpy(m_blocks_p[block].TT.fileName, words_p[ini]);
                    hit = true;
                }
                if (hit == true)
                {
                    Common tools;
                    if (!tools.existFile(m_blocks_p[block].TT.fileName))
                    {
                        //si no existe el fichero, se prueba de buscarlo en el directorio del proyecto
                        bool hit = tools.getFileNameFromPath(m_blocks_p[block].TT.fileName, txt); //nombre del fichero
                        if (hit)
                        {   //se conforma el path y se intenta abrir
                            delete[]m_blocks_p[block].TT.fileName;
                            m_blocks_p[block].TT.fileName = new char[strlen(txt) + strlen(m_gui_p->m_projectDir) + 2];
                            sprintf(m_blocks_p[block].TT.fileName, "%s/%s", m_gui_p->m_projectDir, txt);
                            if (!tools.existFile(m_blocks_p[block].TT.fileName)) hit = false;
                        }
                        if (hit == false) //si no se abre se solicita el fichero al ususario
                        while (true)
                        {
                            char txt[200];
                            sprintf(txt, "Warning: the %s file does not exist", m_blocks_p[block].TT.fileName);
                            QMessageBox::warning(0, "Project information", txt, QMessageBox::Close);
                            FileNameDialog fileDiag((char*)"- open table file -", m_gui_p->m_projectDir); //diálogo
                            fileDiag.exec();
                            if (fileDiag.m_accept == 0) return -1;
                            tools.QlineEditToChar(fileDiag.fileValue, txt); //nombre del proyecto
                            if (m_blocks_p[block].TT.fileName && strlen(txt)>4)
                                {
                                delete[]m_blocks_p[block].TT.fileName; 
                                m_blocks_p[block].TT.fileName = new char[strlen(txt) + 1];
                                strcpy(m_blocks_p[block].TT.fileName, txt);
                            }
                            if (!tools.existFile(m_blocks_p[block].TT.fileName)) continue;
                            break;
                        }
                    }
                    if (m_blocks_p[block].type == (char)Type::COMBI)
                        file.fileToTrueTable(&m_blocks_p[block].TT, false);  //generic                      
                    else if (m_blocks_p[block].type == (char)Type::SEQ && m_blocks_p[block].FF.type == (char)FF::FSM)
                        file.fileToTrueTable(&m_blocks_p[block].TT, true);  //FSM                      
                    else if (m_blocks_p[block].type == (char)Type::MEMORY)
                        file.fileToMemory(&m_blocks_p[block].TT);
                }
                else
                    m_blocks_p[block].TT.fileName = 0;
                 
                }
     
        }
        else 
            {
            sprintf(txt, "Warning: line '%s' contains unknown information.\nPlease, update the project file", line_p);
            QMessageBox::warning(0, "Project", txt, QMessageBox::Close);
            }
    }

    for(int i=0; i<MAX_LINE_WORDS; i++)
        if(words_p[i]) delete[] words_p[i];
    return 0;
}

