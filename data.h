#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED
#include <stdio.h>

extern FILE *source;

extern char character;

extern char pal_res[][21];

extern int fila;

extern int columna;

enum TipoToken { PALRES, ID, NUM, CAD, CAR, SIM };

struct Token {
  enum TipoToken tipo;
  char *nombre;
  int fila;
  int columna;
  char lexema[80];
};

struct Nodo {
  struct Token info;
  struct Nodo *izq;
  struct Nodo *der;
};

void buscar_palabra_reservada(char *c);

void recorrer_nodo(struct Nodo *nodo);

void insertar_numero(char *string);

void insertar_caracter(char *string);

#endif // DATA_H_INCLUDED
