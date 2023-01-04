#ifndef AUTOMATAS_H_INCLUDED
#define AUTOMATAS_H_INCLUDED

#include "data.h"

void identificadores(char c);

void numeros(char c);

void caracter_especial(char c);

void declarar_variable(struct Nodo *nodo, int estado);

void verificar_main(struct Nodo *nodo, int estado);

int verificar_expresion(struct Nodo *nodo, int estado);

void imprimir(struct Nodo *nodo, int estado);

void si(struct Nodo *nodo, int estado);

#endif
