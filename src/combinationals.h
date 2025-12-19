/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 *
 *    Tratamiento de info en bloques combinacionales
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

#ifndef COMBINATIONALS_H
#define COMBINATIONALS_H

#include "paths.h"
#include "blocks.h"
#include "types.h"

class Combinationals
{
public:
    //Constructor, recibe un puntero a clase de tartamiento con bloques
    Combinationals(class Blocks *cBlocks_p);
    
    //puerta NOT
    //una sola entrada de ancho variable
    void NOT(int bk);
    
    //puerta AND
    //Si más de una entrada, se hace la operación por columnas y la salida es del mismo ancho que las entradas
    //Si sólo una entrada,   se hace la operación por bits en la fila y la salida es de un solo bit
    void AND(int bk);
    
    //puerta OR
    //Si más de una entrada, se hace la operación por columnas y la salida es del mismo ancho que las entradas
    //Si sólo una entrada,   se hace la operación por bits en la fila y la salida es de un solo bit
    void OR(int bk);
    
    //puerta XOR
    //Si más de una entrada, se hace la operación por columnas y la salida es del mismo ancho que las entradas
    //Si sólo una entrada,   se hace la operación por bits en la fila y la salida es de un solo bit
    void XOR(int bk);
    
    //Puerta NAND
    //Si más de una entrada, se hace la operación por columnas y la salida es del mismo ancho que las entradas
    //Si sólo una entrada,   se hace la operación por bits en la fila y la salida es de un solo bit
    void NAND(int bk);
    
    //Puerta NOR
    //Si más de una entrada, se hace la operación por columnas y la salida es del mismo ancho que las entradas
    //Si sólo una entrada,   se hace la operación por bits en la fila y la salida es de un solo bit
    void NOR(int bk);
    
    //Puerta XOR
    //Si más de una entrada, se hace la operación por columnas y la salida es del mismo ancho que las entradas
    //Si sólo una entrada,   se hace la operación por bits en la fila y la salida es de un solo bit
    void XNOR(int bk);
    
    //Suma 
    //requiere de más de una entrada
    void ADD(int bk);
    
    //Multiplexor
    //Si más de una entrada, se hace la selección de una de ellas y la salida es del mismo ancho que las entradas
    //Si sólo una entrada,   se hace la selección de un bit en la fila que sale
    void MUX(int bk);

    //deMultiplexor
    //Si más de una salida, se hace la selección de una de ellas y la salida es del mismo ancho que la entrada
    //Si sólo una salida,   se hace la selección de un bit en la fila que sale
    void DMUX(int bk, int subtype);

    //Comparador
    //requiere de dos entradas del mismo ancho
    //Según .logicCmp= '0' -> menor que; '1' -> igual a; '2' -> mayor que
    void CMP(int bk);
    
    //pestaña que separa un camino de otro principal. Pueden ser de ancho distinto
    void TAB(int bk);
    
    //separador/concentrador de caminos
    //Un camino de entrada y varios de salida de ancho variable
    //Varios caminos de entrada de ancho variable y uno de salida
    void NODE(int bk);
    
    //Gestión de la memoria
    //se trata de un bloque sin clock pero surge un problema con la escritura si no se toman medidas.
    //para que se permita la escritura debe de haber novedad en la linea de control a '1' (sincronismo con control)
    //si la novedad se produce sólo en la linea de datos de entrada, no se hace nada.
    void MEMORY(int bk, bool clock);
    
    //Tabla de la verdad con dos columnas (entradas y salidas)
    void OTHERS(int bk);
    
    BLOCK           *m_blocks_p;    //puntero a estructura BLOCK      
    PATH            *m_paths_p;     //puntero a estructura PATH
    class Blocks    *m_cBlocks_p;   //puntero a clase Blocks
};
#endif
