#include "data.h"
#include "automatas.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

char pal_res[][21] = {
    "ent",      "real",    "car",      "discreto", "tirarcodigo", "cuando",
    "sino",     "paranga", "imprimir", "nada",     "regresa",     "importa",
    "nuevo",    "publico", "privado",  "mientras", "bloque",      "clase",
    "mientras", "si",      "sino"};

struct Nodo *raiz;
struct Nodo *actual;

struct Token newToken;

struct Nodo *empty;
struct Nodo *gnext;

struct Pila *base;
struct Pila *cima;

void insertar_nodo(struct Token token) {
  struct Nodo *nuevo;
  nuevo = malloc(sizeof(struct Nodo));

  nuevo->info = token;
  nuevo->izq = NULL;
  nuevo->der = NULL;

  if (raiz == NULL) {
    raiz = nuevo;
    actual = nuevo;
  } else {
    nuevo->izq = actual;
    actual->der = nuevo;
    actual = nuevo;
  }
}

void apilar(struct Nodo nodo) {
  struct Pila *nuevo;
  nuevo = malloc(sizeof(struct Pila));
  nuevo->info = nodo;
  nuevo->der = NULL;
  nuevo->izq = NULL;
  if (base == NULL) {
    base = nuevo;
    cima = nuevo;
  } else {
    nuevo->izq = cima;
    cima->der = nuevo;
    cima = nuevo;
  }
}

void recorrer_pila(struct Pila *pila) {
  if (pila != NULL) {
    recorrer_pila(pila->der);
    printf("Pila:%s\n", pila->info.info.lexema);
  }
}

void recorrer_nodo(struct Nodo *nodo) {
  if (nodo != NULL) {
    // printf("%s \t\t", nodo->info.lexema);
    // printf(" %s \t\t", nodo->info.nombre);
    // printf(" fila:%d\t, columna:%d\n", nodo->info.fila, nodo->info.columna);
    take_token(nodo);
    recorrer_nodo(nodo->der);
  }
}

void buscar_palabra_reservada(char *c) {
  columna++;
  unsigned int array_size = sizeof(*pal_res) / sizeof(*pal_res[0]);
  for (unsigned int i = 0; i < array_size; i++) {
    if (strcmp(c, pal_res[i]) == 0) {
      newToken.tipo = PALRES;
      newToken.nombre = "PALRES";
      strcpy(newToken.lexema, c);
      break;
    } else {
      newToken.tipo = ID;
      newToken.nombre = "IDENTIFICADOR";
      strcpy(newToken.lexema, c);
    }
  }

  newToken.fila = fila;
  newToken.columna = columna;
  insertar_nodo(newToken);
  memset(c, 0, sizeof(c) - 1);
}

void insertar_numero(char *string) {
  columna++;
  newToken.tipo = NUM;
  newToken.nombre = "NUMERO";
  strcpy(newToken.lexema, string);
  newToken.columna = columna;
  newToken.fila = fila;
  insertar_nodo(newToken);
}

void insertar_caracter(char *string) {
  if (string[0] == '"') {
    newToken.tipo = CAD;
    newToken.nombre = "CADENA";
  } else {
    newToken.nombre = "SIMBOLO";
    newToken.tipo = SIM;
  }
  columna++;
  strcpy(newToken.lexema, string);
  newToken.columna = columna;
  newToken.fila = fila;
  insertar_nodo(newToken);
}

// --------------------------- Sintactico --------------------------

void take_token(struct Nodo *nodo) {
  if (nodo->info.tipo == 0) {
    if (!strcasecmp(nodo->info.lexema, "ent") ||
        !strcasecmp(nodo->info.lexema, "real") ||
        !strcasecmp(nodo->info.lexema, "car") ||
        !strcasecmp(nodo->info.lexema, "discreto")) {
      declarar_variable(nodo->der, 0);
    } else if (!strcasecmp(nodo->info.lexema, "tirarcodigo")) {
      verificar_main(nodo->der, 0);
    } else if (!strcasecmp(nodo->info.lexema, "imprimir")) {
      imprimir(nodo->der, 0);
    } else if (!strcasecmp(nodo->info.lexema, "si")) {
      si(nodo->der, 0);
    }
  } else {
    gnext = empty;
    gnext = nodo->der;
  }
}
