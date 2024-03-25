#include <stdio.h>
#include "AnalizadorLexico.h"
#include "AnalizadorSintactico.h"
#include "definiciones.h"
#include "TS.h"
#include "SistemaEntrada.h"


int main(int argc, char *argv[]) {
    // Verificamos que se haya ingresado un archivo de entrada
    if (argc != 2){
        printf("Error: Debe ingresar un unico archivo de entrada\n");
        return 1;
    }

    // Inicializar tabla de simbolos
    initTS();
    printf("--- TABLA DE SIMBOLOS ---\n");
    imprimirTS();

    // Inicializamos el sistema de entrada
    inicializarSistemaEntrada(argv[1]);


    // Empezamos el analisis lexico
    printf("\n\n--- ANALISIS SINTATICO ---\n");
    analisisSintactico();

    printf("\n\n--- TABLA DE SIMBOLOS ---\n");
    imprimirTS();

    // Cerramos el archivo y liberamos memoria
    printf("\n--- Liberando memoria... ---\n");
    finalizarSistemaEntrada();
    destruirTS();

    printf("\n--- Fin del programa ---\n");
    return 0;
}