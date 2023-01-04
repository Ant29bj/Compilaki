#include "automatas.h"
#include "data.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

extern struct Nodo *raiz;

FILE *source;
int fila = 0, columna = 0;

int main() {
  extern struct Pila *base;
  source = fopen("./source.txt", "r");
  if (source == NULL) {
    printf("Cannot get file\n");
    exit(1);
  }
  char character = fgetc(source);
  while (character != EOF) {
    if (character == '\n') {
      fila++;
      columna = 0;
    }
    if (isalpha(character)) {
      identificadores(character);
    } else if (isdigit(character) || character == '-' || character == '.') {
      numeros(character);
    } else if (isascii(character) && !isspace(character)) {
      caracter_especial(character);
    }
    if (iscntrl(character)) {
      fila++;
    }
    character = fgetc(source);
  }

  recorrer_nodo(raiz);
  recorrer_pila(base);
}
