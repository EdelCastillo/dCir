/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 *
 *     Gestión de textos en ficheros
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

#include "extractfromfile.h" 

//Constructor
//Por defecto, el caracter de comentario es '#'
extractFromFile::extractFromFile()
	{m_fileName=NULL; m_comment='#';}

// acepta el nombre del fichero con la info de configuracion y la estructura donde quedara la info extraida
extractFromFile::extractFromFile(const char *fileName, const char ** listNames, float* result, char comment)
{
	//m_fileName=new char[strlen(fileName)+1];
	//char buffer[strlen(fileName)+1];
	//m_fileName=buffer;
    
	setErrorMessages(true); //por defecto, se presentan mensages de error

	m_fileName=new char[strlen(fileName)+1];
	strcpy(m_fileName, fileName);

	m_listNames=listNames;
	m_comment=comment;
	m_result=result;
	// inicialización de array de floats: gestionamos la aritmética bidimensional del array unidimensional
	// La dimensión de result viene en su interior
	m_nLines=(int)result[0];
	m_nValuesPerLine=(int)result[1];
	for(int i=0; i<m_nLines; i++)
		for(int j=0; j<m_nValuesPerLine; j++)
			*(result+(i*m_nValuesPerLine)+j)=0.0;
}
 
extractFromFile::~extractFromFile()
{
	//printf(".........................extractFromFile destructor\n");
	if(m_fileName) delete []m_fileName;
}

//si info==true => se presentan mensajes de error
//retorna el estado anterior
bool extractFromFile::setErrorMessages(bool info)
{
	 bool oldInfo=m_messages;
	 m_messages=info;   
	 return oldInfo;
}

//establece un nuevo caracter de comentario
//retorna el anterior
char extractFromFile::setComment(char comment)
	{char tmp=m_comment; m_comment=comment; return tmp;}

//  extract3DConfig(void)
////////////////////////////////
//carga en el array 'm_result' los valores (tipo float) obtenidos del fichero 'm_fileName'
//el orden es el indicado en el array de punteros 'm_ListName'
int extractFromFile::extract3DConfig(void)
{

char strLine[MAX_ARRAY];
char strWord[MAX_ARRAY];
char *endChar;

int out, error, alerta=0, alerta2=0;
int indexLine=0, indexWord=0, indexStruct=0;
int lineNumber=0;
FILE *fp;
float  value;
char strLowWord[MAX_ARRAY];

if((fp=fopen(m_fileName, "r"))==NULL)
	return -1;
while(true)
	{
	error=extractNextLine(fp, strLine, MAX_ARRAY);
	lineNumber++;
	switch(error)
		{
		case -3:
            if(m_messages)
                fprintf(stderr, "Problemas al leer en el fichero %s\n", m_fileName);
			out=-1;
			break;
		case -2:
            if(m_messages)
                fprintf(stderr, "La línea %d es demasiado larga en %s\n", lineNumber, m_fileName);
//			fprintf(stderr, "Error en reserva de memoria\n\tPosible línea demasiado larga en %s\n", m_fileName);
			//out= -1;
			out=0; //warning: no se trata como un error
			continue;
			break;
		case -1:
			//printf("Localizado el fin del fichero antes de fin de linea\n");
			out=0;
			alerta=true;//ultima linea
			break;
		case 0:
			out=0;
			break;
		}
	if(out!=0) break;
	if(strLine[0]==m_comment) continue;  //linea de comentario
	indexWord=0;
	indexLine=0;
	while((error=extractNextWord(strLine, &indexLine, strWord))!=-1) //hasta que se lea toda la line
		{
		if(strlen(strLine)<2) continue; //no interesa esa linea, muy pequeña
		if(indexWord==0)
			{
			if((indexStruct=searchString(strWord))==-1)
				{
				alerta2++;
				//fprintf(stdout, "El string %s no esta identificado\n", strWord);
				break;
				}
			indexWord++;
			}
		else
			{
			strToLowCase(strWord, strLowWord);
			if(strcmp(strLowWord, "yes")==0)
				{
				value=true;
				}
			else if(strcmp(strLowWord, "no")==0)
				{
				value=false;
				}
			else	{//Se aceptan puntos y comas como elementos separadores de los posibles valores decimales
				value=strtof(strLowWord, &endChar); //Puede que el elemento separador de decimales sea '.'
				if(*endChar=='.') {*endChar=','; value=strtof(strLowWord, &endChar);}
				//value=(float)atof(strLowWord);
				}
			if((indexWord-1)<m_nValuesPerLine)
				{
				//aritmética bidimensional en array unidimensional
				*(m_result+(indexStruct*m_nValuesPerLine)+(indexWord-1))=value;
				}
			else
				{
                if(m_messages)
                    fprintf(stderr, "Error en linea %d. Solo de aceptan %d valores por línea\n", lineNumber, m_nValuesPerLine);
				break;
				}
			indexWord++;
			}
		}
	if(alerta) break;  //fin fichero
	}
fclose(fp);  //cierre de fichero
if (alerta2 && m_messages)
  fprintf(stderr, "Existen %d entradas no identificadas en %s\n", alerta2, m_fileName);
return out;
}



////////////////////////////////
//carga en el array 'm_result' los valores (tipo float) obtenidos de la linea siguinte en fichero pasado
//el orden es el indicado en el array de punteros 'm_ListName'
//La info 'yes' se convierte a 1.0 y 'no' a 0.0
//Por defecto, todas las entradas al array son nulas
//Retorna 0->OK; -1->problemas de lectura en file; -2->fin de fichero
int extractFromFile::extractLineParams(FILE *fp)
{
  char strLine[MAX_ARRAY];
  char strWord[MAX_ARRAY];
  char *endChar;

  int out, error, alerta=0, alerta2=0;
  int indexLine=0, indexWord=0, indexStruct=0;
  static int lineNumber=0;
  float  value;
  char strLowWord[MAX_ARRAY];

for(int i=0; i<m_nLines; i++)
	for(int j=0; j<m_nValuesPerLine; j++)
		*(m_result+(i*m_nValuesPerLine)+j)=-1.0; //inicialización
while(true)
	{
	//  0 -> OK
	// -1 -> fin de fichero antes de fin de linea
	// -2 -> la linea no cabe en el string
	// -3 -> error al leer en el fichero  
	error=extractNextLine(fp, strLine, MAX_ARRAY);
	lineNumber++;
	switch(error)
		{
		case -3:
            if(m_messages)
                fprintf(stderr, "Problemas al leer en el fichero %s\n", m_fileName);
			out=-1;
			break;
		case -2:
            if(m_messages)
                fprintf(stderr, "La línea %d es demasiado larga en %s\n", lineNumber, m_fileName);
//			fprintf(stderr, "Error en reserva de memoria\n\tPosible línea demasiado larga en %s\n", m_fileName);
			//out= -1;
			out=0; //warning: no se trata como un error
			continue;
			break;
		case -1:
			//printf("Localizado el fin del fichero antes de fin de linea\n");
			out=-2;
			alerta=true;//ultima linea
			break;
		case 0: //OK
			out=0;
			break;
		}
	if(out!=0) break;
	if(strLine[0]==m_comment) continue;  //linea de comentario
	indexWord=0;
	indexLine=0;
	while((error=extractNextWord(strLine, &indexLine, strWord))!=-1) //hasta que se lea toda la line
		{
		if(strlen(strLine)<2) continue; //no interesa esa linea, muy pequeña
		if(indexWord==0)
			{
			if((indexStruct=searchString(strWord))==-1)
				{
				alerta2++;
				//fprintf(stdout, "El string %s no esta identificado\n", strWord);
				break;
				}
			indexWord++;
			}
		else
			{
			strToLowCase(strWord, strLowWord);
			if(strcmp(strLowWord, "yes")==0)
				{
				value=true;
				}
			else if(strcmp(strLowWord, "no")==0)
				{
				value=false;
				}
			else	{//Se aceptan puntos y comas como elementos separadores de los posibles valores decimales
				value=strtof(strLowWord, &endChar); //Puede que el elemento separador de decimales sea '.'
				if(*endChar=='.') {*endChar=','; value=strtof(strLowWord, &endChar);}
				//value=(float)atof(strLowWord);
				}
			if((indexWord-1)<m_nValuesPerLine)
				{
				//aritmética bidimensional en array unidimensional
				*(m_result+(indexStruct*m_nValuesPerLine)+(indexWord-1))=value;
				}
			else
				{
                if(m_messages)
                    fprintf(stderr, "Error en linea %d. Solo de aceptan %d valores por línea\n", lineNumber, m_nValuesPerLine);
				break;
				}
			indexWord++;
			}
		}
//	if(alerta) break;  //fin fichero
	break; //fin de analisis de linea
	}
if (alerta2 && m_messages)
  fprintf(stderr, "Existen %d entradas no identificadas en %s\n", alerta2, m_fileName);
return out;
}

//  extractNexLine()
//////////////////////////
// extrae una linea del fichero
// argumentos:
// file_p -> puntero a fichero
// str -> puntero a string donde quedara la linea extraida
// strLen -> tamano del string
// Salida:
//  0 -> OK
// -1 -> fin de fichero antes de fin de linea
// -2 -> la linea no cabe en el string
// -3 -> error al leer en el fichero
int extractFromFile::extractNextLine(FILE *file_p, char *str, int strLen)
{
int out=0, i=0; // fin de linea
char c;

strLen-=1; //espacio para caracter de fin de string
while((c=getc(file_p))!='\n')
	{
	if(ferror(file_p)) {out=-3; break;}   //error en fichero
	if(feof(file_p)) {out=-1; break;}      //fin de fichero
	if(i<strLen)
		str[i++]=c;
	else{
	    while((c=getc(file_p))!='\n');//se acaba de leer la linea sin almacenar el exceso
	    return -2;  //error en dimensionado del string
	    }
	}

//if(out==0 || out ==-1)
str[i]='\0';
if(i>1 && str[i-1]==0x0D) str[i-1]='\0'; //resuelve problemas con CR

return out;
}

//  extractNexWord()
/////////////////////////////
//extrae la primera palabra que aparece en 'strLine' a partir de 'index' y la retorna en 'strWord'
//Actualiza index para la siguiente busqueda
//retorna. 
//  0 -> todo OK
// -1 -> cadena vacia 
//Se consideran como elementos separadores de palabras a ' ', '\t', y newSeparator, si existe
// todo el texto situado entre comillas dobles se considera una sola palabra y se retorna como un string
int extractFromFile::extractNextWord(char *strLine, int *index, char *strWord, char newSeparator)
{
int out=0, iLine, iWord;
bool inWord=false, txt=false;

for(iLine=*index, iWord=0; ;)
	{
	if(!inWord)
		{
		while(strLine[iLine]==' ' || strLine[iLine]=='\t') iLine++;
		if(strLine[iLine]=='\0'||strLine[iLine]==m_comment) {out=-1; break;} // cadena vacia -> no más palabras a buscar
        if(strLine[iLine]=='"') {strWord[iWord++]='"'; iLine++; txt=true;}
		inWord=true; //indice dentro de la palabra
		}
    else if(txt==true && strLine[iLine]=='"') //si fin de string
		txt=false;
	else if(txt==false && (strLine[iLine]==' ' || strLine[iLine]=='\t'|| strLine[iLine]==newSeparator || strLine[iLine]=='\0'))
		{strWord[iWord]='\0'; out=0; if(strLine[iLine]==newSeparator) iLine++; break;}
	else strWord[iWord++]=strLine[iLine++];
	}
*index=iLine; //actualizamos index para busqueda futura
return out;
}

//  searchString()
////////////////////////
// buscamos coincidencias entre el string 'str' y 'm_listNames'
// retorna:
// valor positivo -> indice de 'm_listNames' donde se da la coindicencia
// -1 -> no se encontro coinidencia
// el ultimo elemento del array de punteros debe ser un NULL
int extractFromFile::searchString(char *str)
{
int i=0;
while(*m_listNames[i]!='\0')//cadena vacia->fin de busqueda
	{
	if(strcmp(str, m_listNames[i])==0)
		return i;  //coincidencia en cadena i
	i++;
	}
return -1; //no se encontro la cadena
}

//  strToLowCase()
/////////////////////////
//convierte el string 'strIn' a minusculas en 'strOut'
void extractFromFile::strToLowCase(char *strIn, char *strOut)
{
int i;

for(i=0; strIn[i]!=0; i++)
	{
	if(isalpha(strIn[i]))
		strOut[i]=(char)tolower(strIn[i]);//se trata de letras
	else
		strOut[i]=strIn[i]; //si no son letras
	}
strOut[i]='\0';
}

//retorna el primer índice donde se encuentra la coincidencia de un caracter en un string
//si no se encuentra, retorna -1
int extractFromFile::searchChar(char *str, char c)
{
for(int i=0; i<strlen(str); i++)
    if(str[i]==c) return i;
return -1;
}
