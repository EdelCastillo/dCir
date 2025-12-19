/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 *
 *   Manejo de ficheros true table y ensamblador para maquina sencilla
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

#ifndef FILES_H
#define FILES_H

#include "types.h"
#include "extractfromfile.h"
#include "numeric.h"
#include <QMessageBox>
#include "common.h"

typedef struct
{
     char           txt[20];    //etiqueta
     unsigned int   value;      //valor numerico
}LABEL;

typedef struct
{
     unsigned int   addr,   //direccion
                    value;  //contenido
}DATA;

//Clase para la gestion de ficheros especificos *.tt y ensamblador para maquina sencilla
class Files
{
public: 
    //Constructor (vacio)
    Files();

    //Carga una tabla de la verdad desde un fichero
    //el fichero, en path completo, viene dentro de la estructura
    int fileToMemory(TRUE_TABLE *TT);

    //Carga una tabla de la verdad desde un fichero
    //el fichero, en path completo, viene dentro de la estructura
    //Las columnas tienen formato
    //state  input  status+ output
    //Se inserta una nueva columna luego de input que incluye una mascara
    //esa mascara de construye a partir de input haciendo que los bits 'x' sean '0' y el resto '1'
    int fileToTrueTable(TRUE_TABLE *TT, bool FSM);

    //Carga una tabla de la verdad desde un fichero
    //el fichero, en path completo, viene dentro de la estructura
    //Carga tantas columnas como existan sin alteraciones
    int fileToCombi(TRUE_TABLE *TT);

    //ensamblador para maquina sencilla
    int ensambla(char *fileNameProject);

    //busca en los arrays m_label y m_equ la existencia de una etiqueta y retorna su valor
    bool getValue(char *txt, unsigned int*value);

    //Formato CO[15:14] source[13:7] destination[6:0]
    unsigned int code(char CO, unsigned int source, unsigned int destination);
    
    LABEL m_label[20];
    DATA  m_data [20];
    LABEL m_equ[20];
    int     m_labelIndex,
            m_dataIndex,
            m_equIndex;
};
#endif
