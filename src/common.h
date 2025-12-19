#ifndef COMMON_H 
#define COMMON_H

#include <math.h>
//#include <types.h>
#include <stdio.h>
#include <float.h>
#include <QtWidgets>
//#include <QtGui>
#include <ctype.h>
#include <string.h>

class Common
  {
  public:
    Common();
    ~Common();
    //Retorna el indice en el buffer ocupado por 'data' o -1 si no existe
    //busqueda por aproximaciones sucesivas
    //Requiere que los elementos en 'buffer' esten ordenados de menor a mayor
    int getIndexInSortedList(int data, int *buffer, int bufferSize);

    //ordena de mayor a menor la lista 'bufferIn' de numeros enteros>=0 y la retorna en 'sort'
    //retorna en sort una cadena de indices a bufferIn tal que la secuencia resulta ordenada de mayor a menor
    int sortDown(int *bufferIn, int *sort, int size);

    //ordena de mayor a menor la lista 'bufferIn' de numeros enteros>=0 y la retorna en 'sort'
    //retorna en sort una cadena de indices a bufferIn tal que la secuencia resulta ordenada de mayor a menor
    int sortDown(float *bufferIn, int *sort, int size, bool absolutas=false);

    //ordena de mayor a menor la lista 'bufferIn' de numeros enteros>=0 y la retorna en 'sort'
    //retorna en sort una cadena de indices a bufferIn tal que la secuencia resulta ordenada de mayor a menor
    int sortDown(double *bufferIn, int *sort, int size, bool absolutas=false);

  //retorna en sort una cadena de indices a bufferIn tal que la secuencia resulta ordenada de menor a mayor
  int sortUp(int *bufferIn, int *sort, int size);

  //retorna en sort una cadena de indices a bufferIn tal que la secuencia resulta ordenada de menor a mayor
  int sortUp(float *bufferIn, int *sort, int size);

  //retorna en sort una cadena de indices a bufferIn tal que la secuencia resulta ordenada de menor a mayor
  int sortUp(double *bufferIn, int *sort, int size);

  //retorna true si valor existe en la lista buffer
  //si index_p!=0 establece la posicion donde se encuentra
  bool exist(int valor, int *buffer, int bufferSize, int *index=0);

  //valor medio de un array de floats
  float mean(float *A, int size);

  //valor medio de un array de doubles
  double mean(double *A, int size);

  //valor medio de un array de floats, atendiendo a su probabilidad
  float mean(float *prob, float *data, int size);

  //valor medio de un array de doubles, atendiendo a su probabilidad
  double mean(double *prob, double *data, int size);

  //varianza de un array de floats. Se usa el valor medio si se da (mean_p !=0)
  float var(float *A, int size, float *mean=NULL);

  //varianza de un array de doubles. Se usa el valor medio si se da (mean_p !=0)
  double var(double *A, int size, double *mean=NULL);

  //varianza de un array de doubles, atendiendo a su probabilidad. Se usa el valor medio si se da (mean_p !=0)
  double var(double *prob, double *data, int size, double *mean_p=NULL);

  //varianza de un array de floats, atendiendo a su probabilidad. Se usa el valor medio si se da (mean_p !=0)
  float var(float *prob, float *data, int size, float *mean_p=NULL);

  //retorna verdadero si el fichero existe. Requiere path completo
  bool existFile(char *fileName);

  //ventana para un mensaje de aviso y espera de aceptacion, o no. Requiere de Qt
  bool question(const char *q);

  //Retorna el valor maximo de un array de enteros sin signo
  unsigned int max(unsigned int *bufferIn, int size);

  //Retorna el valor maximo de un array de enteros
  int max(int *bufferIn, int size);

  //Retorna el valor maximo de un array de floats
  float max(float *bufferIn, int size);

  //Retorna el valor maximo de un array de doubles
  double max(double *bufferIn, int size);

  //Retorna el valor minimo de un array de enteros sin signo
  unsigned int min(unsigned int *bufferIn, int size);

  //Retorna el valor minimo de un array de enteros
  int min(int *bufferIn, int size);

  //Retorna el valor minimo de un array de floats
  float min(float *bufferIn, int size);

  //Retorna el valor minimo de un array de doubles
  double min(double *bufferIn, int size);

  //Retorna el indice al valor maximo de un array de doubles
  int maxIndex(double* buffer, int size);

  //Retorna el indice al valor maximo de un array de floats
  int maxIndex(float* buffer, int size);

  //Retorna el indice al valor maximo de un array de enteros
  int32_t maxIndex(int32_t* buffer, int size);

  //a partir de una matrix bidimensional, retorna un array con los valores promediados de los elementos en filas
  //average debe ser de tamano nRows
  void averageCol(float **matrix, int nRows, int nCols, float *average);

  //a partir de una matrix bidimensiona, retorna un array con los valores promediados de los elementos en columnas
  //average debe ser de tamano nRows
  void averageRow(float** matrix, int nRows, int nCols, float* average);

  //Aplica una conversion lineal entre rangos
  //dado el valor 'value' situado dentro del rango [iniLow-iniHigh] retorna su equivalente dentro
  //del rango [endLow-endHigh]
  //Satura si 'value' esta fuera de rango o si el rango destino es nulo
  double rangeConversion(double value, double iniLow, double iniHigh, double endLow, double endHigh);

  //Aplica una conversion lineal entre rangos
  //dado el valor 'value' situado dentro del rango [x1, x2] retorna su equivalente dentro
  //del rango [y1, y2]. Los puntos (x1, y1), (x2, y2) son conocidos
  //Satura si 'value' esta fuera de rango o si el rango destino es nulo
  double rangeConversion2(double value, double x1, double y1, double x2, double y2);

  //retorna el valor de data que hace que el total de valores inferiores coincida con 'percentil', en porcentaje
  //el valor retornado esta interpolado entre las dos magnitudes vecinas
  //Si zeros=true, se consideran los valores nulos igual al resto
  //Si zeros=false, los valores nulos quedan excluidos del conjunto
  float getValueFromPercentil(float   *data, int size, float  percentil, float minValue=1e-6);

  //retorna el valor de data que hace que el total de valores inferiores coincida con 'percentil', en porcentaje
  //el valor retornado esta interpolado entre las dos magnitudes vecinas
  //Si zeros=true, se consideran los valores nulos igual al resto
  //Si zeros=false, los valores nulos quedan excluidos del conjunto
  double getValueFromPercentil(double *data, int size, double percentil, double minValue=1e-6);

  //convierte la iformacion asociada a un puntero del tipo QLineEdit a double
  //Si la linea esta vacia retorna cero
  double QstringToDouble(QLineEdit* lineStr);

  //convierte la iformacion asociada a un puntero del tipo QString a char*
  void QStringToChar(QString* Qstr, char* str);

  //convierte la iformacion asociada a un puntero del tipo QLineEdit a char*
  void QlineEditToChar(QLineEdit* Qline, char* str);

  //Recibe un array en formato "0-5; 8,10; 15-17; 19"
  //Retorna el 'array' [0, 1, 2, 3, 4, 5, 8, 10, 15, 16, 17, 19]
  //El ';' es un separador de campos
  //La ',' es un separador de elementos dentro del campo
  //El '-' indica continuidad entre extremos dados e incluidos
  //Se toleran espacios en blanco
  //arraySize es la longitud maxima de array
  //Retorna la cantidad de elementos en 'array' o -1 si error
  //La longitud de str debe ser menor de 'maxSize' caracteres (fijado aqui a 100)
  int strToArray(char *str, int *array, int arraySize);

  //Establece en 'number' la secuencia de numeros en ascii que sigue a 'str' eliminando posibles espacios blancos
  //se consideran valores negativos
  //Retorna el tamano en caracteres del 'number'
  //'size' es el tamano maximo de number
  int getNumberFromStr(char *str, char* number, int size);

  //Establece en fileName el nombre del fichero extraido de un path
  //busca desde el final del string hasta encontrar en separador '\' o '/' de campo
  //si no lo encuentra se retorna el propio string de entrada
  //retorna false si path es nulo
  bool getFileNameFromPath(char* path, char* fileName);

  //Establece en directory el directorio asociado al fichero incluido en path
  //busca desde el final del string hasta encontrar en separador '\' o '/' de campo
  //si no lo encuentra se retorna el propio string de entrada
  //retorna false si path es nulo
  bool getDirFromPath(char* path, char* directory);

 //Establece en fileNameOut el nombre del fichero, con su posible path, sin la extension.
 //Busca desde el final, el '.' si encuentra antes el marcador de directorio o no encuentra el '.', 
 //establece una copia de fileNameIn.
 //Retorna false si fileNameIn esta vacio y hace que fileNameOut tambien sea nulo.
  bool getFileNameWithoutExtension(char* fileNameIn, char* fileNameOut);
 
  //Establece en extension la extension de fileNameIn.
  //Busca desde el final, el '.' si encuentra antes el marcador de directorio o no encuentra el '.', 
  //establece extension como nulo.
  //Retorna false si fileNameIn esta vacio y hace que extension tambien sea nulo.
  bool getFileNameExtension(char* fileNameIn, char* extension);
};

#endif

