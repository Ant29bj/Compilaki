#include "automatas.h"
#include "data.h"
#include <ctype.h>
#include <stdio.h>

extern struct Nodo *raiz;

FILE *source;
int fila, columna;

int main() {
  source = fopen("./source.txt", "r");
  char character = fgetc(source);
  while (character != EOF) {
    if (isalpha(character)) {
      identificadores(character);
    } else if (isdigit(character) || character == '-' || character == '.') {
      numeros(character);
    } else if (isascii(character) && !isspace(character)) {
      caracter_especial(character);
    }
    character = fgetc(source);
  }

  recorrer_nodo(raiz);
}
