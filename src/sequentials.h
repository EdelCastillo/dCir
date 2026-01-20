/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 *
 *    Manejo de bloques secuenciales
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

#ifndef SEQUENTIALS_H
#define SEQUENTIALS_H

#include "paths.h"
#include "blocks.h"
#include "types.h"

//Case para el manejo de bloques secuenciales
class Sequentials
{
public:
    //Constructor
    Sequentials(class Blocks *blocks_p);

    //Gestion de flip-flops tipo D
    void FFD(int bk);

    //Gestion de flip-flops tipo T
    void FFT(int bk);

    //Gestion de flip-flops tipo JK
    void FFJK(int bk);

    //Gestion de latches
    void LATCH(int bk);

    //Gestion de tabla de la verdad para FSM
    void FSM(int bk);

    //Gestion de tabla de la verdad para FSM en caso de cambio la entrada (Mealy)
    void FSM_out(int bk);

    BLOCK           *m_blocks_p;    //puntero a estructura BLOCK
    PATH            *m_paths_p;     //puntero a estructura PATH
    class Blocks    *m_cBlocks_p;   //puntero a clase Blocks
    
};
#endif
