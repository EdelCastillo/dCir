/***********************************************
*   Proyecto "digital circuits simulator (dCir)"
*   Copyright (C) 2024 by Esteban del Castillo
*   esteban.delcastillo@urv.cat  julio-diciembre 2024
*
*    Declaracion de tipos comunes
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

#ifndef TYPES_H
#define TYPES_H
  
#include <QtWidgets>

#define MAX_PATHS           100 //# máximo de caminos
#define MAX_POINTS_PATH     50  //# máximo de puntos por camino
#define MAX_BLOCKS          50  //# máximo de bloques
#define MAX_BLOCKS_PATHS    10  //# máximo de caminos en entradas y salidas por bloques
#define MAX_BLOCK_IN        10  //# máximo de bloques de entrada
#define MAX_LINE_WORDS      20  //# máximo de palabras por línea
#define MAX_CHARACTER_WORD  200 //# máximo de caracteres por palabra
#define MAX_SIZE_LINE       200 //# máximo de caracteres por línea
#define MAX_TAPS_PATH        20 //# máximo de caracteres por línea

enum class Type {COMBI, SEQ, NODE, IN_PORT, OUT_PORT, TAB, MEMORY};             //tipos de bloques
enum class Oper {NOT, AND, OR, XOR, NAND, NOR, XNOR, ADD, MUX, CMP, OTHERS, DMUX, DECOD, DECODN};    //tipos de bloques combinacionales
enum class FF { FFD, FFT, FFJK, LATCH, FSM};                                    //tipos de bloques secuenciales

typedef unsigned char uchar;

//Novedades en caminos
typedef struct{
    bool    news;       //true si existe novedad
    char    type;       //1->la info llegó al final del camino; 2->la info llegó a una bifurcación
    int     block;      //bloque que recibe la novedad
}PATH_NEWS;

//Bifurcación sobre un camino principal
typedef struct{
    int     tab,    //bloque con el que conecta
            pos;    //posición de la bifurcación en #fechas
    bool    pass;   //la flecha ha pasado
}FORK;
  
//texto general para la imagen
typedef struct{
    int     value;   //valor digital del camino
    bool    unKnow;  //true=valor desconocido
    QPointF pos;     //posición
    double  angle;   //ángulo
    QColor  color;   //color
}TEXT;

//información asociada a un segmento del camino
typedef struct{
    QPointF pIni,                   //punto de inicio
            pEnd,                   //punto de fin
            deltaSeg;               //incremento de posición para la siguiente flecha
    double  angle;                  //ángulo
    int     nDeltaSeg,              //# de flechas que puede alojar
            currentDeltaSeg;        //flecha actual
    FORK    forks[MAX_TAPS_PATH];//puntos de bifurcación en unidades de deltaSeg
    int        nForks;              //número de bifurcaciones
    QGraphicsLineItem    *lineItem; //item del segmento
    QGraphicsPolygonItem **arrowItem;//array de items para las flechas
}SEGMENT;

//Información asociada a un camino
typedef struct{
    SEGMENT *segment_p;                 //puntero al array de segmentos          
    int     nSegments,                  //# de segmentos
            currentSeg;                 //segmento en curso sobre el que se sitúan las flechas
    char    nBits;                      //bits del camino
    TEXT    valueIni,                   //valor inicial
            valueEnd;                   //valor final            
    QGraphicsSimpleTextItem *nameItem,  //item al nombre
                            *iniItem,   //item al valor de inicio
                            *endItem;   //item al valor de fin   
    int     blockIni,                   //bloque inicial del camino
            blockEnd;                   //bloque final   del camino
    bool    endPath,                    //true si la información llegó al final del camino
            start;                      //se inicia el avance de las flechas
}PATH;

//información asociada a un bloque secuencial
typedef struct{
    bool    start,          //clock: deben evaluarse las entradas si enable='1'
            enable;         //FF responde al clock
    char    type;           //tipo de FF: D, T, JK, LATCH y FSM
    int
            valueA,         //variable intermedia que aloja la información de entrada capturada en clock (D, T, J)
            valueB,         //variable intermedia que aloja la información de entrada capturada en clock (K)
            valueOut,       //valor de salida
            resetValue,     //valor inicial (se activa al comenzar la simulación)
            status;         //Mantiene el estado actual en bloque FSM
    bool    valueAUnknow,   //true si valor intermedio A desconocido
            valueBUnknow,   //true si valor intermedio B desconocido
            valueOutUnknow, //true si salida desconocida
            resetValueUnknow; //true si valor de reset desconocido
}FLIP_FLOP;
 
//información asociada a un bloque combinacional
typedef struct{
    char    type;           //tipo de puerta; NOT, AND, OR, XOR, NAND, NOR, XNOR, ADD, MUX, CMP, OTHERS
    int     tmpValue[MAX_BLOCKS_PATHS];       //valor temporal de la salida
    bool    tmpValueUnknow[MAX_BLOCKS_PATHS]; //true si valor temporal desconocido
    int     pathSelect[MAX_BLOCKS_PATHS], //caminos de selección para MUX, DEMUX, DECOD y DECODN
            pathSelectOut,  //camino de salida para dMUX
            nPathSelect;    //número de caminos de selección
    char    logicCmp;       //para bloque CMP (comparador lógico): 0->'<'(menor que); 1->'='(igual a), 2->'>'(mayor que)
}COMBINATIONAL;

//tamaño en bits de los caminos
typedef struct{         
       int  path,       //camino
            bitIni,     //bit de inicio (MSB)
            bitEnd;     //bit de fin    (LSB)
}BITS_PATH;

//para ficheros que alojan una tabla de la verdad (.tt)
typedef struct{         
    char *fileName; //nombre del fichero
    int **table_p,  //puntero a tabla 
        nRows,      //número de filas
        nCols;      //número de columnas
}TRUE_TABLE;

//información asociada a un bloque
typedef struct{
    char            type,                       //tipo de bloque: COMBI, SEC, NODE, IN_POT, OUT_PORT, TAB, MEMORY
                    nBits;                        
    int             pathIn[MAX_BLOCKS_PATHS],   //caminos que entran
                    pathOut[MAX_BLOCKS_PATHS],  //caminos que salen
                    pathEnable,                 //camino de validación
                    nPathIn,                    //# de caminos que entran
                    nPathOut;                   //# de caminos que salen
    BITS_PATH       nodePathOut[MAX_BLOCKS_PATHS]; //bits de inicio y fin de cada camino en blocks NODE
    BITS_PATH       nodePathIn [MAX_BLOCKS_PATHS]; //bits de inicio y fin de cada camino en blocks NODE
    int             delay,                      //retardo en activar su salida luego de una actualización (ms)
                    currentDelay;               //valor actual del retardo en unidades de deltaT_blocks
    QRect           rec;                        //rectángulo que circunscribe al bloque (zona de sensibilidad al ratón)
    QGraphicsRectItem *recItem;                 //item para el rectángulo
    QGraphicsSimpleTextItem *nameItem;          //item para el nombre del bloque
    int             initPortValue;              //valor inicial de la salida
    bool            initPortValueUnknow;        //true si valor inicial de la salida desconocido
    TEXT            blockName;                  //nombre del bloque
    FLIP_FLOP       FF;                         //info extra para bloque secuencial
    COMBINATIONAL   combi;                      //info extra para bloque combinacional
    bool            enable;                     //true si bloque operativo
    TRUE_TABLE      TT;                         //tabla de la verdad para bloques: OTHERS, FSM, MEMORY
}BLOCK;

//parámetros del proyecto
typedef struct{
    char    *graphicFile[200];          //camino absoluto al nombre del fichero con esquema digital
    int     velocity,                   //velocidad de avance de las flechas (pixels/s)
            deltaT_paths,               //intervalo para la evaluación de los caminos (ms)
            deltaT_blocks,              //intervalo para la evaluación de los bloques (ms)
            arrowType;                  //tipo de flecha:grande, mediana, pequeña
    QColor  arrowColor0,                //color para la flecha del '0'
            arrowColor1,                //color para la flecha del '1'
            pathColor,                  //color para el camino
            blockColor;                 //color para el bloque
                        
    bool    viewPaths,                  //flag=true ->visualiza la linea de los caminos
            viewPathsName,              //flag=true ->visualiza el nombre de los caminos
            viewIniValuePaths,          //flag=true ->visualiza el valor inicial de los caminos
            viewEndValuePaths,          //flag=true ->visualiza el valor final de los caminos
            viewNodeBlocksName,         //flag=true ->visualiza el nombre de los nodos
            viewPortBlocksName,         //flag=true ->visualiza el nombre de los puertos
            viewOthersBlocksName,       //flag=true ->visualiza el nombre de los bloque distintos de nodos y puertos
            viewRectBlocks;             //flag=true ->visualiza los rectángulos sobre los bloques    
}PARAMS;
#endif
