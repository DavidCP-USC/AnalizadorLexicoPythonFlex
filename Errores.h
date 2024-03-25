#ifndef ERRORES_H
#define ERRORES_H
#include <stdio.h>
#include "definiciones.h"
#include "SistemaEntrada.h"

/*
    * Función que imprime un mensaje de error en la salida 
        estandar junto con la linea en la que se produce
    * @param tipo: tipo de error
    * @param error: mensaje de error
*/
void printTipoError(int tipo, char *error);

#endif