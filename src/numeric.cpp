/***********************************************
 *   Proyecto "digital circuits simulator (dCir)"
 *   Copyright (C) 2024 by Esteban del Castillo
 *   esteban.delcastillo@urv.cat  julio-diciembre 2024
 *
 *    Manejo de información numérica
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

#include "numeric.h"

//Constructor
Numeric::Numeric()
{
}

//retorna true si el texto contiene la secuencia 0b o 0B y valores '0', '1', 'x' o 'X'
//puede estar precedido por espacios o tabuladores
bool Numeric::isBinary(char *txt)
{
    int i, j;
    bool hit=false;
    for(i=0; i<strlen(txt)-1; i++)  //sitúa el índice al primer elemento válido y busca la firma '0b' o '0B'
        {
        if(txt[i]==' ' || txt[i]=='\t') continue;
        if(txt[i]=='0' && (txt[i+1]=='b' || txt[i+1]=='B')) {i+=2; hit=true; break;};
        break;
        }
    if(!hit) return false;
    hit=true;
    j=i;
    for(; i<strlen(txt)-1; i++) //control de elementos válidos '0', '1', 'x', 'X'
        { 
        if((txt[i]==' '|| txt[i]=='\t') && i>j) break; //excluye espacios en blanco y asegura al menos un valor numérico
        if((txt[i]=='0' || txt[i]=='1' || txt[i]=='x' || txt[i]=='X')) continue; //valores posibles
        hit=false;
        }
    if(!hit) return false;
    return true;
}

//Retorna el valor decimal correspondiente a la cadena de texto en binario
//si no contiene ningún caracter válido, retorna cero
unsigned int Numeric::binaryToDecimal(char *txt)
{
    int i, j, k=0, value[8], one=1;
    unsigned int result=0;
    bool hit=false;
    
    if(!isBinary(txt)) return 0;  //se trata de un valor binario??
    
    for(i=0; i<strlen(txt)-1; i++) //sitúa el índice al primer elemento válido y busca la firma '0b' o '0B'
        {
        if(txt[i]==' ' || txt[i]=='\t') continue;
        if(txt[i]=='0' && (txt[i+1]=='b' || txt[i+1]=='B')) {i+=2; hit=true; break;}
        }
    if(hit)
        {
        for(j=i; j<strlen(txt); j++)//se convierte caracter a caracter a decimal
            {
            if(txt[j]=='0') value[k++]=0;
            else if(txt[j]=='1') value[k++]=1;
            else break;
            }
        int nDigits=j-i;
        for(int i2=0; i2<nDigits; i2++)
            result+=value[i2]*one<<(nDigits-i2-1);  //hexa to decimal por sumas de potencias
        }
    return result;
}

//retorna true si el texto contiene la secuencia 0x o 0X y valores '0'..'9', 'a'..'f', 'A'..'F', 'x', 'X'
//puede estar precedido por espacios o tabuladores
bool Numeric::isHexa(char *txt)
{
    int i, j;
    bool hit=false;
    for(i=0; i<strlen(txt)-1; i++)
        {
        if(txt[i]==' ' || txt[i]=='\t') continue;
        if(txt[i]=='0' && (txt[i+1]=='x' || txt[i+1]=='X')) {i+=2; hit=true; break;};
        break;
        }
    if(!hit) return false;
    hit=true;
    j=i;
    for(; i<strlen(txt)-1; i++) //control de elementos válidos '0'..'9', 'a'..'f', 'A'..'F', 'x', 'X'
        { 
        if((txt[i]==' '|| txt[i]=='\t') && i>j) break; //excluye espacios en blanco y asegura al menos un valor numérico
        else if(txt[j]>='0' && txt[j]<='9') continue;
        else if(txt[j]>='a' && txt[j]<='f') continue;
        else if(txt[j]>='A' && txt[j]<='F') continue;
        else if(txt[j]=='x' || txt[j]=='X') continue;
        else
            hit=false;
        }
    if(!hit) return false;
    return true;
}

//Retorna el valor decimal correspondiente a la cadena de texto en hexadecimal
//si no contiene ningún caracter válido, retorna 0
unsigned int Numeric::hexaToDecimal(char *txt)
{
    int i, j, k=0, value[8], one=1;
    unsigned int result=0;
    bool hit=false;
    
    if(!isHexa(txt)) return 0;  //es hexadecimal??
    
    for(i=0; i<strlen(txt)-1; i++) //sitúa el índice al primer elemento válido y busca la firma '0x' o '0X'
        {
        if(txt[i]==' ' || txt[i]=='\t') continue;
        if(txt[i]=='0' && (txt[i+1]=='x' || txt[i+1]=='X')) {i+=2; hit=true; break;}
        }
    if(hit)
        {
        for(j=i; j<strlen(txt); j++)  //se convierte caracter a caracter a decimal
            {
            if(txt[j]>='0' && txt[j]<='9') value[k++]=txt[j]-'0';
            else if(txt[j]>='a' && txt[j]<='f') value[k++]=txt[j]-'a'+10;
            else if(txt[j]>='A' && txt[j]<='F') value[k++]=txt[j]-'A'+10;
            else break;
            }
        int nDigits=j-i;
        for(int i2=0; i2<nDigits; i2++)
            result+=value[i2]*one<<((nDigits-i2-1)*4);  //hexa to decimal por sumas de potencias          
        }
    return result;
}

//compara dos cadenas de texto en formato binario
//retorna true si son idénticas o difieren en algún caracter pero una o ambas cadenas contiene 'x' 0 'X' en esa posición
bool Numeric::isEqual_binary(char *txt1, char *txt2)
{
    int i, j, k=0, value[8], one=1;
    unsigned int result=0;
    bool hit1=false, hit2=false;
//    if(!isBinary(txt1) || !isBinary(txt2)) return false;
    for(i=0; i<strlen(txt1)-1; i++) //i=índice al primer elemento numérico para text1
        {
        if(txt1[i]==' ' || txt1[i]=='\t') continue;
        if(txt1[i]=='0' && (txt1[i+1]=='b' || txt1[i+1]=='B')) {i+=2; hit1=true; break;}
        }
    for(j=0; j<strlen(txt2)-1; j++) //j=índice al primer elemento numérico para text2
        {
        if(txt2[i]==' ' || txt2[i]=='\t') continue;
        if(txt2[i]=='0' && (txt2[i+1]=='b' || txt2[i+1]=='B')) {i+=2; hit2=true; break;}
        }
    if(!hit1 || !hit2 || strlen(txt1+i) != strlen(txt2+j)) return false;  //difieren en longitud
    hit1=true;
    for(int k=i; k<strlen(txt1); k++)  //coinciden o difieren pero contiene 'x' o 'X'
        {
        if(txt1[i+k]==txt2[j+k]) //si coinciden
            {
            if((txt1[i+k]=='0' || txt1[i+k]=='1' || txt1[i+k]=='x' || txt1[i+k]=='X')) continue; //'0', '1', 'x', 'X' válidos
            else {hit1=false; break;} //elementos extraños en cadena binaria
            }
        else if(txt1[i+k]!='x' || txt1[i+k]!='X' || txt2[j+k]!='x' || txt2[j+k]!='X') continue;
        else {hit1=false; break;}    
        }
    if(!hit1) return false;
    return true;
}

//compara dos cadenas de texto en formato hexadecimal
//retorna true si son idénticas o difieren en algún caracter pero una o ambas cadenas contiene 'x' 0 'X' en esa posición
bool Numeric::isEqual_hexa(char *txt1, char *txt2)
{
    int i, j, k=0, value[8], one=1;
    unsigned int result=0;
    bool hit1=false, hit2=false;
//    if(!isBinary(txt1) || !isBinary(txt2)) return false;
    for(i=0; i<strlen(txt1)-1; i++) //i=índice al primer elemento numérico para text1
        {
        if(txt1[i]==' ' || txt1[i]=='\t') continue;
        if(txt1[i]=='0' && (txt1[i+1]=='x' || txt1[i+1]=='X')) {i+=2; hit1=true; break;}
        }
    for(j=0; j<strlen(txt2)-1; j++) //j=índice al primer elemento numérico para text2
        {
        if(txt2[i]==' ' || txt2[i]=='\t') continue;
        if(txt2[i]=='0' && (txt2[i+1]=='x' || txt2[i+1]=='X')) {i+=2; hit2=true; break;}
        }
    if(!hit1 || !hit2 || strlen(txt1+i) != strlen(txt2+j)) return false;  //difieren en longitud
    hit1=true;
    for(int k=i; k<strlen(txt1); k++)  //coinciden o difieren pero contiene 'x' o 'X'
        {
        if(txt1[i+k]==txt2[j+k]) //si coinciden
            {
            if((txt1[i+k]=='0' || txt1[i+k]=='1' || txt1[i+k]=='x' || txt1[i+k]=='X')) continue; //'0', '1', 'x', 'X' válidos
            else {hit1=false; break;} //elementos extraños en cadena binaria
            }
        else if(txt1[i+k]!='x' || txt1[i+k]!='X' || txt2[j+k]!='x' || txt2[j+k]!='X') continue;
        else {hit1=false; break;}    
        }
    if(!hit1) return false;
    return true;
}

//recibe un texto en formato binario (ej."0b10X1") o hexadecimal (ej."0x1AX0") y retorna un valor 
//en el que los bits con 'x' se hacen cero, junto el número de bits
//aplicando esa máscara sobre cada nuevo valor dado se identifican los valores válidos
int Numeric::valueToMask(char* txt, unsigned int *value, int *nBits, unsigned int *mask)
{
    int i, j, k, n, mask1=1, ret=0;
    unsigned int result=0;
    char* txt2 = 0;
    if ((txt2 = new char[strlen(txt)+5]) == 0) return -1;
    //char txt2[strlen(txt)];
    char val1, val2;
    bool bin=false, hexa=false;
    
    if(isBinary(txt)) //
        {val1='b'; val2='B'; bin=true;}
    else if(isHexa(txt)) //
        {val1='x'; val2='X'; hexa=true;}
    
    for(i=0; i<strlen(txt)-1; i++) //i=índice al primer elemento numérico para text
        {
        if(txt[i]==' ' || txt[i]=='\t') continue;
        if(txt[i]=='0' && (txt[i+1]==val1 || txt[i+1]==val2)) {i+=2; break;}
        }

    for(j=i; j<strlen(txt); j++) //i=índice al último elemento numérico para text
        if(txt[j]==' ' || txt[j]=='\t') break; 
    
    *nBits=j-i; //bits
    txt2[0]='0'; txt2[1]='b'; 
    mask1<<=*nBits-1;
    char txt3[10];
    txt3[0]='0', txt3[1]='x';
    for(k=i, n=2; k<j; k++, n++)
        {
        if(txt[k]=='x' || txt[k]=='X') 
            {result&=~mask1; txt2[n]='0'; txt3[n]='0';}
        else {result|=mask1; txt2[n]=txt[k];txt3[n]='F';}
        mask1>>=1;
        }        
     txt2[n]=0;  //fin de string
     txt3[n]=0;  //fin de string
     
     if(bin==true)
        {
        *value=binaryToDecimal(txt2);
        *mask=result;
        }
    else if(hexa==true)
        {
        txt2[1]='x';
        *value=hexaToDecimal(txt2);
        *mask=hexaToDecimal(txt3);
        (*nBits)*=4;
        }
     else ret=-1;
     if (txt2) delete []txt2;
     return ret;
}


//retorna true si valor existe en la lista buffer
//si index_p!=0 establece la posición donde se encuentra
bool Numeric::exist(int valor, int *buffer, int bufferSize, int *index_p)
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

//acepta texto binario (0b/0B), hexadecimal (0x/0X) y decimal (si no incluye prefijo)
//Retorna su valor o cero si no existe ningún valor
unsigned int Numeric::getValueFromText(char *text)
{
    unsigned int value;
    if(isBinary(text))
        value=binaryToDecimal(text); 
    else if(isHexa(text))
        value=hexaToDecimal(text);  //no tolera 'x'                
    else 
        value=atoi(text);  //decimal               
   return value; 
}

//convierte un valor numérico a hexadecimal en formato de texto
void Numeric::valueToHexa(unsigned int value, char *text, int nBits)
{
    int txt2[20];
    text[0]='0'; text[1]='x';
    unsigned int dividendo=value;
    unsigned int cociente;
    int c=ceil(nBits/4.0);
    for(int i=0; i<c; i++) txt2[i]='0';
    int i=0, k=2;
    while(true)  //conversión a hexa
        {
        cociente=dividendo/16;
        txt2[i++]=dividendo%16;
        dividendo=cociente;
        if(cociente<16) break;
        }
    txt2[i]=cociente;
    for(int j=c-1; j>=0; j--) //los valores quedan invertidos
        {
        if(txt2[j]>=0 && txt2[j]<10) txt2[j]+='0';
        else if(txt2[j]>=10 && txt2[j]<16) txt2[j]+='A'-10;
        text[k++]=txt2[j];
        }
    text[k]=0; //fin de string
}







