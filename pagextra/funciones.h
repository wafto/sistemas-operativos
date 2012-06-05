#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../util/list.h"

#define FISICA  1
#define VIRTUAL 2

typedef struct {
	int pid;
} Proceso;


typedef struct {
	Proceso* proceso;
	int num;
} Pagina;

typedef struct {
	int indice;
	Pagina pagina;	
} Marco;

typedef struct {
	int tipo;
	Marco* marcos;
	int tam;
} Memoria;

typedef struct {
	Memoria memfisica;
	Memoria memvirtual;	
} Paginacion;

int crearMemoria(Memoria* mem, int tam, int tipo);
int crearPaginacion(Paginacion* pag, int tam);
void imprimePaginacion(Paginacion pag);

#endif