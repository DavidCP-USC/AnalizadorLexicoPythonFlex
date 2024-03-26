#ifndef ANALIZADORLEXICO_H
#define ANALIZADORLEXICO_H
#include "TS.h"
#include "Errores.h"

/*
    * Funcion que inicializa el analizador lexico
    * @return Siguiente componente lexico
*/
tipoelem siguienteComponenteLexico();


/*
    * Función que libera la memoria utilizada por el analizador lexico
*/
void liberarMemoria();

void leerFichero (char* nombre);

void cerrarFichero();

#endif