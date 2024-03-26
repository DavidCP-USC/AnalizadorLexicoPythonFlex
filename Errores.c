#include "Errores.h"
#include <stdio.h>
#include <stdlib.h>
#include "definiciones.h"

void printTipoError(int tipo, char *error, int linea){
    switch(tipo){
        case ERROR_TS:
            printf("Error con la TS - Linea: %d\n", linea);
            break;
        case ERROR_ANALIZADOR_LEXICO:
            printf("Error en el analizador lexico - Linea: %d\n", linea);
            break;
        case ERROR_ANALIZADOR_SINTACTICO:
            printf("Error en el analizador sintactico - Linea: %d\n", linea);
            break;
        case ERROR_SISTEMA_DE_ENTRADA:
        case ERROR_TAM_MAX_LEXEMA_SUPERADO:
            printf("Error en el sistema de entrada - Linea: %d\n", linea);
            break;
        default:
            printf("Error desconocido - Linea: %d\n", linea);
            break;
    }
    if (error != NULL){
        printf("Error: %s\n", error);
    }    
}
