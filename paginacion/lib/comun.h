#ifndef _COMUN_H
#define _COMUN_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "particiones.h"

typedef struct {
	long tipo;
	int accion;
	Paginacion* paginacion;
} NodoIPC;

typedef struct {
	key_t llave;
	int cola;
	NodoIPC nodo;
} ColaIPC;

/* Tipos de Nodos, especifican la direccion de la comunicacion */
#define CONTROL_MEMORIA 1
#define MEMORIA_CONTROL 2

/* Acciones de los Nodos */
#define AGREGAR_SOLICITUD 1

#define IPC_LONGITUD (sizeof(NodoIPC) - sizeof(long))
#define FLLAVE "/bin/ls"
#define CLAVE 33

int inicializar(ColaIPC*);
int enviar(ColaIPC*, long, int);
int recibir(ColaIPC*, long);
int pregunta(const char*);

#endif