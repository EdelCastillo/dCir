/***********************************************
 *   Copyright (C) 2014 by Esteban del Castillo   
 *   esteban.delcastillo@urv.cat   
 *
 *   Proyecto 
 *  	septiembre 2017
************************************************/

#include "common.h"

Common::Common()
  {
  }

Common::~Common()
  {
  }

//Retorna el índice en el buffer ocupado por 'data' o -1 si no existe
//búsqueda por aproximaciones sucesivas
//Requiere que los elementos en 'buffer' estén ordenados de menor a mayor
int Common::getIndexInSortedList(int data, int *buffer, int bufferSize)
{
  int index, delta;
  int h=bufferSize-1; 	//índice alto dentro del rango de búsqueda
  int l=0;		//índice bajo dentro del rango de búsqueda
       if(data==buffer[l]) return l;		//los casos extremos requieren especial atención
  else if(data==buffer[h]) return h;
 index=bufferSize/2;
  if(!index) return -1; //sólo un elemento y no coinciden
//for(int i=0; i<bufferSize-1; i++) 
//  if(buffer[i+1]<buffer[i]) printf("ERROR in getIndexInSortedList()\n");
  while(1)
  {
    if(data==buffer[index])  return index;
    else if(data>buffer[index]) 
    {
      l=index;
      if((delta=(h-l)/2)==0) break; //no existe
      index=delta+l;
    }
    else 
    {
      h=index;
      if((delta=(h-l)/2)==0) break; //no existe
      index=delta+l;
    }
  }
  return -1;
}

//ordena de mayor a menor la lista 'bufferIn' de numeros enteros>=0
//y la retorna en 'sort'
//retorna en sort una cadena de índices a bufferIn tal que la secuencia resulta ordenada de mayor a menor
int Common::sortDown(int *bufferIn, int *sort, int size)
{
int k=0, j;// end=m_m_distributionSize;
int mayor;
int  *list=0;

if(size==0) return 0; 
if((list=new int[size])==0)
    return -1;
for(j=0; j<size; j++)
      {
      if(bufferIn[j]<0)
	list[j]=0;
      else
	list[j]=bufferIn[j]; //copia ya que se producen cambios en el contenido
      }

if(size==1) sort[0]=0;//Si sólo hay uno ya está ordenado. Si no hay ninguno -> no hacer nada
else
   do
	{
	mayor=-1; //minor<=minValue.
	for(j=0; j<size; j++)
		{
		if((list[j])>mayor) 
			{
			mayor=list[j];
			sort[k]=j;
			}
		}
	list[sort[k]]=-1;
//	if(k==72)
//	  printf(".");
	k++;
	}
while(k<size);
if(list) delete [] list;
return 0;
}

int Common::sortDown(float* bufferIn, int *sort, int size, bool absolutas)
{
int k=0, j;// end=m_m_distributionSize;
float  mayor;
float  *list=0;

if(size==0) return 0; 
if((list=new float[size])==0)
    return -1;
for(j=0; j<size; j++)
{
  if(absolutas)
    list[j]=fabs(bufferIn[j]); //copia ya que se producen cambios en el contenido
  else
    list[j]=bufferIn[j]; //copia ya que se producen cambios en el contenido
}

if(size==1) sort[0]=0;//Si sólo hay uno ya está ordenado. Si no hay ninguno -> no hacer nada
else
   do
	{
	mayor=-FLT_MAX; //minor<=minValue.
	for(j=0; j<size; j++)
		{
		if((list[j])>mayor) 
			{
			mayor=list[j];
			sort[k]=j;
			}
		}
	list[sort[k]]=-FLT_MAX;
	k++;
	}
while(k<size);
if(list) delete [] list;
return 0;
}

int Common::sortDown(double* bufferIn, int *sort, int size, bool absolutas)
{
int k=0, j;// end=m_m_distributionSize;
double  mayor;
double  *list=0;

if(size==0) return 0; 
if((list=new double[size])==0)
    return -1;

for(j=0; j<size; j++)
{
  if(absolutas)
    list[j]=fabs(bufferIn[j]); //copia ya que se producen cambios en el contenido
  else
    list[j]=bufferIn[j]; //copia ya que se producen cambios en el contenido
}

if(size==1) sort[0]=0;//Si sólo hay uno ya está ordenado. Si no hay ninguno -> no hacer nada
else
   do
	{
	mayor=-DBL_MAX; //minor<=minValue.
	for(j=0; j<size; j++)
		{
		if((list[j])>mayor) 
			{
			mayor=list[j];
			sort[k]=j;
			}
		}
  
	list[sort[k]]=-DBL_MAX;
	k++;
	}
while(k<size);
if(list) delete [] list;
return 0;
}


//retorna en sort una cadena de índices a bufferIn tal que la secuencia resulta ordenada de menor a mayor
int Common::sortUp(double *bufferIn, int *sort, int size)
{
int k=0, j;// end=m_m_distributionSize;
double  minor;
//double  list[size];
double  *list=0;
if(size<=0) return 0; 
if ((list = new double[size]) == 0) return 0;
for(j=0; j<size; j++)
  {
   list[j]=bufferIn[j]; //copia ya que se producen cambios en el contenido
  }

if(size==1) //Si sólo hay uno ya está ordenado. Si no hay ninguno -> no hacer nada
    *sort=0;
else
   do
	{
//	minor=2; //minor>maxValue. El valor máximo se sabe que será 1.0
	minor=DBL_MAX; //minor>maxValue. El valor superior al máximo 
	for(j=0; j<size; j++)
		{
		if((list[j])<minor) 
			{
			minor=list[j];
			sort[k]=j;
			}
		}
	list[sort[k]]=DBL_MAX;
	k++;
	}
while(k<size);
if (list) delete[]list;
return 0;
}

//retorna en sort una cadena de índices a bufferIn tal que la secuencia resulta ordenada de menor a mayor
int Common::sortUp(float *bufferIn, int *sort, int size)
{
int k=0, j;// end=m_m_distributionSize;
float  minor;
//float  list[size];
float* list = 0;
if (size <= 0) return 0;
if ((list = new float[size]) == 0) return 0;
for(j=0; j<size; j++)
      list[j]=bufferIn[j]; //copia ya que se producen cambios en el contenido

if(size==1) //Si sólo hay uno ya está ordenado. Si no hay ninguno -> no hacer nada
    *sort=0;
else
   do
	{
//	minor=2; //minor>maxValue. El valor máximo se sabe que será 1.0
	minor=0x7FFFFFFF; //minor>maxValue. El valor superior al máximo 
	for(j=0; j<size; j++)
		{
		if((list[j])<minor) 
			{
			minor=list[j];
			sort[k]=j;
			}
		}
	list[sort[k]]=0x7FFFFFFF;
	k++;
	}
while(k<size);
if (list) delete[]list;
return 0;
}

//retorna en sort una cadena de índices a bufferIn tal que la secuencia resulta ordenada de menor a mayor
int Common::sortUp(int *bufferIn, int *sort, int size)
{
int k=0, j;// end=m_m_distributionSize;
int minor;
//int  list[size];
int* list = 0;
if (size <= 0) return 0;
if ((list = new int[size]) == 0) return 0;
for(j=0; j<size; j++)
      list[j]=bufferIn[j]; //copia ya que se producen cambios en el contenido

if(size==1) //Si sólo hay uno ya está ordenado. Si no hay ninguno -> no hacer nada
    *sort=0;
else
   do
	{
//	minor=2; //minor>maxValue. El valor máximo se sabe que será 1.0
	minor=0x7FFFFFFF; //minor>maxValue. El valor superior al máximo 
	for(j=0; j<size; j++)
		{
		if((list[j])<minor) 
			{
			minor=list[j];
			sort[k]=j;
			}
		}
	list[sort[k]]=0x7FFFFFFF;
	k++;
	}
while(k<size);
if (list) delete[]list;
return 0;
}

//retorna true si valor existe en la lista buffer
//si index_p!=0 establece la posición donde se encuentra
bool Common::exist(int valor, int *buffer, int bufferSize, int *index_p)
  {
    if(index_p)
        {
        for (int i=0; i<bufferSize; i++)
            if(valor==buffer[i]) 
                {
                *index_p=i; 
                return true;
                }
        }
    else
        {
        for (int i=0; i<bufferSize; i++)
            if(valor==buffer[i]) 
                return true;
        }
        
  return false;
  }

//valor medio de un array de floats
float Common::mean(float *A, int size)
{
  float S=0;
  if(size<=0) return -1;
  for(int i=0; i<size; i++)
    S+=A[i];
  return S/size;
}

//valor medio de un array de doubles
double Common::mean(double *A, int size)
{
  double S=0;
  if(size<=0) return -1;
  for(int i=0; i<size; i++)
    S+=A[i];
  return S/size;
}

//valor medio de un array de doubles, atendiendo a su probabilidad
double Common::mean(double *prob, double *data, int size)
{
 double S=0;   
  if(size<=0) return -1;
  for(int i=0; i<size; i++)
    S+=prob[i]*data[i];
  return S;
}

//valor medio de un array de floats, atendiendo a su probabilidad
float Common::mean(float *prob, float *data, int size)
{
 float S=0;   
  if(size<=0) return -1;
  for(int i=0; i<size; i++)
    S+=prob[i]*data[i];
  return S;
}

//varianza de un array de floats. Se usa el valor medio si se da (mean_p !=0)
float Common::var(float *A, int size, float *mean_p)
{
  float m;
  float S=0;
  if(size<=0) return -1;
  if(mean_p) m=*mean_p;
  else m=mean(A, size);
  
  for(int i=0; i<size; i++)
    S+=(A[i]-m)*(A[i]-m);
  return S/size;
}

//varianza de un array de double. Se usa el valor medio si se da (mean_p !=0)
double Common::var(double *A, int size, double *mean_p)
{
  double m;
  double S=0;
  if(size<=0) return -1;
  
  if(mean_p) m=*mean_p;
  else m=mean(A, size);
  
  for(int i=0; i<size; i++)
    S+=(A[i]-m)*(A[i]-m);
  return S/size;
}

//varianza de un array de doubl, atendiendo a su probabilidade. Se usa el valor medio si se da (mean_p !=0)
double Common::var(double *prob, double *data, int size, double *mean_p)
{
  double m;
  double S=0;
  if(size<=0) return -1;
  
  if(mean_p) m=*mean_p;
  else m=mean(prob, data, size);
  
  for(int i=0; i<size; i++)
    S+=(data[i]-m)*(data[i]-m)*prob[i];
  return S;
}

//varianza de un array de floats, atendiendo a su probabilidad. Se usa el valor medio si se da (mean_p !=0)
float Common::var(float *prob, float *data, int size, float *mean_p)
{
  float m;
  float S=0;
  if(size<=0) return -1;
  if(mean_p) m=*mean_p;
  else m=mean(prob, data, size);
  
  for(int i=0; i<size; i++)
    S+=(data[i]-m)*(data[i]-m)*prob[i];
  return S;
}

//retorna verdadero si el fichero existe. Requiere path completo
bool Common::existFile(char *fileName)
    {
    bool exist=false;
    FILE *fp;
    if((fp=fopen(fileName, "r"))!=NULL)
        {
        exist=true;
        fclose(fp);
        }
    return exist;
    }

//ventana para un mensaje de aviso y espera de aceptación, o no
//requiere de Qt
bool Common::question(const char *q)
        {
        bool exit=false;
//        printf("Exist %s\n", fileName_2); fclose(fp);
        QMessageBox messageBox;
        messageBox.setWindowTitle(("MALDI"));
//        messageBox.setText(("This data exist in a cache file.\n     Do you want load it?"));
        messageBox.setText(q);
        messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        messageBox.setDefaultButton(QMessageBox::No);
        if (messageBox.exec() == QMessageBox::Yes)
            exit=true;
        return exit;
        }
        
//Retorna el valor máximo de un array de doubles
double Common::max(double* buffer, int size)
{
  double maxValue=0;
  for(int i=0; i<size; i++)
    if(buffer[i]>maxValue) maxValue=buffer[i];
  return maxValue;
}


//Retorna el valor máximo de un array de floats
float Common::max(float* buffer, int size)
{
  float maxValue=0.0;
  for(int i=0; i<size; i++)
    if(buffer[i]>maxValue) maxValue=buffer[i];
  return maxValue;
}

//Retorna el valor máximo de un array de enteros
unsigned int Common::max(unsigned int* buffer, int size)
{
  unsigned int maxValue=-0xFFFFFFFF;
  for(int i=0; i<size; i++)
    if(buffer[i]>maxValue) maxValue=buffer[i];
  return maxValue;
}

int Common::max(int* buffer, int size)
{
  int maxValue=-0x7FFFFFFF;
  for(int i=0; i<size; i++)
    if(buffer[i]>maxValue) maxValue=buffer[i];
  return maxValue;
}

//Retorna el valor mínimo de un array
double Common::min(double* buffer, int size)
{
  double minValue=1e308;
  for(int i=0; i<size; i++)
    if(buffer[i]<minValue) minValue=buffer[i];
  return minValue;
}

//Retorna el valor mínimo de un array
float Common::min(float* buffer, int size)
{
  float minValue=1e38;
  for(int i=0; i<size; i++)
    if(buffer[i]<minValue) minValue=buffer[i];
  return minValue;
}

//Retorna el valor mínimo de un array
unsigned int Common::min(unsigned int* buffer, int size)
{
  unsigned int minValue=0xFFFFFFFF;
  for(int i=0; i<size; i++)
    if(buffer[i]<minValue) minValue=buffer[i];
  return minValue;
}

//Retorna el valor mínimo de un array
int Common::min(int* buffer, int size)
{
  int minValue=0x7FFFFFFF;
  for(int i=0; i<size; i++)
    if(buffer[i]<minValue) minValue=buffer[i];
  return minValue;
}


//retorna el índice correspondiente al máximo valor
int Common::maxIndex(double* buffer, int size)
{
  double maxValue=0;
  int index=0;
  for(int i=0; i<size; i++)
    if(buffer[i]>maxValue) {maxValue=buffer[i]; index=i;}
  return index;
}

//retorna el índice correspondiente al máximo valor
int Common::maxIndex(float* buffer, int size)
{
  float maxValue=0;
  int index=0;
  for(int i=0; i<size; i++)
    if(buffer[i]>maxValue) {maxValue=buffer[i]; index=i;}
  return index;
}

//retorna el índice correspondiente al máximo valor
int32_t Common::maxIndex(int32_t* buffer, int size)
{
  int32_t maxValue=-0x7FFFFFFF;
  int index=0;
  for(int i=0; i<size; i++)
    if(buffer[i]>maxValue) {maxValue=buffer[i]; index=i;}
  return index;
}

//a partir de una matrix 2D, retorna un array con los valores promediados de los elementos en filas
//average debe ser de tamaño nRows
void Common::averageCol(float **matrix, int nRows, int nCols, float *average)
{
    for(int row=0; row<nRows; row++)
        average[row]=0.0;
    
    for(int row=0; row<nRows; row++)
        for(int col=0; col<nCols; col++)
            average[row]+=matrix[row][col];
        
    for(int row=0; row<nRows; row++)
        average[row]/=nCols;     
}

//a partir de una matrix 2D, retorna un array con los valores promediados de los elementos en columnas
//average debe ser de tamaño nRows
void Common::averageRow(float **matrix, int nRows, int nCols, float *average)
{
    for(int col=0; col<nCols; col++)
        average[col]=0.0;
    
    for(int col=0; col<nCols; col++)
        for(int row=0; row<nRows; row++)
            average[col]+=matrix[row][col];
        
    for(int col=0; col<nCols; col++)
        average[col]/=nRows;     
}

//Aplica una conversión lineal entre rangos
//dado el valor 'value' situado dentro del rango [iniLow-iniHigh] retorna su equivalente dentro
//del rango [endLow-endHigh]
//Satura si 'value' está fuera de rango o si el rango destino es nulo
double Common::rangeConversion(double value, double iniLow, double iniHigh, double endLow, double endHigh)
{
  double x, y, w;
  if(value<iniLow) 
      return endLow;
  else if(value>iniHigh)
      return endHigh;
  else if(endHigh-endLow==0)
      return endLow;
  
  if(endLow==0)
    {
    x=(iniHigh*endLow-endHigh*iniLow)/(endLow-endHigh);
    y=endHigh/(iniHigh-x);
    }
  else
    {
    x=(endHigh*iniLow-iniHigh*endLow)/(endHigh-endLow);
    y=endLow/(iniLow-x);
    }
  w=(value-x)*y;
  return w;
}

//Aplica una conversión lineal entre rangos
//dado el valor 'value' situado dentro del rango [x1, x2] retorna su equivalente dentro
//del rango [y1, y2]. Los puntos (x1, y1), (x2, y2) son conocidos
//Satura si 'value' está fuera de rango o si el rango destino es nulo
double Common::rangeConversion2(double value, double x1, double x2, double y1, double y2)
{
  if(value<x1) 
      return y1;
  else if(value>x2)
      return y2;
  else if(y2-y1==0)
      return y1;

  return y1+value*(y2-y1)/(x2-x1);
}

//retorna el valor de data que hace que el total de valores inferiores coincida con 'percentil', en porcentaje
//el valor retornado está interpolado entre las dos magnitudes vecinas
//Si zeros=true, se consideran los valores nulos igual al resto
//Si zeros=false, los valores nulos quedan excluidos del conjunto
float Common::getValueFromPercentil(float *data, int size, float percentil, float minValue)
 {
     int *sortDataIndex_p=0;
     float a, b, x;
     int ia, ib, indexLow=0;
     
     if(size<1) return 0; //control de límites
     if((sortDataIndex_p=new int[size])==0)
        {printf("ERROR new() in Common class\n"); return -1;}
        
     sortUp(data, sortDataIndex_p, size);
//printf("... excludeZeros:%d", excludeZeros);
     if(minValue>0.0)
        {
        for(indexLow=0; indexLow<size; indexLow++)
            {   
            if(data[sortDataIndex_p[indexLow]]>minValue) break;
            }
        if(indexLow==size-1) return 0.0;
        else
            size-=indexLow;
        }
     float indexSort=size*percentil/100.0 + indexLow;
//printf("... indexLow:%d", indexLow);
     
     //interpolación y control de límites
     ia=trunc(indexSort);  //índice anterior
     ib=ceil(indexSort);   //índice posterior
     if(ib>=size)  //control de límites
        {ia-=1; ib-=1;}
     if(ia<0 || indexSort<0)
        return data[0];
     
     a=data[sortDataIndex_p[ia]];
     b=data[sortDataIndex_p[ib]];
     x=a+(b-a)*(indexSort-ia);
     
    if(sortDataIndex_p) delete []sortDataIndex_p;
    return x;
 }
 
//retorna el valor de data que hace que el total de valores inferiores coincida con 'percentil', en porcentaje
//el valor retornado está interpolado entre las dos magnitudes vecinas
//Si zeros=true, se consideran los valores nulos igual al resto
//Si zeros=false, los valores nulos quedan excluidos del conjunto
double Common::getValueFromPercentil(double *data, int size, double percentil, double minValue)
 {
     int *sortDataIndex_p=0;
     double a, b, x;
     int ia, ib, indexLow=0;
     
     if(size<1) return 0; //control de límites
     if((sortDataIndex_p=new int[size])==0)
        {printf("ERROR new() in Common class\n"); return -1;}
        
     sortUp(data, sortDataIndex_p, size);
     if(minValue>0.0)
        {
        for(indexLow=0; indexLow<size; indexLow++)
            if(sortDataIndex_p[indexLow]>minValue) break;
        if(indexLow==size-1) return 0.0;
        else
            size-=indexLow;
        }
     float indexSort=size*percentil/100.0 + indexLow;
//     float indexSort=size*percentil/100.0;
     
     //interpolación y control de límites
     ia=trunc(indexSort);  //índice anterior
     ib=ceil(indexSort);   //índice posterior
     if(ib>=size)  //control de límites
        {ia-=1; ib-=1;}
     if(ia<0 || indexSort<0)
        return data[0];
     
     a=data[sortDataIndex_p[ia]];
     b=data[sortDataIndex_p[ib]];
     x=a+(b-a)*(indexSort-ia);
     
    if(sortDataIndex_p) delete []sortDataIndex_p;
    return x;
 }

//convierte la iformación asociada a un puntero del tipo QLineEdit a double
//Si la linea está vacía retorna cero
 double Common::QstringToDouble(QLineEdit *lineStr)
 {
    double val; 
    char str[100];  
    int i;
//  QString XMAX=str->text();
//   QString str=lineStr->displayText();
    const char* iniStr_p=(const char*)lineStr->displayText().toUtf8();
/*
    for(i=0; iniStr_p[i]; i++)
        {
        str[i]=iniStr_p[i];
        if(str[i]==',') str[i]='.';
        }
    str[i]=0;
    if (i==0)
        val = 0;
    else
        val = atof(str);
    
   return val;
   */
    return atof(iniStr_p);
  }
  
 //convierte la iformación asociada a un puntero del tipo QString a char*
 void Common::QStringToChar(QString* Qstr, char *str)
  {
  QByteArray ba;
  ba.resize(200);
  ba =Qstr->toLatin1();
  strcpy(str, ba.data());
  }
  
 //convierte la iformación asociada a un puntero del tipo QLineEdit a char*
 void Common::QlineEditToChar(QLineEdit* Qline, char *str)
  {
  QString Qstr;
  Qstr.resize(200);
  QByteArray ba;
  ba.resize(200);
  Qstr=Qline->text();
  ba = Qstr.toLatin1();
  strcpy(str, ba.data()); 
  }
  
//Recibe un array en formato "0-5; 8,10; 15-17; 19"
//Retorna el 'array' [0, 1, 2, 3, 4, 5, 8, 10, 15, 16, 17, 19]
//El ';' es un separador de campos
//La ',' es un separador de elementos dentro del campo
//El '-' indica continuidad entre extremos dados e incluidos
//Se toleran espacios en blanco
//arraySize es la longitud máxima de array
//Retorna la cantidad de elementos en 'array' o -1 si error
//La longitud de str debe ser menor de 'maxSize' caracteres (fijado aquí a 100)
int Common::strToArray(char *str, int *array, int arraySize)
{
  const char *warning= "Warning: no existe valor numérico en el string del campo %d: (%s)\n";
  const char *warning2="Warning: el campo %d no es correcto: (%s)\n";
  const int maxSize=100;
  int strSize=strlen(str);
  if(strSize>maxSize) return -1;
  int countPC=0, i, j, index=0, valueIni, valueEnd;
  
  typedef struct
  {
    int ini, 
	end;
  }PC;
  PC pc[maxSize];
  char strTmp[maxSize];
  
  pc[0].ini=0;
  for(i=0; i<strSize; i++) //se determina el numero de bloques (separados por ';')
    {
    if(countPC) pc[countPC].ini=pc[countPC-1].end+1;
    if(str[i]==';') pc[countPC++].end=i;
    }
    if(i==strSize) pc[countPC++].end=i-1;
    
  for(i=0; i<countPC; i++) //para cada bloque
    {
//      printf("[%d]%d %d\n",i, pc[i].ini, pc[i].end);
      j=pc[i].ini;  //para cada elemento carácter del bloque
      j+=getNumberFromStr(str+j, strTmp, maxSize); //consigue el primer número
      if(strTmp[0]==0) //error: no existe valor numérico en el estring
            break;//{printf(warning, i, str); return -1;}
      valueIni=atoi(strTmp);
      
      for( ; str[j]==' ' && j<maxSize; j++); //se eliminan pobibles espacios en blanco
      switch(str[j]) //se evalua la acción a tomar
        {
        case '-': //se trata de un rango de valores entre extremos dados 
            getNumberFromStr(str+j+1, strTmp, maxSize); //segundo número
            
            if(strTmp[0]==0) //error: no existe valor numérico en el estring
                break;//{printf(warning, i, str); return -1;}
                
            valueEnd=atoi(strTmp);
            
            if(valueEnd<valueIni) //error: el segundo valor es menor que el primero
                {printf(warning2, i, str); return -1;}
                
            for(int k=valueIni; k<=valueEnd; k++)
                {
                array[index++]=k;
                if(index>arraySize) 
                    return -1;
                }
        
    //	  j=pc[i].end; //termina el grupo
            break;
        case ',': //se trata de elementos aislados separados por comas
            array[index++]=valueIni;
            for(; j<pc[i].end; j++) //para cada elemento carácter del bloque
                {
                j+=getNumberFromStr(str+j+1, strTmp, maxSize);//siguiente número
                
                if(strTmp[0]==0) //error: no existe valor numérico en el string
                    break;//{printf(warning, i, str); return -1;}
                
                array[index++]=atoi(strTmp);
                if(index>arraySize) 
                    return -1;
                }
            break;
        default: //atiende al último valor al que puede no seguir '-' ó ',' ó ';'
            array[index++]=valueIni;
            if(index>arraySize) 
                return -1;
            break;
        }
    }
//printf("...\n");
 return index;
}

/*
//Establece en 'number' la secuencia de números en ascii que sigue a 'str' eliminando posibles espacios blancos
//Retorna el tamaño en caracteres del 'number'
//'size' es el tamaño máximo de number
int Common::getNumberFromStr(char *str, char* number, int size)
{
  int i, j=0;
  int k=strlen(str);
  for(i=0; str[i]==' ' && i<k; i++); //se eliminan pobibles espacios en blanco
  for(; isdigit(str[i]) && i<size; i++, j++)
    number[j]=str[i];
  number[j]='\0';
  return i;
}
*/

//Establece en 'number' la secuencia de números en ascii que sigue a 'str' eliminando posibles espacios blancos
//se consideran valores negativos
//Retorna el tamaño en caracteres del 'number'
//'size' es el tamaño máximo de number
int Common::getNumberFromStr(char *str, char* number, int size)
{
  int i, j=0;
  int k=strlen(str);
  for(i=0; str[i]==' ' && i<k; i++); //se eliminan pobibles espacios en blanco
  for(; i<size; i++)
    {
    if(j==0 && (str[i]=='-' || str[i]=='+')) { number[j++] = str[i]; }  //posible signo
    else if(str[i]==' ' || str[i]=='\t') continue; //espacios en blanco
    else if(isdigit(str[i])) number[j++]=str[i];
    else break;
    }
  number[j]='\0';
  return i;
}

//Establece en fileName el nombre del fichero extraido de un path
//busca desde el final del string hasta encontrar en separador '\' o '/' de campo
//si no lo encuentra se retorna el propio string de entrada
//retorna false si path es nulo
bool Common::getFileNameFromPath(char* path, char* fileName)
{
    bool hit = false;
    int size, i;
    if ((size = strlen(path)) == 0){
        fileName[0] = 0; return false;
        }
    for (i = strlen(path); i; i--)
        if ((path[i] == '\\') || (path[i] == '/')) { hit = true; break; }
    if (hit) { strcpy(fileName, path + i + 1); return true; } //si path incluye un directorio
    else { strcpy(fileName, path); return true; }
    return  false;
}

//Establece en directory el directorio asociado al fichero incluido en path
//busca desde el final del string hasta encontrar en separador '\' o '/' de campo
//si no lo encuentra se retorna el propio string de entrada
//retorna false si path es nulo
bool Common::getDirFromPath(char* path, char* directory)
{
    int i, size, size2;
    if ((size = strlen(path)) == 0) {
        directory[0] = 0;  return false;
        }
    if (getFileNameFromPath(path, directory) == false)
        strcpy(directory, path); //no existe nombre de fichero en el path
    else
    {
        size2 = strlen(directory)+1; //+1 para incluir el separador de campo
        for (i = 0; i < size - size2; i++)
            directory[i] = path[i];
        directory[i] = 0;
    }
    return true;
}

//Establece en fileNameOut el nombre del fichero, con su posible path, sin la extensión.
//Busca desde el final, el '.' si encuentra antes el marcador de directorio o no encuentra el '.', 
//establece una copia de fileNameIn.
//Retorna false si fileNameIn está vacío y hace que fileNameOut también sea nulo.
bool Common::getFileNameWithoutExtension(char* fileNameIn, char* fileNameOut)
{
    int i, size;
    bool hit=false;
    if ((size = strlen(fileNameIn)) == 0) {
        fileNameOut[0] = 0;  return false;
    }
    else
    {
        for (i = strlen(fileNameIn); i; i--)
        {
            if ((fileNameIn[i] == '\\') || (fileNameIn[i] == '/')) { hit = false; break; }
            if (fileNameIn[i] == '.') {hit = true; break;}
        }
        if (hit)
        {
            for (int j = 0; j < i; j++) fileNameOut[j] = fileNameIn[j];
            fileNameOut[i] = 0;
        }
        else
            strcpy(fileNameOut, fileNameIn);  //nombre sin extensión
    }
    return true;
}

//Establece en extensión la extensión de fileNameIn.
//Busca desde el final, el '.' si encuentra antes el marcador de directorio o no encuentra el '.', 
//establece extension como nulo.
//Retorna false si fileNameIn está vacío y hace que extension también sea nulo.
bool Common::getFileNameExtension(char* fileNameIn, char* extension)
{
    int i, size;
    bool hit = false;
    if ((size = strlen(fileNameIn)) == 0) {
        extension[0] = 0;  return false;
    }
    else
    {
        for (i = strlen(fileNameIn); i; i--)
        {
            if ((fileNameIn[i] == '\\') || (fileNameIn[i] == '/')) { hit = false; break; }
            if (fileNameIn[i] == '.') { hit = true; break; }
        }
        if (hit)
        {
            for (; i < strlen(fileNameIn); i++) extension[i] = fileNameIn[i];
            extension[i] = 0;
        }
        else {
            extension[0] = 0; return false;
        }
    }
    return true;
}
