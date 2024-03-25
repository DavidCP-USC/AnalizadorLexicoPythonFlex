#ifndef TS_H
#define TS_H
#include "abin.h"

/*
    * Inicializa la tabla de símbolos con las palabras reservadas del lenguaje
*/
void initTS();

/*
    * Imprime la tabla de símbolos en formato texto
*/
void imprimirTS ();

/*
    * Destruye la tabla de símbolos y libera la memoria
*/
void destruirTS();


/*
    * Busca el valor de un lexema en la tabla de símbolos
    * @param lexema: lexema a buscar
    * @return valor del lexema en la tabla de símbolos
*/
int obtenerValorTS(char* lexema);

#endif