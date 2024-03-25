// Constantes y definiciones para el analizador léxico

// -- Constantes --
#define TAM_MAX_LEXEMA 52
#define TAM_LEXEMAS_UNICARACTER 13

// -- Tokens -- 
#define DEF 300
#define IMPORT 301
#define AS 302
#define FOR 303
#define IN 304
#define IF 305
#define ELIF 306
#define ELSE 307
#define RETURN 308
#define NOT 309
#define ID 310
#define INTEGER 311
#define FLOATNUMBER 312
#define STRING 313

// -- Tokens de operadores de mas de un caracter --
#define MAYOR_IGUAL 411 // >=
#define MENOR_IGUAL 412 // <=
#define DISTINTO 414 // !=
#define COMPARACION_IGUAL 415 // ==
#define MAS_IGUAL 416 // +=
#define MENOS_IGUAL 417 // -=
#define POR_IGUAL 418 // *=
#define DIVIDIDO_IGUAL 419 // /=
#define MODULO_IGUAL 420 // %=
#define EXPONENTE 421 // **

// -- Tokens de gestion de errores --
#define ERROR_SISTEMA_DE_ENTRADA 500
#define ERROR_ANALIZADOR_LEXICO 501
#define ERROR_ANALIZADOR_SINTACTICO 502
#define ERROR_TS 503
#define ERROR_TAM_MAX_LEXEMA_SUPERADO 504