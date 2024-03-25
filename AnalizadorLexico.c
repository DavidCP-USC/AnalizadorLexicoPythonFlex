#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "definiciones.h"
#include "AnalizadorLexico.h"
#include "SistemaEntrada.h"
#include "TS.h"
#include "Errores.h"

char lexemasIndividuales[TAM_LEXEMAS_UNICARACTER] = "()[]{}.,:;!&|";
tipoelem returnValue;
int error = 0;

void _saltarComentario(char *caracter);
int  _identificarSiComentarioMultilinea() ;
void _saltarComentarioMultilinea(char *caracter);
int _comprobarSiPuntoEsParteDeNumero();
int _esLexemaUnicaracter(char cadena);
void _recuperarLexema(int tipo, int retroceder);
void _identificarCadenasAlfanumericas(char *caracter);
void _identificarNumeros(char *caracter, int *estado, int *terminado);
void _identificarNumerosHexadecimales(char *caracter);
void _identificarStrings(char *caracter, char tipoDeComillas);
void _identificarOperadores(char *caracter);
void liberarMemoria();

// Devuelve el siguiente componente lexico al analizador sintactico
tipoelem siguienteComponenteLexico(){
    error = 0;
    char caracter;
    char tipoDeComillas;
    int estado = 0;
    int terminado = 0;
    int terminadoIndicadoEnAutomata = 0;


    while (!(terminado || error)){
        switch (estado){
            case 0:
                caracter = siguienteCaracter();
                if (caracter == '#'){
                    estado = 1; // Automata de comentarios de una linea
                }
                else if(caracter == '"'){
                    if (_identificarSiComentarioMultilinea()){
                        estado = 2; // Automata de comentarios multilinea
                    }
                    else{
                        estado = 5; // Automata de strings
                        tipoDeComillas = '"';
                    }
                }
                else if(caracter == '.'){
                    if (_comprobarSiPuntoEsParteDeNumero()){
                        estado = 4; // Automata de numeros
                    }
                    else{
                        estado = 7; // Automata de lexemas de un caracter que no sean operadores ni letras
                    }
                }
                else if (isalpha(caracter) || caracter == '_'){
                    estado = 3; // Automata de cadenas alfanumericas
                }
                else if (isdigit(caracter)){
                    estado = 4; // Automata de numeros
                }
                else if (caracter == '\''){ // Las " ya se han comprobado antes
                    tipoDeComillas = '\'';
                    estado = 5; // Automata de strings
                }
                else if(caracter == '+' || caracter == '-' || caracter == '*' || caracter == '/' || caracter == '<' || caracter == '>' || caracter == '='){
                    estado = 6; // Automata de operadores
                }
                else if (_esLexemaUnicaracter(caracter)){
                    estado = 7; // Automata de lexemas de un caracter que no sean operadores ni letras
                }
                else if (caracter == EOF) { // Si encontramos un EOF terminamos y liberamos la memoria
                    terminado = 1;
                    if (returnValue.lexema != NULL){
                        free(returnValue.lexema);
                        returnValue.lexema = NULL;
                    }
                    returnValue.valor = EOF;
                    returnValue.lexema = malloc(0);
                }
                else if(isblank(caracter) || caracter == '\n'){
                    // Puede haber multiples espacios en blanco o saltos de linea
                    // entre componentes lexicos, por lo que nos los saltamos
                    while (isblank(caracter) || caracter == '\n'){
                        if (caracter == '\n'){
                            aumentarContadorLineas();
                        }
                        caracter = siguienteCaracter();
                    }
                    retrocederCaracter();
                    aceptarLexema();
                }
                else{
                    // Imprimimos el error
                    printTipoError(ERROR_ANALIZADOR_LEXICO, "Caracter no reconocido\n");
                    error = 1;
                }
                break;
            case 1: // Identificar comentarios
                _saltarComentario(&caracter);
                estado = 0;
                break;
            case 2: // Identificar comentarios multilinea
                _saltarComentarioMultilinea(&caracter);
                estado = 0;
                break;
            case 3: // Identificar cadenas
                _identificarCadenasAlfanumericas(&caracter);
                // Hemos identificado una cadena, terminamos
                terminado = 1;
                break;
            case 4: // Identificar numeros
                _identificarNumeros(&caracter, &estado, &terminadoIndicadoEnAutomata);
                terminado = terminadoIndicadoEnAutomata;
                break;
            case 5: // Identificar strings
                _identificarStrings(&caracter, tipoDeComillas);
                terminado = 1;
                break;
            case 6: // Identificar operadores
                _identificarOperadores(&caracter);
                terminado = 1;
                break;
            case 7: // Identificar delimitadores
                _recuperarLexema(caracter, 0);
                terminado = 1;
                break;
            default:
                printTipoError(ERROR_ANALIZADOR_LEXICO, "Estado no reconocido\n");
                error = 1;
                break;

        }
    }
               
    return returnValue;
}

// Salta los comentarios de una linea
void _saltarComentario(char *caracter){
    while (*caracter != '\n'){
        *caracter = siguienteCaracter();
        aceptarLexema();
    }
    aumentarContadorLineas();
    // La funcion prepara el buffer para el siguiente componente 
    // lexico, por eso se va ejecutando mientras se salta el comentario
}

// Identifica si estamos ante un comentario multilinea
int  _identificarSiComentarioMultilinea(){    
    if (siguienteCaracter() == '"'){ // Posible comentario multilinea
        if (siguienteCaracter() == '"'){ // ES un comentario multilinea
            return 1;
        }
        else{
            retrocederCaracter();
            retrocederCaracter();
        }
    }
    else{
        retrocederCaracter();
    }
    return 0;
}

// Salta los comentarios multilinea
void _saltarComentarioMultilinea(char *caracter){
    while (1){
        *caracter = siguienteCaracter();
        if (*caracter == '\n'){
            aumentarContadorLineas();
        }
        aceptarLexema();        
        if (*caracter == '"'){
            *caracter = siguienteCaracter();
            aceptarLexema();
            if (*caracter == '\n'){
                aumentarContadorLineas();
            }
            if (*caracter == '"'){
                *caracter = siguienteCaracter();
                aceptarLexema();
                if (*caracter == '\n'){
                    aumentarContadorLineas();
                }
                if (*caracter == '"'){
                    *caracter = siguienteCaracter();
                    // Podria darse el caso de que el siguiente caracter no sea \n
                    retrocederCaracter();
                    aceptarLexema();
                    break;
                }
            }
        }
        if (*caracter == EOF){
            printf("comentario multilinea no cerrado\n");
            break;
        }
    }
    
}

// Comprueba si el punto que se acaba de leer es parte de un numero
int _comprobarSiPuntoEsParteDeNumero(){
    if (isdigit(siguienteCaracter())){
        return 1;
    }
    else{
        retrocederCaracter();
        return 0;
    }
}

// Identifica cadenas alfanumericas
void _identificarCadenasAlfanumericas(char *caracter){
    do{
        *caracter = siguienteCaracter();
    }while(isalpha(*caracter) || isdigit(*caracter) || *caracter == '_');
    _recuperarLexema(ID, 1);
}

// Identifica numeros
void _identificarNumeros(char *caracter, int *estado, int *terminado){ 
    char caracterSiguiente = *caracter;
    int esFloat = 0;
    // Comprobamos si es un numero en notacion hexadecimal
    if (*caracter == '0'){
        caracterSiguiente = siguienteCaracter();
        if (caracterSiguiente == 'x' || caracterSiguiente == 'X'){
            _identificarNumerosHexadecimales(caracter);
            *terminado = 1;
            return;
        }
        *caracter = caracterSiguiente;
    }

    // Leemos hasta encontrar un caracter que no sea un digito
    if (isdigit(*caracter)){
        do{
            *caracter = siguienteCaracter();
        }while (isdigit(*caracter));
    }

    // Comprobamos si es un numero en notacion decimal
    if (*caracter == '.'){
        esFloat = 1;
        *caracter = siguienteCaracter();
        if (isdigit(*caracter)){
            do{
                *caracter = siguienteCaracter();
            }while (isdigit(*caracter));
        }
    }

    // Comprobamos si es un numero en notacion cientifica
    // (exponencial)
    if(*caracter == 'e' || *caracter == 'E'){
        *caracter = siguienteCaracter();
        if (*caracter == '+' || *caracter == '-'){
            *caracter = siguienteCaracter();
        }
        if (isdigit(*caracter)){
            do{
                *caracter = siguienteCaracter();
            }while (isdigit(*caracter));
        }
        else{
            printTipoError(ERROR_ANALIZADOR_LEXICO, "Numero mal formado\n");
            error = 1;
            return;
        }
        esFloat = 1;
    }

    // Hemos terminado de leer el numero
    // aceptamos el lexema
    if (_esLexemaUnicaracter(*caracter) || isblank(*caracter) || *caracter == '\n' || *caracter == EOF || *caracter == '+' || *caracter == '-' || *caracter == '*' || *caracter == '/' || *caracter == '<' || *caracter == '>' || *caracter == '='){
        if (esFloat){
            _recuperarLexema(FLOATNUMBER, 1);
        }
        else{
            _recuperarLexema(INTEGER, 1);            
        }
        *terminado = 1;
    }
    else{
        printTipoError(ERROR_ANALIZADOR_LEXICO, "Numero mal formado\n");
        error = 1;
        return;
    }
}

// Identifica numeros en notacion hexadecimal
void _identificarNumerosHexadecimales(char *caracter){
    do{
        *caracter = siguienteCaracter();
    }while (isxdigit(*caracter));
    if (_esLexemaUnicaracter(*caracter) || isblank(*caracter) || *caracter == '\n' || *caracter == EOF){
        if (*caracter == '\n'){
            aumentarContadorLineas();
        }
        _recuperarLexema(FLOATNUMBER, 1);
    }
    else{
        printTipoError(ERROR_ANALIZADOR_LEXICO, "Numero mal formado\n");
        error = 1;
        return;
    }

}

// Identifica strings
void _identificarStrings(char *caracter, char tipoDeComillas){
    do{
        *caracter = siguienteCaracter();
        if (*caracter == EOF){
            printTipoError(ERROR_ANALIZADOR_LEXICO, "Se llego al final de larchivo pero no se cerro el string\n");
            return;
        }
    }while (*caracter != tipoDeComillas);

    _recuperarLexema(STRING, 0);
}

// Identifica operadores
void _identificarOperadores(char *caracter){
    char caracterAnterior = *caracter;
    // Comrobamos si es '=' o '=='
    if (*caracter == '='){
        *caracter = siguienteCaracter();
        if (*caracter == '='){
            _recuperarLexema(COMPARACION_IGUAL, 0);
            return;
        }
        else{
            _recuperarLexema(caracterAnterior, 1);
        }
    }
    // Comprobamos si es '<' o '<='
    else if (*caracter == '<'){
        *caracter = siguienteCaracter();
        if (*caracter == '='){
            _recuperarLexema(MENOR_IGUAL, 0);
            return;
        }
        else{
            _recuperarLexema(caracterAnterior, 1);
        }
    }
    // Comprobamos si es '>' o '>='
    else if (*caracter == '>'){
        *caracter = siguienteCaracter();
        if (*caracter == '='){
            _recuperarLexema(MAYOR_IGUAL, 0);
            return;
        }
        else{
            _recuperarLexema(caracterAnterior, 1);
        }
    }
    // Comprobamos si es '!' o '!='
    else if (*caracter == '!'){
        *caracter = siguienteCaracter();
        if (*caracter == '='){
            _recuperarLexema(DISTINTO, 0);
            return;
        }
        else{
            printTipoError(ERROR_ANALIZADOR_LEXICO, "Operador no reconocido\n");
            error = 1;
            return;
        }
    }
    // Comprobamos si es '+' o '+='
    else if (*caracter == '+'){
        *caracter = siguienteCaracter();
        if (*caracter == '='){
            _recuperarLexema(MAS_IGUAL, 0);
            return;
        }
        else{
            _recuperarLexema(caracterAnterior, 1);
        }
    }
    // Comprobamos si es '-' o '-='
    else if (*caracter == '-'){
        *caracter = siguienteCaracter();
        if (*caracter == '='){
            _recuperarLexema(MENOS_IGUAL, 0);
            return;
        }
        else{
            _recuperarLexema(caracterAnterior, 1);
        }
    }
    // Compribamos si es '*' o '*=' o '**'
    else if (*caracter == '*'){
        *caracter = siguienteCaracter();
        if (*caracter == '='){
            _recuperarLexema(POR_IGUAL, 0);
            return;
        }
        else if(*caracter == '*'){
            _recuperarLexema(EXPONENTE, 0);
            return;
        }
        else{
            _recuperarLexema(caracterAnterior, 1);
        }
    }
    // Comprobamos si es '/' o '/='
    else if (*caracter == '/'){
        *caracter = siguienteCaracter();
        if (*caracter == '='){
            _recuperarLexema(DIVIDIDO_IGUAL, 0);
            return;
        }
        else{
            _recuperarLexema(caracterAnterior, 1);
        }
    }
    // Comprobamos si es '%' o '%='
    else if (*caracter == '%'){
        *caracter = siguienteCaracter();
        if (*caracter == '='){
            _recuperarLexema(MODULO_IGUAL, 0);
            return;
        }
        else{
            _recuperarLexema(caracterAnterior, 1);
        }
    }

    else if (isblank(*caracter) || *caracter == '\n' || *caracter == EOF){
        if (*caracter == '\n'){
            aumentarContadorLineas();
        }
        _recuperarLexema(caracterAnterior, 1);
    }
    else{
        printTipoError(ERROR_ANALIZADOR_LEXICO, "Operador no reconocido\n");
        error = 1;
        return;
    }
}

// Recupera el lexema y lo acepta
void _recuperarLexema(int tipo, int retroceder){
    if (retroceder){
        retrocederCaracter();
    }
    if (tipo == ID){
        obtenerLexema(&returnValue);
        returnValue.valor = obtenerValorTS(returnValue.lexema);
    }
    else if (tipo == INTEGER){
        obtenerLexema(&returnValue);
        returnValue.valor = INTEGER;
    }
    else if (tipo == FLOATNUMBER){
        obtenerLexema(&returnValue);
        returnValue.valor = FLOATNUMBER;
    }
    else if (tipo == MAS_IGUAL){
        returnValue.valor = MAS_IGUAL;
        if (returnValue.lexema != NULL){
            free(returnValue.lexema);
        }
        returnValue.lexema = (char*)malloc(3 * sizeof(char));
        returnValue.lexema[0] = '+';
        returnValue.lexema[1] = '=';
        returnValue.lexema[2] = '\0';
    }
    else if (tipo == MENOS_IGUAL){
        returnValue.valor = MENOS_IGUAL;
        if (returnValue.lexema != NULL){
            free(returnValue.lexema);
        }
        returnValue.lexema = (char*)malloc(3 * sizeof(char));
        returnValue.lexema[0] = '-';
        returnValue.lexema[1] = '=';
        returnValue.lexema[2] = '\0';
    }
    else if (tipo == POR_IGUAL){
        returnValue.valor = POR_IGUAL;
        if (returnValue.lexema != NULL){
            free(returnValue.lexema);
        }
        returnValue.lexema = (char*)malloc(3 * sizeof(char));
        returnValue.lexema[0] = '*';
        returnValue.lexema[1] = '=';
        returnValue.lexema[2] = '\0';
    }
    else if (tipo == DIVIDIDO_IGUAL){
        returnValue.valor = DIVIDIDO_IGUAL;
        if (returnValue.lexema != NULL){
            free(returnValue.lexema);
        }
        returnValue.lexema = (char*)malloc(3 * sizeof(char));
        returnValue.lexema[0] = '/';
        returnValue.lexema[1] = '=';
        returnValue.lexema[2] = '\0';
    }
    else if (tipo == MODULO_IGUAL){
        returnValue.valor = MODULO_IGUAL;
        if (returnValue.lexema != NULL){
            free(returnValue.lexema);
        }
        returnValue.lexema = (char*)malloc(3 * sizeof(char));
        returnValue.lexema[0] = '%';
        returnValue.lexema[1] = '=';
        returnValue.lexema[2] = '\0';
    }
    else if (tipo == EXPONENTE){
        returnValue.valor = EXPONENTE;
        if (returnValue.lexema != NULL){
            free(returnValue.lexema);
        }
        returnValue.lexema = (char*)malloc(3 * sizeof(char));
        returnValue.lexema[0] = '*';
        returnValue.lexema[1] = '*';
        returnValue.lexema[2] = '\0';
    }
    else if (tipo == MENOR_IGUAL){
        returnValue.valor = MENOR_IGUAL;
        if (returnValue.lexema != NULL){
            free(returnValue.lexema);
        }
        returnValue.lexema = (char*)malloc(3 * sizeof(char));
        returnValue.lexema[0] = '<';
        returnValue.lexema[1] = '=';
        returnValue.lexema[2] = '\0';
    }
     else if (tipo == MAYOR_IGUAL){
        returnValue.valor = MAYOR_IGUAL;
        if (returnValue.lexema != NULL){
            free(returnValue.lexema);
        }
        returnValue.lexema = (char*)malloc(3 * sizeof(char));
        returnValue.lexema[0] = '>';
        returnValue.lexema[1] = '=';
        returnValue.lexema[2] = '\0';
    }
    else if (tipo == DISTINTO){
        returnValue.valor = DISTINTO;
        if (returnValue.lexema != NULL){
            free(returnValue.lexema);
        }
        returnValue.lexema = (char*)malloc(3 * sizeof(char));
        returnValue.lexema[0] = '!';
        returnValue.lexema[1] = '=';
        returnValue.lexema[2] = '\0';
    }
    else if (tipo == COMPARACION_IGUAL){
        returnValue.valor = COMPARACION_IGUAL;
        if (returnValue.lexema != NULL){
            free(returnValue.lexema);
        }
        returnValue.lexema = (char*)malloc(3 * sizeof(char));
        returnValue.lexema[0] = '=';
        returnValue.lexema[1] = '=';
        returnValue.lexema[2] = '\0';
    }
    else if (tipo == STRING){
        returnValue.valor = STRING;
        obtenerLexema(&returnValue);
    }

    else{ // Para lexemas que solo tienen un caracter
        returnValue.valor = tipo;
        if (returnValue.lexema != NULL){
            free(returnValue.lexema);
        }
        returnValue.lexema = (char*)malloc(2 * sizeof(char));
        returnValue.lexema[0] = tipo;
        returnValue.lexema[1] = '\0';
    }
    aceptarLexema();
}

// Comprueba si el caracter es un lexema de un caracter que esta
// almacenado en el array lexemasIndividuales
int _esLexemaUnicaracter(char cadena){
    for (int i = 0; i < TAM_LEXEMAS_UNICARACTER; i++){
        if (cadena == lexemasIndividuales[i]){
            return 1;
        }
    }
    return 0;
}

// Libera la memoria
void liberarMemoria(tipoelem *componenteLexico){
    if (componenteLexico->lexema != NULL){
        free(componenteLexico->lexema);
        componenteLexico->lexema = NULL;
    }
}