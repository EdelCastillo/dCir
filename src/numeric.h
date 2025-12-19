/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 *
 *    Manejo de informacion numerica
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

#ifndef NUMERIC_H
#define NUMERIC_H

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//Clase para el manejo de informacion numerica
class Numeric
{
public:   
    //Constructor (vacio)
    Numeric();

    //retorna true si el texto contiene la secuencia 0b o 0B y valores '0', '1', 'x' o 'X'
    //puede estar precedido por espacios o tabuladores
    bool isBinary(char *txt);

    //Retorna el valor decimal correspondiente a la cadena de texto en binario
    //si no contiene ningun caracter valido, retorna cero
    unsigned int binaryToDecimal(char *txt);

    //retorna true si el texto contiene la secuencia 0x o 0X y valores '0'..'9', 'a'..'f', 'A'..'F', 'x', 'X'
    //puede estar precedido por espacios o tabuladores
    bool isHexa(char *txt);

    //Retorna el valor decimal correspondiente a la cadena de texto en hexadecimal
    //si no contiene ningun caracter valido, retorna 0
    unsigned int hexaToDecimal(char *txt);

    //compara dos cadenas de texto en formato binario
    //retorna true si son identicas o difieren en algun caracter pero una o ambas cadenas contiene 'x' 0 'X' en esa posicion
    bool isEqual_binary(char *txt1, char *txt2);

    //compara dos cadenas de texto en formato hexadecimal
    //retorna true si son identicas o difieren en algun caracter pero una o ambas cadenas contiene 'x' 0 'X' en esa posicion
    bool isEqual_hexa(char *txt1, char *txt2);

    //recibe un texto en formato binario (ej."0b10X1") o hexadecimal (ej."0x1AX0") y retorna un valor 
    //en el que los bits con 'x' se hacen cero, junto el numero de bits
    //aplicando esa mascara sobre cada nuevo valor dado se identifican los valores validos
    int valueToMask(char* txt, unsigned int *value, int *nBits, unsigned int *mask);

    //retorna true si valor existe en la lista buffer
    //si index_p!=0 establece la posicion donde se encuentra
    bool exist(int valor, int *buffer, int bufferSize, int *index_p=0);

    //acepta texto binario (0b/0B), hexadecimal (0x/0X) y decimal (si no incluye prefijo)
    //Retorna su valor o cero si no existe ningun valor
    unsigned int getValueFromText(char *text);

    //convierte un valor numerico a hexadecimal en formato de texto
    void valueToHexa(unsigned int value, char *text, int nBits);
};

#endif
