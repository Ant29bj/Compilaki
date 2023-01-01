#include "automatas.h"
#include "data.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void identificadores(char c) {
  int estado = 1;
  char cadena[100];
  memset(cadena, 0, sizeof(cadena) - 1);
  while (estado == 1 || estado == 2) {
    if (iscntrl(c)) {
      fila++;
      columna = 0;
    }
    switch (estado) {
    case 1:
      if (isalpha(c)) {
        strncat(cadena, &c, 1);
        c = fgetc(source);
        estado = 2;
      }
      break;

    case 2:
      if (isalnum(c) || c == '_') {
        strncat(cadena, &c, 1);
        c = fgetc(source);
      } else {
        estado = 3;
      }
      break;
    }
  }
  buscar_palabra_reservada(cadena);
  memset(cadena, 0, sizeof(cadena) - 1);

  if (c == ';' || c == '(' || c == ')' || c == '{' || c == '}' || c == '[' ||
      c == ']' || c == ',') {
    caracter_especial(c);
  } else if (isascii(c) && !isspace(c)) {

    printf("Error: invalid %c", c);
    exit(1);
  }
}

void numeros(char c) {
  int estado = 1;
  char numero[999];
  memset(numero, 0, sizeof(numero) - 1);

  while (estado < 6) {
    if (iscntrl(c)) {
      fila++;
      columna = 0;
    }
    switch (estado) {
    case 1:
      if (c == '-' || c == '.') {
        strncat(numero, &c, 1);
        c = fgetc(source);
        estado = 2;
      } else if (isdigit(c)) {

        strncat(numero, &c, 1);
        c = fgetc(source);
        estado = 3;
      }
      break;
    case 2:
      if (c == '.') {
        strncat(numero, &c, 1);
        c = fgetc(source);
        estado = 4;
      } else if (isspace(c)) {
        estado = 8;
      } else if (isdigit(c)) {
        strncat(numero, &c, 1);
        c = fgetc(source);
        estado = 3;
      }
      break;
    case 3:
      if (isdigit(c)) {
        strncat(numero, &c, 1);
        c = fgetc(source);
      } else if (c == '.') {
        strncat(numero, &c, 1);
        c = fgetc(source);
      } else {
        estado = 6;
      }
      break;
    case 4:
      if (isdigit(c)) {
        strncat(numero, &c, 1);
        c = fgetc(source);
        estado = 5;
      }
      if (!isdigit(c)) {
        printf("Entre \n");
        estado = 5;
      }
      break;
    case 5:
      if (isdigit(c)) {
        strncat(numero, &c, 1);
        c = fgetc(source);
      } else {
        estado = 6;
      }
      break;
    }
  }

  if (estado == 6) {
    insertar_numero(numero);
    memset(numero, 0, sizeof(numero) - 1);
  }

  if (estado == 8) {
    insertar_caracter(numero);
    memset(numero, 0, sizeof(numero) - 1);
  }

  if (isascii(c) && !isspace(c)) {
    // caracterEspecial(c);
  }
}

void caracter_especial(char c) {
  int estado = 1;
  char operadores[1000];
  memset(operadores, 0, sizeof(operadores) - 1);
  while (estado < 7) {
    switch (estado) {
    case 1:
      if (c == '+' || c == '-' || c == '*' || c == '/' || c == ';' ||
          c == ':') {
        strncat(operadores, &c, 1);
        c = fgetc(source);
        estado = 7;
      } else if (c == '!' || c == '<' || c == '>' || c == '=') {
        strncat(operadores, &c, 1);
        c = fgetc(source);
        estado = 4;
      } else if (c == '&' || c == '|') {
        strncat(operadores, &c, 1);
        c = fgetc(source);
        estado = 5;
      } else if (c == '(' || c == ')' || c == '{' || c == '}' || c == '[' ||
                 c == ']') {
        strncat(operadores, &c, 1);
        estado = 7;
      } else if (c == '"') {
        strncat(operadores, &c, 1);
        c = fgetc(source);
        estado = 6;
      } else if (isascii(c)) {
        strncat(operadores, &c, 1);
        estado = 7;
      } else {
        estado = 7;
      }
      break;
    case 4:
      if (c == '!' || c == '<' || c == '>' || c == '=') {
        strncat(operadores, &c, 1);
        c = fgetc(source);
        estado = 7;
      } else {
        estado = 7;
      }
      break;
    case 5:
      if (c == '&' && c == '|') {
        strncat(operadores, &c, 1);
        estado = 7;
      }
      break;
    case 6:
      if (c != '"' && c != '\n') {
        strncat(operadores, &c, 1);
        c = fgetc(source);
      } else if (c == '"') {
        strncat(operadores, &c, 1);
        estado = 7;
      } else if (c == '\n') {
        printf("Error: missing element ");
        exit(1);
      }
      break;
    }

    if (estado == 7) {
      insertar_caracter(operadores);
      memset(operadores, 0, sizeof(operadores) - 1);
    }
  }
}
