#ifndef SISTEMAENTRADA_H
#define SISTEMAENTRADA_H

#include <stdio.h>
#include <stdlib.h>
#include "definiciones.h"
#include "abin.h"

#define CENTINELA1 TAM_MAX_LEXEMA
#define CENTINELA2 TAM_MAX_LEXEMA*2+1
#define TAMANO_DOBLE_BUFFER TAM_MAX_LEXEMA*2+2
#define INICIO_BLOQUE2 TAM_MAX_LEXEMA+1 

/*
Se va a utilizar el método del doble buffer con un unico array
pero se trabajara como dos bloques logicos
*/



void _imprimirTamano();

void inicializarSistemaEntrada(char* nombreArchivo);

char siguienteCaracter();

void retrocederCaracter();

void devolverCaracter();

void finalizarSistemaEntrada();

void obtenerLexema(tipoelem *returnValue);

void aceptarLexema();

void aumentarContadorLineas();

int obtenerContadorLineas();


#endif