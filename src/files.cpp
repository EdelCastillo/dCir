/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 *
 *   Manejo de ficheros true table y ensamblador para máquina sencilla
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
 
#include "files.h"

Files::Files()
{
}

//Carga una tabla de la verdad desde un fichero
//el fichero, en path completo, viene dentro de la estructura
int Files::fileToMemory(TRUE_TABLE* TT)
{
    FILE* fp = 0;
    int ret;
    char txt[200];

    //intenta abrir el fichero de proyecto
    if ((fp = fopen(TT->fileName, "r")) == 0)
    {
        sprintf(txt, "Warning: the %s file could not be opened.\nPlease, update the project file.", TT->fileName);
        QMessageBox::warning(0, "Files", txt, QMessageBox::Cancel);
        return -1;
    }

    int indexWord = 0, wordCount = 0;
    char* words_p[MAX_LINE_WORDS];
    char word_p[MAX_SIZE_LINE]; //pueden ser largas cadenas de caracteres (file paths, etc.)
    for (int i = 0; i < MAX_LINE_WORDS; i++)
        words_p[i] = 0;
    char line_p[MAX_SIZE_LINE];
    extractFromFile file; //clase para la extracción de la información desde fichero
    int lineCount, maxLineCount = 0;
    int maxCol = 0, minCol = 0x7FFFFFFF;
    Numeric cnv;

    //Primera lectura: se determinan las dimensiones de la  matriz necesaria para alojar la tabla de la verdad
    while (true)
    {
        ret = file.extractNextLine(fp, line_p, MAX_SIZE_LINE); //nueva linea 
        if (ret == -1) break; //fin de fichero
        else if (ret < -1)
        {
            sprintf(txt, "Warning: some problem reading the %s file", TT->fileName);
            QMessageBox::warning(0, "Files", txt, QMessageBox::Cancel);
            return -1;
        }
        else if (strlen(line_p) < 3 || line_p[0] == '#') continue;
        indexWord = 0; wordCount = 0;
        while (true)
        {
            ret = file.extractNextWord(line_p, &indexWord, word_p); //en word_p[] queda la siguienete palabra de la linea
            if (ret == -1) break;                      //linea vacía o de comentario
            if (word_p[0] == 0) break;                 //no más palabras
            if (wordCount == 0)
            {
                lineCount = cnv.getValueFromText(word_p);
                if (lineCount > maxLineCount) maxLineCount = lineCount;
            }
            wordCount++;
        }
        if (wordCount > maxCol) maxCol = wordCount; //observa si falta algún elemento en cualquier columna
        if (wordCount < minCol) minCol = wordCount;
    }
    if (maxCol != minCol)
    {
        QMessageBox::warning(0, "Files", "Warning: all rows are not the same lenght", QMessageBox::Cancel);
        return -1;
    }
    maxLineCount++;  //considera al cero
    maxLineCount = 128;

    //memoria
    if (TT->table_p) {
        delete TT->table_p; TT->table_p = 0;
        }
    int **tmp_p;
    tmp_p=new int*[maxLineCount];
    for(int i=0; i<maxLineCount; i++)
        {
        tmp_p[i]=new int[maxCol];
        tmp_p[i][0]=i;tmp_p[i][1]=0;
        }
    rewind(fp); //reinicio del fichero
    TT->nRows=maxLineCount;
    TT->nCols=maxCol;
    
    unsigned int value;
    bool hit=false;
    
    lineCount=0; wordCount=0;
    //Segunda lectura: se establece la matriz con la tabla de la verdad (tabla_p & nRows & nCols)
    while(true)
        {
        ret=file.extractNextLine(fp, line_p, MAX_SIZE_LINE); //nueva linea 
        if(ret==-1) break; //fin de fichero
        else if(ret<-1) 
        {
            sprintf(txt, "Warning: some problem reading the %s file", TT->fileName);
            QMessageBox::warning(0, "Files", txt, QMessageBox::Cancel);
            return -1;
        }
        else if(strlen(line_p)<3 || line_p[0]=='#') continue;
        indexWord=0; wordCount=0;
        while(true)
            {
            ret=file.extractNextWord(line_p, &indexWord, word_p); //en word_p[] queda la siguiente palabra de la linea
            if(ret==-1) break;                      //linea vacía o de comentario
            if(word_p[0]==0) break;                 //no más palabras
            
            value=cnv.getValueFromText(word_p);
            if(wordCount==0)    lineCount=value;
            tmp_p[lineCount][wordCount]=value;
            wordCount++;
            }            
        }

    TT->table_p=tmp_p;
    if(!hit)
        TT->nCols=maxCol-1;
    if(fp) fclose(fp);
    
    return 0;
}

//Carga una tabla de la verdad desde un fichero
//el fichero, en path completo, viene dentro de la estructura
//Las columnas tienen formato
//state  input  status+ output
//Se inserta una nueva columna luego de input que incluye una máscara
//esa máscara de construye a partir de input haciendo que los bits 'x' sean '0' y el resto '1'
int Files::fileToTrueTable(TRUE_TABLE *TT, bool FSM)
{
    FILE *fp=0;
    int ret; 
    TT->nRows=0;
    TT->nCols=0;
    char txt[200];
    
    //intenta abrir el fichero de proyecto
    if((fp=fopen(TT->fileName, "r"))==0) 
    {
        sprintf(txt, "Warning: file '%s' can not be opened", TT->fileName);
        QMessageBox::warning(0, "Files", txt, QMessageBox::Cancel);
        return -1;
    }

    int indexWord=0, wordCount=0;
    char *words_p[MAX_LINE_WORDS];
    char word_p[MAX_SIZE_LINE]; //pueden ser largas cadenas de caracteres (file paths, etc.)
    for(int i=0; i<MAX_LINE_WORDS; i++)
        words_p[i]=0;
    char line_p[MAX_SIZE_LINE];
    extractFromFile file; //clase para la extracción de la información desde fichero
    int lineCount=0;
    int maxCol=0, minCol=0x7FFFFFFF;
    //Primera lectura: se determinan las dimensiones de la  matriz necesaria para alojar la tabla de la verdad
    while(true)
        {
        ret=file.extractNextLine(fp, line_p, MAX_SIZE_LINE); //nueva linea 
        if(ret==-1) break; //fin de fichero
        else if(ret<-1) 
        {
            sprintf(txt, "Warning: some problem reading the %s file", TT->fileName);
            QMessageBox::warning(0, "Files", txt, QMessageBox::Cancel);
            return -1;
        }
        else if(strlen(line_p)<3 || line_p[0]=='#') continue;
        lineCount++;
        indexWord=0; wordCount=0;
        while(true)
            {
            ret=file.extractNextWord(line_p, &indexWord, word_p); //en word_p[] queda la siguienete palabra de la linea
            if(ret==-1) break;                      //linea vacía o de comentario
            if(word_p[0]==0) break;                 //no más palabras
            wordCount++;
            }
        if(wordCount>maxCol) maxCol=wordCount; //observa si falta algún elemento en cualquier columna
        if(wordCount<minCol) minCol=wordCount;
        }
    if(maxCol!=minCol) 
    {
        QMessageBox::warning(0, "Files", "Warning: all rows are not the same lenght", QMessageBox::Cancel);
        return -1;
    }
    maxCol++; //para alojar la columna de máscaras
    
    //memoria
    int **tmp_p;
    tmp_p=new int*[lineCount];
    for(int i=0; i<lineCount; i++)
        tmp_p[i]=new int[maxCol];
    rewind(fp); //reinicio del fichero
    TT->nRows=lineCount;
    TT->nCols=maxCol;
    
    unsigned int value;
    bool hit=false;
    Numeric cnv;
    unsigned int mask; 
    int nBits;
    
    lineCount=0; wordCount=0;
    //Segunda lectura: se establece la matriz con la tabla de la verdad (tabla_p & nRows & nCols)
    while(true)
        {
        ret=file.extractNextLine(fp, line_p, MAX_SIZE_LINE); //nueva linea 
        if(ret==-1) break; //fin de fichero
        else if(ret<-1) 
        {
            sprintf(txt, "Warning: some problem reading the %s file", TT->fileName);
            QMessageBox::warning(0, "Files", txt, QMessageBox::Cancel);
            return -1;
        }
        else if(strlen(line_p)<3 || line_p[0]=='#') continue;
        indexWord=0; wordCount=0;
        while(true)
            {
            ret=file.extractNextWord(line_p, &indexWord, word_p); //en word_p[] queda la siguiente palabra de la linea
            if(ret==-1) break;                      //linea vacía o de comentario
            if(word_p[0]==0) break;                 //no más palabras
            
            //if(cnv.isBinary(word_p))
            if((FSM && wordCount==1) || (!FSM && wordCount==0))
                {
                //value=valor decimal con 'x' a '0', máscara binaria con 'x' a '0'
                cnv.valueToMask(word_p, &value, &nBits, &mask);
                tmp_p[lineCount][wordCount++]=value;
                tmp_p[lineCount][wordCount]=mask;
                hit=true;
                }
            else {
                value=cnv.getValueFromText(word_p);
                tmp_p[lineCount][wordCount]=value;
                }
            wordCount++;
            }
        lineCount++;
        }
    TT->table_p=tmp_p;
    if(!hit)
        TT->nCols=maxCol-1;
    if(fp) fclose(fp);

    return 0;
}

//Carga una tabla de la verdad desde un fichero
//el fichero, en path completo, viene dentro de la estructura
//Carga tantas columnas como existan sin alteraciones
int Files::fileToCombi(TRUE_TABLE *TT)
{
    FILE *fp=0;
    int ret; 
    char txt[200];

    //intenta abrir el fichero de proyecto
    if((fp=fopen(TT->fileName, "r"))==0) 
    {
        sprintf(txt, "Warning: the %s file could not be opened.\nPlease, update the project file.", TT->fileName);
        QMessageBox::warning(0, "Files", txt, QMessageBox::Cancel);
        return -1;
    }

    int indexWord=0, wordCount=0;
    char *words_p[MAX_LINE_WORDS];
    char word_p[MAX_SIZE_LINE]; //pueden ser largas cadenas de caracteres (file paths, etc.)
    for(int i=0; i<MAX_LINE_WORDS; i++)
        words_p[i]=0;
    char line_p[MAX_SIZE_LINE];
    extractFromFile file; //clase para la extracción de la información desde fichero
    int lineCount=0;
    int maxCol=0, minCol=0x7FFFFFFF;
    //Primera lectura: se determinan las dimensiones de la  matriz necesaria para alojar la tabla de la verdad
    while(true)
        {
        ret=file.extractNextLine(fp, line_p, MAX_SIZE_LINE); //nueva linea 
        if(ret==-1) break; //fin de fichero
        else if(ret<-1) 
        {
            sprintf(txt, "Warning: some problem reading the %s file", TT->fileName);
            QMessageBox::warning(0, "Files", txt, QMessageBox::Cancel);
            return -1;
        }
        else if(strlen(line_p)<3 || line_p[0]=='#') continue;
        lineCount++;
        indexWord=0; wordCount=0;
        while(true)
            {
            ret=file.extractNextWord(line_p, &indexWord, word_p); //en word_p[] queda la siguienete palabra de la linea
            if(ret==-1) break;                      //linea vacía o de comentario
            if(word_p[0]==0) break;                 //no más palabras
            wordCount++;
            }
        if(wordCount>maxCol) maxCol=wordCount; //observa si falta algún elemento en cualquier columna
        if(wordCount<minCol) minCol=wordCount;
        }
    if(maxCol!=minCol) 
    {
        QMessageBox::warning(0, "Files", "Warning: all rows are not the same lenght", QMessageBox::Cancel);
        return -1;
    }

    //memoria
    int **tmp_p;
    tmp_p=new int*[lineCount];
    for(int i=0; i<lineCount; i++)
        tmp_p[i]=new int[maxCol];
    rewind(fp); //reinicio del fichero
    TT->nRows=lineCount;
    TT->nCols=maxCol;
    
    unsigned int value;
    bool hit=false;
    Numeric cnv;
    unsigned int mask; 
    int nBits;
    
    lineCount=0; wordCount=0;
    //Segunda lectura: se establece la matriz con la tabla de la verdad (tabla_p & nRows & nCols)
    while(true)
        {
        ret=file.extractNextLine(fp, line_p, MAX_SIZE_LINE); //nueva linea 
        if(ret==-1) break; //fin de fichero
        else if(ret<-1) 
        {
            sprintf(txt, "Warning: some problem reading the %s file", TT->fileName);
            QMessageBox::warning(0, "Files", txt, QMessageBox::Cancel);
            return -1;
        }
        else if(strlen(line_p)<3 || line_p[0]=='#') continue;
        indexWord=0; wordCount=0;
        while(true)
            {
            ret=file.extractNextWord(line_p, &indexWord, word_p); //en word_p[] queda la siguiente palabra de la linea
            if(ret==-1) break;                      //linea vacía o de comentario
            if(word_p[0]==0) break;                 //no más palabras
            
            value=cnv.getValueFromText(word_p);
            tmp_p[lineCount][wordCount]=value;
            
            wordCount++;
            }
        lineCount++;
        }
    TT->table_p=tmp_p;
    if(!hit)
        TT->nCols=maxCol-1;
    if(fp) fclose(fp);
    return 0;
}

//ensamblador para máquina sencilla
int Files::ensambla(char *inputFile)    
{
    bool hit=false;
    char txt[200];
    Common common; //utilidades

    FILE *fp=0, *fp2=0, *fp3=0;
    if((fp=fopen(inputFile, "r"))==0)
    {
        sprintf(txt, "Warning: the %s file could not be opened", inputFile);
        QMessageBox::warning(0, "Files", txt, QMessageBox::Cancel);
        return -1;
    }
    int index;
    char tmpFile[200];
    for(index=strlen(inputFile); index>=0; index--)
        if(inputFile[index]=='.') {hit=true; break;}
    if(index==0) strcpy(tmpFile, ".dasm");
    if(hit) {
        strcpy(tmpFile, inputFile);
        strcpy(tmpFile+index, ".dasm");
        }
    else 
    {
        QMessageBox::warning(0, "Files", "Warning: the tmpFile.txt file could not be opened", QMessageBox::Cancel);
        return -1;
    }
    if((fp2=fopen(tmpFile, "w"))==0)
    {
        sprintf(txt, "Warning: the %s file could not be created", tmpFile);
        QMessageBox::warning(0, "Files", txt, QMessageBox::Cancel);
        return -1;
    }

    if(index==0) strcpy(tmpFile, ".dasm");
    strcpy(tmpFile+index, ".tt");
    if(hit) {
        strcpy(tmpFile, inputFile);
        strcpy(tmpFile+index, ".tt");
        }
     if((fp3=fopen(tmpFile, "w"))==0)
     {
         sprintf(txt, "Warning: the %s file could not be created", tmpFile);
         QMessageBox::warning(0, "Files", txt, QMessageBox::Cancel);
         return -1;
     }

   extractFromFile file; //clase para la extracción de la información desde fichero
    file.setComment(';');
    Numeric cnv;
    
    char line_p[MAX_SIZE_LINE];
    char lowCaseWord[MAX_CHARACTER_WORD];   //array para alojar la primera palabra clave, en minúsculas
    char word2[MAX_CHARACTER_WORD];         //array para alojar la segunda palabra clave, en minúsculas
    char word_p[MAX_SIZE_LINE]; //pueden ser largas cadenas de caracteres (file paths, etc.)
    int ret, indexWord=0;
    unsigned int addr=0;
    int lineCount=0;
    m_labelIndex=0; m_dataIndex=0;
    
    //primera pasada: se identifican las etiquetas y la memoria de datos
    while(true)
        {
        indexWord=0;    
        ret=file.extractNextLine(fp, line_p, MAX_SIZE_LINE); //nueva linea 
        if(ret==-1) break; //fin de fichero
        else if(ret<-1) 
        {
            sprintf(txt, "Warning: some problem reading the %s file", inputFile);
            QMessageBox::warning(0, "Files", txt, QMessageBox::Cancel);
            return -1;
        }
        else if(strlen(line_p)<3 || line_p[0]==';') continue;
        
        indexWord=0;    
        lineCount++;
        for(int i=0; i<MAX_LINE_WORDS; i++)
            {
            ret=file.extractNextWord(line_p, &indexWord, word_p, ','); //en word_p[] queda la siguienete palabra de la linea
            if(ret==-1) break;                      //linea vacía o de comentario
            if(word_p[0]==0) break;                 //no más palabras
                
            file.strToLowCase(word_p, lowCaseWord);         //palabra a minúsculas
            if(strcmp(lowCaseWord, ".org")==0)  //directiva de dirección de inicio
                {
                ret=file.extractNextWord(line_p, &indexWord, word_p, ','); //en word_p[] queda la siguiente palabra de la linea
                if(ret==-1 || word_p[0]==0) break; 
                addr=cnv.getValueFromText(word_p);
                lineCount=-1;
                }
            else if((index=file.searchChar(word_p, ':'))!=-1)  //etiqueta de dirección
                {
                word_p[strlen(word_p)-1]=0;  //se elimina ':'
                strcpy(m_label[m_labelIndex].txt, word_p);
                m_label[m_labelIndex++].value=addr+lineCount;
                ret=file.extractNextWord(line_p, &indexWord, word_p, ','); //en word_p[] queda la siguiente palabra de la linea
                if(ret==-1 || word_p[0]==0) break; 
                file.strToLowCase(word_p, lowCaseWord);         //palabra a minúsculas
                if(strcmp(lowCaseWord, ".word")==0)  //directiva de palabra de datos
                    {
                    int localCount=0;
                    while(true)
                        {
                        ret=file.extractNextWord(line_p, &indexWord, word_p, ','); //en word_p[] queda la siguiente palabra de la linea
                        if(ret==-1 || word_p[0]==0) break; 
                        m_data[m_dataIndex].addr=addr+lineCount+localCount;
                        m_data[m_dataIndex++].value=cnv.getValueFromText(word_p);
                        localCount++;
                        }
                    }
                }
                
                
//                cnv.valueToHexa(value, word2);
            }
        }
    //se determina la dirección de memoria de datos más alta
    int maxAddr=0;
    for(int i=0; i<m_dataIndex; i++)
        if(m_data[i].addr>maxAddr) maxAddr=m_data[i].addr;
            
    //segunda pasada: etiquetas de equivalencia  
    rewind(fp);
    lineCount=0;
    m_equIndex=0;
    char number_p[20];

    while(true)
        {
        ret=file.extractNextLine(fp, line_p, MAX_SIZE_LINE); //nueva linea 
        if(ret==-1) break; //fin de fichero
        else if(ret<-1) 
        {
            sprintf(txt, "Warning: some problem reading the %s file", inputFile);
            QMessageBox::warning(0, "Files", txt, QMessageBox::Cancel);
            return -1;
        }
        else if(strlen(line_p)<3 || line_p[0]==';') continue;

        indexWord=0;    
        lineCount++;
        for(int i=0; i<MAX_LINE_WORDS; i++)
            {
            ret=file.extractNextWord(line_p, &indexWord, word_p, ','); //en word_p[] queda la siguienete palabra de la linea
            if(ret==-1 || word_p[0]==0) break;              //linea vacía o de comentario
            file.strToLowCase(word_p, lowCaseWord);         //palabra a minúsculas
            if(strcmp(lowCaseWord, "equ")==0)  //directiva de equivalencia
                {
                indexWord=0;   
                ret=file.extractNextWord(line_p, &indexWord, word_p, '+'); //palabra 0
                if(ret==-1 || word_p[0]==0) break; 
                strcpy(m_equ[m_equIndex].txt, word_p);
            
                ret=file.extractNextWord(line_p, &indexWord, word_p, '+'); //palabra 1 
                if(ret==-1 || word_p[0]==0) break; 
                ret=file.extractNextWord(line_p, &indexWord, word_p, '+');  //palabra 2
                if(ret==-1 || word_p[0]==0) break; 

                int numberSize=common.getNumberFromStr(word_p, number_p, strlen(word_p));
                if(numberSize>0) //existe un valor numérico
                    m_equ[m_equIndex++].value = cnv.getValueFromText(word_p);
                else //se trata de asociar el valor de una etiqueta más un desplazamiento
                {
                    int j;
                    hit = false;
                    for (j = 0; j < m_labelIndex; j++) //buscamos la etiqueta en la lista
                        if (strcmp(m_label[j].txt, word_p) == 0) { hit = true; break; }
                    if (hit) //existe una etiqueta
                    {
                        ret = file.extractNextWord(line_p, &indexWord, word_p, '+');  //palabra 2
                        if (ret == -1 || word_p[0] == 0) //no existen más palabras
                            m_equ[m_equIndex++].value = m_label[j].value; //se asocia el valor de la etiqueta
                        else //se asocia el valor de la etiqueta más un desplazamiento
                            m_equ[m_equIndex++].value = m_label[j].value + cnv.getValueFromText(word_p);
                    }
                    else m_equ[m_equIndex++].value = 0; //equ sin asignación. Se asocia el valor 0
                }
                }
            }
        }
    //tercera pasada
    rewind(fp);
    lineCount=0;
    char *labelMessage=(char*)"label is not located.\n";
    char txt1[20], txt2[20], txt3[20];
    
    fprintf(fp2, ";-- CODE --\n\n");
    while(true)
        {
        ret=file.extractNextLine(fp, line_p, MAX_SIZE_LINE); //nueva linea 
        if(ret==-1) break; //fin de fichero
        else if(ret<-1) 
        {
            sprintf(txt, "Warning: some problem reading the %s file", inputFile);
            QMessageBox::warning(0, "Files", txt, QMessageBox::Cancel);
            return -1;
        }
        else if(strlen(line_p)<3 || line_p[0]==';') continue;

        indexWord=0;
        lineCount++;
        unsigned int value1, value2;
        for(int i=0; i<MAX_LINE_WORDS; i++)
            {
            ret=file.extractNextWord(line_p, &indexWord, word_p, ','); //en word_p[] queda la siguienete palabra de la linea
            if(ret==-1 || word_p[0]==0) break;                      //linea vacía o de comentario
            file.strToLowCase(word_p, lowCaseWord);         //palabra a minúsculas
            if(strcmp(lowCaseWord, ".org")==0)  //directiva de equivalencia
                {
                ret=file.extractNextWord(line_p, &indexWord, word_p, ','); //en word_p[] queda la siguiente palabra de la linea
                if(ret==-1 || word_p[0]==0) break; 
                addr=cnv.getValueFromText(word_p);
                lineCount=-1;
                }
            else if(strcmp(lowCaseWord, "add")==0)  //directiva de equivalencia
                {
                ret=file.extractNextWord(line_p, &indexWord, word_p, ','); //palabra 0
                if(ret==-1 || word_p[0]==0) break; 
                if(getValue(word_p, &value1)==false) 
                {
                    sprintf(txt, "Warning: the %s %s", word_p, labelMessage);
                    QMessageBox::warning(0, "Files", txt, QMessageBox::Cancel);
                    return -1;
                }

                ret=file.extractNextWord(line_p, &indexWord, word_p, ','); //palabra 0
                if(ret==-1 || word_p[0]==0) break; 
                if(getValue(word_p, &value2)==false) 
                {
                    sprintf(txt, "Warning: the %s %s", word_p, labelMessage);
                    QMessageBox::warning(0, "Files", txt, QMessageBox::Cancel);
                    return -1;
                }

                cnv.valueToHexa(addr, txt1, 7);
                cnv.valueToHexa(value1, txt2, 7);
                cnv.valueToHexa(value2, txt3, 7);
                fprintf(fp2, "%s\tADD %s, %s\n", txt1, txt2, txt3);

                unsigned int cd=code(0, value1, value2);
                cnv.valueToHexa(cd, txt2, 16);                
                fprintf(fp3, "%s\t%s\n",txt1, txt2);
                addr++;
                }
            else if(strcmp(lowCaseWord, "mov")==0)  //directiva de equivalencia
                {
                ret=file.extractNextWord(line_p, &indexWord, word_p, ','); //palabra 0
                if(ret==-1 || word_p[0]==0) break; 
                if(getValue(word_p, &value1)==false) 
                {
                    sprintf(txt, "Warning: the %s %s", word_p, labelMessage);
                    QMessageBox::warning(0, "Files", txt, QMessageBox::Cancel);
                    return -1;
                }

                ret=file.extractNextWord(line_p, &indexWord, word_p, ','); //palabra 0
                if(ret==-1 || word_p[0]==0) break; 
                if(getValue(word_p, &value2)==false) 
                {
                    sprintf(txt, "Warning: the %s %s", word_p, labelMessage);
                    QMessageBox::warning(0, "Files", txt, QMessageBox::Cancel);
                    return -1;
                }

                cnv.valueToHexa(addr, txt1, 7);
                cnv.valueToHexa(value1, txt2, 7);
                cnv.valueToHexa(value2, txt3, 7);
                fprintf(fp2, "%s\tMOV %s, %s\n", txt1, txt2, txt3);

                unsigned int cd=code(2, value1, value2);
                cnv.valueToHexa(cd, txt2, 16);                
                fprintf(fp3, "%s\t%s\n",txt1, txt2);
                addr++;
                }
            else if(strcmp(lowCaseWord, "cmp")==0)  //directiva de equivalencia
                {
                ret=file.extractNextWord(line_p, &indexWord, word_p, ','); //palabra 0
                if(ret==-1 || word_p[0]==0) break; 
                if(getValue(word_p, &value1)==false) 
                {
                    sprintf(txt, "Warning: the %s %s", word_p, labelMessage);
                    QMessageBox::warning(0, "Files", txt, QMessageBox::Cancel);
                    return -1;
                }

                ret=file.extractNextWord(line_p, &indexWord, word_p, ','); //palabra 0
                if(ret==-1 || word_p[0]==0) break; 
                if(getValue(word_p, &value2)==false) 
                {
                    sprintf(txt, "Warning: the %s %s", word_p, labelMessage);
                    QMessageBox::warning(0, "Files", txt, QMessageBox::Cancel);
                    return -1;
                }

                cnv.valueToHexa(addr, txt1, 7);
                cnv.valueToHexa(value1, txt2, 7);
                cnv.valueToHexa(value2, txt3, 7);
                fprintf(fp2, "%s\tCMP %s, %s\n", txt1, txt2, txt3);
                
                unsigned int cd=code(1, value1, value2);
                cnv.valueToHexa(cd, txt2, 16);                
                fprintf(fp3, "%s\t%s\n",txt1, txt2);
                addr++;
                }
            else if(strcmp(lowCaseWord, "beq")==0)  //directiva de equivalencia
                {
                ret=file.extractNextWord(line_p, &indexWord, word_p, ','); //palabra 0
                if(ret==-1 || word_p[0]==0) break; 
                if(getValue(word_p, &value2)==false) 
                {
                    sprintf(txt, "Warning: the %s %s", word_p, labelMessage);
                    QMessageBox::warning(0, "Files", txt, QMessageBox::Cancel);
                    return -1;
                }

                cnv.valueToHexa(addr, txt1, 7);
                cnv.valueToHexa(value2, txt2, 7);
                fprintf(fp2, "%s\tBEQ %s\n", txt1, txt2);
            
                unsigned int cd=code(3, 0, value2);
                cnv.valueToHexa(cd, txt2, 16);                
                fprintf(fp3, "%s\t%s\n",txt1, txt2);
                addr++;
                }
        }
    }
    fprintf(fp2, "\n;-- Data --\n\n");
    fprintf(fp3, "\n\n");
    for(int i=0; i<m_dataIndex; i++)
        {
        cnv.valueToHexa(m_data[i].addr,  txt1, 7);
        cnv.valueToHexa(m_data[i].value, txt2, 16);
        fprintf(fp2, "%s\t.word %s\n", txt1, txt2);            
        fprintf(fp3, "%s\t%s\n", txt1, txt2);
        }

    if(fp)  fclose(fp);
    if(fp2) fclose(fp2);
    if(fp3) fclose(fp3);
    return 0;    
}
 
//busca en los arrays m_label y m_equ la existencia de una etiqueta y retorna su valor
bool Files::getValue(char *txt, unsigned int*value)
{
    bool hit=false;
    for(int i=0; i<m_labelIndex; i++)
        if(strcmp(txt, m_label[i].txt)==0)
            {hit=true; *value=m_label[i].value;}
    if(!hit)
    for(int i=0; i<m_equIndex; i++)
        if(strcmp(txt, m_equ[i].txt)==0)
            {hit=true; *value=m_equ[i].value;}
    return hit;
}

//Formato CO[15:14] source[13:7] destination[6:0]
unsigned int Files::code(char CO, unsigned int source, unsigned int destination)
{
    unsigned int data=0;
    data=CO;
    data<<=14;
    data|=source<<7;
    data|=destination;
    return data;
}


    
    
