#include <stdio.h>
#include <stdlib.h>
#include "definiciones.h"
#include "TS.h"


abin TS;

void initTS(){
    crear(&TS);
    insertar(&TS, DEF, "def");
    insertar(&TS, IMPORT, "import");
    insertar(&TS, AS, "as");
    insertar(&TS, FOR, "for");
    insertar(&TS, IN, "in");
    insertar(&TS, IF, "if");
    insertar(&TS, ELIF, "elif");
    insertar(&TS, ELSE, "else");
    insertar(&TS, RETURN, "return");
    insertar(&TS, NOT, "not");
}

int obtenerValorTS(char* lexema){
    return buscar_lexema(&TS, lexema).valor;
}

void imprimirTS (){
    imprimir(&TS);
}

void destruirTS(){
    destruir(&TS);
}