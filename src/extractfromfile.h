/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 *
 *     Gestion de textos en ficheros
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


#ifndef EXTRACT_FROM_FILE_H
#define EXTRACT_FROM_FILE_H

//Extrae de una fichero la informacion que contiene
//cada linea de informacion se compone de una palabra seguida de un valor
//ese valor puede ser numérico (real) o las palabras 'yes' y 'no'
//las lineas que comienzan por '#' se consideran de comentario
//pueden existir lineas en blanco

//Recibe el nombre del fichero y un puntero a un array de punteros a string con las
//palabras reservadas a buscar en el fichero como primera palabra de linea.
//el último string del array debe estar vacío como indicador del fin.
//Retorna un array de reales con los valores indexados en el mismo orden que el
//indicado en el array de string
//Cada linea del fichero puede contener varios valores que son asignados sonsecutivamente
//como si de un array bidimensional se tratara

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
 
#define MAX_ARRAY 	500 	//longitud de array

class extractFromFile
{
public:
	//Constructor
	//Por defecto, el caracter de comentario es '#'
	extractFromFile();

	//Constructor
	// acepta el nombre del fichero con la info de configuracion, la lista de palabras
	//reservadas y el array de reales donde quedara la info extraida
	extractFromFile(const char *fileName, const char ** listName, float* result, char comment);

	//Destructor
	//libera memoria reservada
	~extractFromFile();

	//establece un nuevo caracter de comentario
	//retorna el anterior
	char setComment(char comment);
	 
	//carga en el array 'm_result' los valores (tipo float) obtenidos del fichero 'm_fileName'
	//el orden es el indicado en el array de punteros 'm_listNames'
	int extract3DConfig(void);

	//si info==true => se presentan mensajes de error
	//retorna el estado anterior
	bool setErrorMessages(bool info = true);

	//carga en el array 'm_result' los valores (tipo float) obtenidos de la linea siguinte en fichero pasado
	//el orden es el indicado en el array de punteros 'm_ListName'
	//La info 'yes' se convierte a 1.0 y 'no' a 0.0
	//Por defecto, todas las entradas al array son nulas
	//Retorna 0->OK; -1->problemas de lectura en file; -2->fin de fichero
	int extractLineParams(FILE *fp);


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
	int extractNextLine(FILE *file_p, char *str, int strLen);

	//extrae la primera palabra que aparece en 'strLine' a partir de 'index' y la retorna en 'strWord'
	//Actualiza index para la siguiente busqueda
	//retorna. 
	//  0 -> todo OK
	// -1 -> cadena vacia 
	//Se consideran como elementos separadores de palabras a ' ', '\t', y newSeparator, si existe
	// todo el texto situado entre comillas dobles se considera una sola palabra y se retorna como un string
	int extractNextWord(char *strLine, int *index, char *strWord, char newSeparator='^');

	// buscamos coincidencias entre el string 'str' y 'm_listNames'
	// retorna:
	// valor positivo -> indice de 'm_listNames' donde se da la coindicencia
	// -1 -> no se encontro coinidencia
	// el ultimo elemento del array de punteros debe ser un NULL
	int searchString(char *str);

	//convierte el string 'strIn' a minusculas en 'strOut'
	void strToLowCase(char *strIn, char *strOut);

	//retorna el primer índice donde se encuentra la coincidencia de un caracter en un string
	//si no se encuentra, retorna -1
	int searchChar(char *str, char c);

float		*m_result;			//para los constructores
char		*m_fileName;		//nombre de fichero
const char	**m_listNames;		//lista de nombres
int 		m_nLines,			//lineas válidas en fichero de configuración
			m_nValuesPerLine;	// valures por linea
char 		m_comment;			//caracter de comentario
bool		m_messages;			//si true=> presenta mensages de error
};
#endif

