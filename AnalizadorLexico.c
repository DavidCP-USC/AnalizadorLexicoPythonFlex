#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "definiciones.h"
#include "AnalizadorLexico.h"
#include "lex.yy.c"

char lexemasIndividuales[TAM_LEXEMAS_UNICARACTER] = "()[]{}.,:;!&|";
tipoelem returnValue;
int error = 0;

void liberarMemoria();
void leerFichero (char* nombre);
void cerrarFichero();

// Devuelve el siguiente componente lexico al analizador sintactico
tipoelem siguienteComponenteLexico(){
    returnValue.valor = yylex();
    returnValue.lexema = malloc(sizeof(char) *yyleng);

    if(returnValue.valor == 2){ // Se llego al EOF
            returnValue.valor = EOF;
            free(returnValue.lexema);
            returnValue.lexema = NULL;
    }
    else{
        returnValue.lexema = strcpy(returnValue.lexema, yytext);
    }

    if(returnValue.valor == 3){ // Se encontro un posible identificador
        returnValue.valor = obtenerValorTS(yytext);

    }
                
    return returnValue;
}


// Libera la memoria
void liberarMemoria(tipoelem *componenteLexico){
    if (componenteLexico->lexema != NULL){
        free(componenteLexico->lexema);
        componenteLexico->lexema = NULL;
    }
}

void leerFichero (char* nombre){
    yyin = fopen(nombre, "r");
    if (yyin == NULL){
        printTipoError(ERROR_SISTEMA_DE_ENTRADA, "No se pudo abrir el archivo\n", 0);
        exit(1);
    }
}

void cerrarFichero(){
    fclose(yyin);
}