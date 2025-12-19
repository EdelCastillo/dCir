/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 *
 *    Tratamiento de bloques
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

#ifndef BLOCKS_H
#define BLOCKS_H

#include <QtWidgets>
#include "types.h"
#include "image.h"
#include "paths.h"
#include "blockDialog.h"
#include "common.h"
#include "extractfromfile.h"
#include "numeric.h"
#include "files.h"

class Blocks:public Paths
{
public:   
 
    //constructor
    //clase derivada de la clase Paths
    //recibe un puntero a la clase Image para la visualización gráfica
    Blocks(class Image *image_p);
    
    //destructor
    //elimina todos los items y libera la memoria reservada
    ~Blocks();
    
    //crea e inicializa la estructura de bloques como desactivados
    void createAllBlocks();
    
    //inicializa los bloques activos
    //si puerto de entrada o FF, traslada a los caminos de salida el valor de inicialización
    void initActiveBlocks();
    
    //retorna true si existe el camino
    //existe si la estructura está creada y el bloque está habilitado
    bool existBlock(int block);
    
    //retorna true si existe el nombre del bloque
    //existe si la estructura está creada y ya se creó el nombre
    bool existNameBlock(int block);
    
    //elimina un bloque
    //elimina los items y desactiva el bloque
    void removeBlock(int block);
    
    //elimina todos los rectángulos alrededor de los bloques a menos que sean puertos de entrada o salida
    void removeAllBlocksRecItems();
    
    //elimina los nombres de todos los bloques
    void removeAllBlocksNameItems();
    
    //Determina si un camino existe en todos los bloque y lo elimina
    //retorna -1 si no existe ese camino en ningún bloque o no existe ningún bloque
    int  removePathFromAllBlocks(int path);
    
    //Hace que los nombres sobre los bloques NODE sean visibles o no
    void setNodeBlocksNameVisibles(bool visible);
    
    //Hace que los nombres sobre los bloques PORT sean visibles o no
    void setPortBlocksNameVisibles(bool visible);
    
    //Hace que los nombres sobre el resto de bloques sean visibles o no (distintos de NODE y PORT)
    void setOthersBlocksNameVisibles(bool visible);
    
    //Hace que los rectángulos sobre los bloques sean visibles o no
    void setRectBlocksVisibles(bool visible);
    
    //evalua la lista de bloques a la búsqueda del primero libre
    //Retorna el índice a la primera entrada libre o -1 si no existe o no está creada la entrada en m_blocks_p
    int  getFreeBlock();
    
    //recibe control al pulsar el clock
    //Se captura la info de entrada y se inicializa el reloj del retardo
    //no se actua en la salida hasta pasado el tiempo de retardo
    void clockAttn();
    
    //recibe control cada params.delatT_blocks
    //Se evaluan las entradas de cada bloque. Si hay cambios y ha transcurrido el retardo, se actualiza la salida
    void evaluateBlocks();
        
    //Dibuja un rectángulo sobre el bloque
    //El rectángulo se conforma a partir de la posición de los caminos que llegan y salen al bloque
    QPointF drawRectBlock(int block);
    
    //dibuja el nombre del bloque
    //Lo posiciona en un lugar central del bloque
    void drawBlockName(int block);
    
    //dibuja todos los nombres sobre boques NODE
    void drawAllOthersBlocksName();
    
    //dibuja todos los nombres sobre boques PORT
    void drawAllPortsBlocksName();
    
    //dibuja todos los nombres sobre boques NODE
    void drawAllNodeBlocksName();
    
    //dibuja todos los rectángulos sobre los bloques 
    void drawAllRectBlocks();
    
    //retorna el top item situado bajo el ratón            
    QGraphicsItem* getMouseItem();
    
    //mueve el texto cuando se arrastra el mouse
    void moveItem(QGraphicsItem* gItem);
    
    //Establece los bits de cada bloque
    void setAllBlockBits();

    //retorna los bits del camino de mayor número de bits de entre los que entran o salen al block
    //si el bloque no existe retorna -1
    int getBlockBits(int block);
   
    //retorna el punto de un camino más próximo al punto dado
    //en segment, se establece el segmento del camino donde se ubica el punto
    QPointF pointToPath(int path, QPointF point, int *segment=0);
   
    //analiza una info de texto y la descompone en path, bit inicial y bit final
    //ej:txt="2[1:0], 5[4:2]" bPath[0].path=2, bPath[0].bitIni=1, bPath[0].bitEnd=0; bPath[1].path=5..
    //Si no se especifica un ango de bits, se asume que la salida coincide con la entrada
    int getBitsPath(char *txt, BITS_PATH *bPath_p, char nBitsIn);
       
    //actualiza los bits en los bloques que conectan con un camino dado
    //Retorna -1 si no existe el camino o los bloque que conecta
    int updateBitsBlocks(int path);
    
    //revisa que todos los bloques activos sean correctos, de no serlo lo comunica
    //retorna false si algo no está bien
    bool isAllBlocksOK();
    
    //retorna true si el bloque no tienen ningún camino conectado
    bool isIsolatedBlock(int bk);
    
    class Image             *m_image_p;     //puntero a clase Image
    BLOCK                   *m_blocks_p;    //puntero a array de estructuras BLOCKs
    class Combinationals    *m_combi_p;     //puntero a clase de trato con sistemas combinacionales
    class Sequentials       *m_sequ_p;      //puntero a clase de trato con sistemas secuenciales
};
#endif

