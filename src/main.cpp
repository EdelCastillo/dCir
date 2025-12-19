/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 *
 *    Entrada a la aplicacion
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
 
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "dCir.h"

#include <QApplication>

#include <files.h>

//Inicio del proyecto
/////////////////////////////////
int main(int argc, char **argv)
{   
const char *errorMessage="Error in memory assigment\n";
char *formato=(char*)"\nSoftware for digital circuits\n \tformat: ./dCir ";
printf("by Esteban del Castillo (esteban.delcastillo@urv.cat)\n");
if(argc<1)
	{	printf("%s", formato);
	return -1;
	}


  QApplication app(argc, argv);
  Gui window(argc, argv); //clase principal derivada de QMainWindow 
 /*
       QScreen *screen = QGuiApplication::primaryScreen();
       QRect screenGeometry = screen->geometry();
       int height = screenGeometry.height();
       int width = screenGeometry.width();
    width *= 1; // 90% of the screen size
    height *= 1; // 90% of the screen size
    QRect rec(0, 0, width, height);
    window.setGeometry(rec);
*/    
//  window.setMinimumSize(500, 500);
 // window.setMinimumSize(width * 0.7, height / 2);

  window.show();
  app.exec();

return(0);

}



