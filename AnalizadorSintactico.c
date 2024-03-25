#include <stdio.h>
#include "AnalizadorLexico.h"
#include "definiciones.h"
#include <unistd.h>
#include <stdlib.h>

void analisisSintactico(){
    tipoelem componenteLexico;
    while(1){
        componenteLexico = siguienteComponenteLexico();
        if (componenteLexico.valor != EOF){
            printf("Componente lexico -->\tLexema: %d\tValor: %s\n", componenteLexico.valor, componenteLexico.lexema);
        }
        else{
            printf("Componente lexico -->\tLexema: %d\tValor: EOF\n", componenteLexico.valor);
            break;
        }
    }
    liberarMemoria(&componenteLexico);

}