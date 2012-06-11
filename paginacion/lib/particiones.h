#ifndef _PARTICIONES_H
#define _PARTICIONES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../../util/list.h"

#define MAXSTR    16

/* Estados */
#define LIBRE     0
#define LISTO     1
#define ESPERA    2
#define EJECUCION 3
#define PARADA    4
#define FIN       5
#define BLOQUEADO 6

/* Manejo de errores */
#define NO_SOLICITUDES    1
#define NO_MEMORIA        2
#define NO_MALLOC         4
#define NO_PROCESOS       8
#define PAGINA_FUERA     16
#define TSCONT_INVALIDO  32
#define CRITICO          64
#define ES_BLOQUEADO    128

/* Tipos de Memorias */
#define MEM_FISICA  1
#define MEM_VIRTUAL 2

typedef struct {
	int tam;
	char usuario[MAXSTR];
} Solicitud;

struct Marco;

typedef struct {
	struct Marco* marco;
	int tscont;
} Pagina;

typedef struct {
	int pid;
	int tam;
	int xpag;
	int npag;
	char usuario[MAXSTR];
	Pagina* paginas;
	int zcinicio;
	int zcfinal;
} Proceso;

typedef struct Marco {
	int estado;
	Proceso* proceso;
	int pagina;
	int tipo;
	int indice;
} Marco;

typedef struct {
	int tam;
	Marco* marcos;
} Tabla;

typedef struct {
	Marco* ant;
	Marco* actual;
} Meta;

typedef struct {
	List solicitudes;
	List procesos;
	Tabla* memfisica;
	Tabla* memvirtual;
	int cpids;
	int tampag;
	IteratorList actual;
	Proceso* zonacritica;
	Meta meta;
} Paginacion;

Solicitud* crearSolicitud(int, const char*);
Proceso* crearProceso(int, int, int, const char*);
Tabla* crearTabla(int, int);
int inicializarPaginacion(Paginacion*, int, int);
int estaVaciaSolicitudes(Paginacion);
int agregarSolicitud(Paginacion*, int, const char*);
int paginasLibresMemFisica(Paginacion);
int paginasLibresMemVirtual(Paginacion);
int paginasLibres(Paginacion);
int cargarSolicitud(Paginacion*, int*);
void imprimeTablaSolicitudes(Paginacion);
void imprimeTablaProcesos(Paginacion);
void imprimeTablaMemorias(Paginacion);
int quantum(Paginacion*, int*);
int estaZonaCritica(Proceso);
int tienePrestamo(Paginacion, Proceso);
int estaBloqueado(Paginacion, Proceso);

#endif