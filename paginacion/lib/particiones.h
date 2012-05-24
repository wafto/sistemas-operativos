#ifndef _PARTICIONES_H
#define _PARTICIONES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../../util/list.h"

#define MAXSTR    32

/* Estado de los procesos */
#define LISTO     1
#define ESPERA    2
#define EJECUCION 3
#define PARADA    4
#define FIN       5

typedef struct {
	int tam;
	char usuario[MAXSTR];
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
	char usuario[MAXSTR];
	Pagina* paginas;
} Proceso;

typedef struct {
	int estado;
	Proceso* proceso;
	int pagina;
} Marco;

typedef struct {
	Marco* marcos;
} Tabla;

typedef struct {
	List solicitudes;
	List procesos;
	Tabla* memfisica;
	Tabla* memvirtual;
	int cpids;
	int tampag;
} Paginacion;

Solicitud* crearSolicitud(int, const char*);
Proceso* crearProceso(int, int, int, const char*);
Tabla* crearTabla(int);
int inicializarPaginacion(Paginacion*, int, int);
int estaVaciaSolicitudes(Paginacion);
int agregarSolicitud(Paginacion*, int, const char*);
int cargarSolicitud(Paginacion*);

#endif