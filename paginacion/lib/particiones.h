#ifndef PARTICIONES_H
#define PARTICIONES_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define STR_USUARIO 32

typedef struct {
	int tam;
	char usuario[STR_USUARIO];
} Solicitud;

typedef struct {
	int tscont;
	int estado;
} Pagina;

typedef struct {
	int pid;
	int tam;
	int xpag;
	int npag;
	char usuario[STR_USUARIO];
	Pagina* paginas;
} Proceso;

typedef struct {
	long tipo;
	int accion;
	union {
		Solicitud solicitud;
		Proceso proceso;
	} dato;
} NodoIPC;

typedef struct {
	key_t llave;
	int cola;
	NodoIPC nodo;
} ColaIPC;


#define IPC_LONGITUD (sizeof(NodoIPC) - sizeof(long))
#define FLLAVE "/bin/ls"
#define CLAVE 33

/* Tipos de Nodos, especifican la direccion de la comunicacion */
#define CONTROL_SOLICITUD 1
#define SOLICITUD_CONTROL 2
#define CONTROL_PROCESO   3
#define PROCESO_CONTROL   4

/* Estados para los nodos */
#define AGREGAR 1
#define SACAR   2
#define VACIA   3
#define FRACASO 4
#define EXITO   5
#define SALIR   6

/* Estados para los procesos */
#define LISTO     1
#define ESPERA    2
#define EJECUCION 3
#define PARADA    4
#define FIN       5

int inicializar(ColaIPC*);
int enviar(ColaIPC*, long, int);
int recibir(ColaIPC*, long);

Solicitud* crearSolicitud(int, const char*);
Proceso* crearProceso(int, int, int, const char*);

#endif