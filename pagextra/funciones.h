#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../util/list.h"


typedef struct {
	/*Proceso* proceso;*/
	int num;
} Pagina;

typedef struct {
	int indice;
	Pagina pagina;	
} Marco;

typedef struct {
	Marco* marcos;
	int tam;
} Memoria;

int crearMemoria(Memoria* mem, int tam);

#endif