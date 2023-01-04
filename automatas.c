#include "automatas.h"
#include "data.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern int columna;
extern int fila;

void identificadores(char c) {
  int estado = 1;
  char cadena[100];
  memset(cadena, 0, sizeof(cadena) - 1);
  while (estado == 1 || estado == 2) {
    if (c == '\n') {
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
    if (c == '\n') {
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
    caracter_especial(c);
  }
}

void caracter_especial(char c) {
  int estado = 1;
  char operadores[1000];
  memset(operadores, 0, sizeof(operadores) - 1);
  while (estado < 7) {
    if (c == '\n') {
      fila++;
      columna = 0;
    }
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

// --------------------------- Sintactico --------------------------
extern struct Nodo *gnext;
extern struct Nodo *empty;
// Declaracion de Variables del tipo [tipo variable] <Nombre variable> ||
// <Coma><Nombre Variable> || Terminal
void declarar_variable(struct Nodo *nodo, int estado) {
  if (nodo != NULL) {
    switch (estado) {
    case 0:
      if (nodo->info.tipo == 1) {
        declarar_variable(nodo->der, 1);
      } else {
        printf("error on fila %d", nodo->info.fila);
        printf(" columna %d\n", nodo->info.columna);
      }
      break;
    case 1:
      if (nodo->info.lexema[0] == ';') {
        gnext = empty;
        gnext = nodo->der;
      } else if (nodo->info.lexema[0] == ',') {
        declarar_variable(nodo->der, 0);
      } else {
        printf("error on fila %d", nodo->info.fila);
        printf(" columna %d\n", nodo->info.columna);
      }
      break;
    default:
      break;
    }
  }
}
// verificar trirarcodigo trirarcodigo(){}
void verificar_main(struct Nodo *nodo, int estado) {
  if (nodo != NULL) {
    switch (estado) {
    case 0:
      if (!strcmp(nodo->info.lexema, "(")) {
        verificar_main(nodo->der, 1);
      } else {
        printf("Error en fila %d ", nodo->info.fila);
        printf("columna %d ", nodo->info.columna);
        printf("en %s\n", nodo->info.lexema);
        gnext = empty;
        gnext = nodo->der;
      }
      break;
    case 1:
      if (!strcmp(nodo->info.lexema, ")")) {
        verificar_main(nodo->der, 2);
      } else {
        printf("Error en fila %d ", nodo->info.fila);
        printf("columna %d ", nodo->info.columna);
        printf("en %s\n", nodo->info.lexema);
        gnext = empty;
        gnext = nodo->der;
      }
      break;
    case 2:
      if (!strcmp(nodo->info.lexema, "{")) {
        apilar(*nodo);
        gnext = empty;
        gnext = nodo->der;
      } else {
        printf("Error en fila %d ", nodo->info.fila);
        printf("columna %d ", nodo->info.columna);
        printf("en %s\n", nodo->info.lexema);
        gnext = empty;
        gnext = nodo->der;
      }
      break;
    default:
      break;
    }
  }
}

void imprimir(struct Nodo *nodo, int estado) {
  if (nodo != NULL) {
    switch (estado) {
    case 0:
      if (nodo->info.lexema[0] == '(') {
        imprimir(nodo->der, 1);
      } else {
        printf("Error en fila %d ", nodo->info.fila);
        printf("columna %d ", nodo->info.columna);
        printf("en %s\n", nodo->info.lexema);
        gnext = empty;
        gnext = nodo->der;
      }
      break;
    case 1:
      if (nodo->info.tipo == 3) {
        imprimir(nodo->der, 2);
      } else {
        printf("Error en fila %d ", nodo->info.fila);
        printf("columna %d ", nodo->info.columna);
        printf("en %s\n", nodo->info.lexema);
        gnext = empty;
        gnext = nodo->der;
      }
      break;
    case 2:
      if (nodo->info.lexema[0] == ')') {
        imprimir(nodo->der, 3);
      } else {
        printf("Error en fila %d ", nodo->info.fila);
        printf("columna %d ", nodo->info.columna);
        printf("en %s\n", nodo->info.lexema);
        gnext = empty;
        gnext = nodo->der;
      }
      break;
    case 3:
      if (nodo->info.lexema[0] == ';') {
        gnext = empty;
        gnext = nodo->der;
      } else {
        printf("Error en fila %d ", nodo->info.fila);
        printf("columna %d ", nodo->info.columna);
        printf("en %s\n", nodo->info.lexema);
        gnext = empty;
        gnext = nodo->der;
      }
      break;
    default:

      break;
    }
  }
}

void si(struct Nodo *nodo, int estado) {
  switch (estado) {
  case 0:
    if (!strcmp(nodo->info.lexema, "(")) {
      si(nodo->der, 1);
    } else {
      printf("Error en fila %d ", nodo->info.fila);
      printf("columna %d ", nodo->info.columna);
      printf("en %s\n", nodo->info.lexema);
      gnext = empty;
      gnext = nodo->der;
    }
    break;
  case 1:
    if (verificar_expresion(nodo, 0)) {
      si(nodo->der, 2);
    } else {
      printf("Error en fila %d", nodo->info.fila);
      printf(" columna %d\n", nodo->info.columna);
      gnext = empty;
      gnext = nodo->der;
    }
    break;
  case 2:
    if (nodo->info.lexema[0] == ')') {
      si(nodo->der, 3);
    } else {
      printf("este error\n");
      printf("Error en fila %d", nodo->info.fila);
      printf(" columna %d\n", nodo->info.columna);
      gnext = empty;
      gnext = nodo->der;
    }
    break;
  case 3:
    if (nodo->info.lexema[0] == '{') {
      apilar(*nodo);
      gnext = empty;
      gnext = nodo->der;
    } else {
      printf("Error en fila %d", nodo->info.fila);
      printf(" columna %d\n", nodo->info.columna);
      gnext = empty;
      gnext = nodo->der;
    }
    break;
  }
}

int verificar_expresion(struct Nodo *nodo, int estado) {
  int resultado;
  switch (estado) {
  case 0:
    if (nodo->info.tipo == 1 || nodo->info.tipo == 2) {
      verificar_expresion(nodo->der, 1);
    } else {
      resultado = 0;
      printf("Error en fila %d", nodo->info.fila);
      printf(" columna %d\n", nodo->info.columna);
      gnext = empty;
      gnext = nodo->der;
    }
    break;
  case 1:
    if (nodo->info.tipo == 5 && nodo->info.lexema[0] != ';') {
      verificar_expresion(nodo->der, 2);
    } else if (nodo->info.lexema[0] == ';') {
      gnext = empty;
      gnext = nodo;
    } else {
      resultado = 0;
      printf("Error en fila %d", nodo->info.fila);
      printf(" columna %d\n", nodo->info.columna);
      gnext = empty;
      gnext = nodo->der;
    }
    break;
  case 2:
    if (nodo->info.tipo == 1 || nodo->info.tipo == 2) {
      gnext = empty;
      gnext = nodo->der;
      resultado = 1;
    } else {
      resultado = 0;
      printf("Error en fila %d ", nodo->info.fila);
      printf("columna %d ", nodo->info.columna);
      printf("en %s\n", nodo->info.lexema);
      gnext = empty;
      gnext = nodo->der;
    }
    break;
  }
  return resultado;
}

void ident_struct(struct Nodo *nodo, int estado) {
  if (nodo != NULL) {
    switch (estado) {
    case 0:
      if (nodo->info.lexema[0] == '=') {
        ident_struct(nodo->der, 1);
      } else {
        printf("Error en fila %d", nodo->info.fila);
        printf(" columna en %d \n", nodo->info.columna);
        gnext = empty;
        gnext = nodo->der;
      }
      break;
    case 1:
      if (verificar_expresion(nodo, 0)) {
        ident_struct(gnext, 2);
      } else if (nodo->info.tipo == 1 || nodo->info.tipo == 2 ||
                 nodo->info.tipo == 3) {
        ident_struct(nodo->der, 2);
      } else {
        printf("Error en fila %d", nodo->info.fila);
        printf(" columna en %d", nodo->info.columna);
        gnext = empty;
        gnext = nodo->der;
      }
      break;
    case 2:
      if (nodo->info.lexema[0] == ';') {
        gnext = empty;
        gnext = nodo->der;
      } else {
        printf("Error en fila %d ", nodo->info.fila);
        printf("columna %d ", nodo->info.columna);
        printf("en %s\n", nodo->info.lexema);
        gnext = empty;
        gnext = nodo->der;
      }
      break;
    default:

      break;
    }
  }
}
