/* pdinamicasv2.c
 * Programa que ejemplifica el funcionamiento de una particion dinamica en relacion a
 * los sistemas operativos. (segunda version)
 * 
 * Autor: Aftab Hussain Miranda
 * Fecha de Elaboracion: 23 de Marzo del 2012.
 *  
 * Repositorio: https://github.com/wafto/sistemas-operativos
 * Compilacion: gcc pdinamicasv2.c ../util/list.c -o app
 * Ejecucion: ./app
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../util/list.h"

#define STRTAM 30
#define MEMUNIDADES 1000
#define LINEA 100

/* Estructuras */
typedef struct {
	int pid;
	char usuario[STRTAM];
	char comando[STRTAM];
	int tam;	
} Proceso;

typedef struct {
	int tam;
	int direccion;
	Proceso* proceso;
} Particion;

/* Prototipos de las funciones */
int menu();
void linea();
int pregunta(const char*);
Proceso* crearProceso(int, const char*, const char*, int);
Particion* crearParticion(int, int, Proceso*);
int agregarPorBestFit(List*, Proceso*);
int eliminarProceso(List*, int);
void imprimeTablaParticiones(List*);
void compactacionContigua(List*);
void compactacionCompleta(List*);
int memoriaLibre(List);

/* Principal */
int main() {
	List solicitudes, particiones;
	int contadorPid = 0;
	Proceso* proceso;
	char usuario[STRTAM], comando[STRTAM];
	int numaux, bandera, correcto;

	initlist(&solicitudes);
	initlist(&particiones);

	pushbacklist(&particiones, crearParticion(MEMUNIDADES, 0, NULL));

	while (1) {
		switch(menu()) {
			case 0:
				if (pregunta("Desea salir del programa?")) {
					printf("Adios.\n");
					return 0;
				} else {
					printf("Accion de salir cancelada.\n");
				}
				break;
			case 1:
				do {
					printf("Usuario para la solicitud: ");
					scanf("%s", &usuario[0]);
					printf("Comando para la solicitud: ");
					scanf("%s", &comando[0]);
					do {
						printf("Tamaño para la solicitud: ");
						scanf("%d", &numaux);
						correcto = (numaux > 0 && numaux <= MEMUNIDADES) ? 1 : 0;
						if (!correcto)
							printf("Tamaño de solicitud no valido, vuelva a intentar.\n");
					} while (!correcto);
					pushbacklist(&solicitudes, crearProceso(-1, usuario, comando, numaux));
					printf("Se agrego una nueva solicitud en la cola.\n");
					linea();
				} while (pregunta("Desea agregar otra solicitud?"));
				break;
			case 2:
				do {
					if (!isemptylist(solicitudes)) {
						proceso = (Proceso*) popfrontlist(&solicitudes);
						bandera = agregarPorBestFit(&particiones, proceso);
						if (!bandera && memoriaLibre(particiones) >= proceso->tam) {
							compactacionCompleta(&particiones);
							bandera = agregarPorBestFit(&particiones, proceso);
						}
						if (!bandera) {
							printf("No se tienen particiones libres para tal tamaño.\n");
							pushbacklist(&solicitudes, proceso);
							printf("La solicitud se vuelve a encolar.\n");
							break;
						} else {
							proceso->pid = contadorPid++;
							printf("Se agrego exitosamente con el usuario %s y PID %d.\n", proceso->usuario, proceso->pid);
						}
					} else {
						printf("No se tienen solicitudes pendientes.\n");
						break;
					}
					linea();
				} while (pregunta("Desea cargar otro proceso?"));
				break;
			case 3:
				printf("Ingrese el PID del proceso que desea eliminar: ");
				scanf("%d", &numaux);
				if (eliminarProceso(&particiones, numaux)) {
					compactacionContigua(&particiones);
					printf("Se elimino con exito el proceso.\n");
				} else {
					printf("No se encontro el PID del proceso para eliminar.\n");
				}
				break;
			case 4:
				imprimeTablaParticiones(&particiones);
				break;
			default:
				printf("\tOpcion invalida.\n");
				break;
		}
	}

}

/* Implementacion de las funciones */
int menu() {
	int opt;
	linea();
	printf(" 1 - Capturar Solicitud.\n");
	printf(" 2 - Cargar Proceso.\n");
	printf(" 3 - Terminar Proceso.\n");
	printf(" 4 - Mostrar Tabla.\n");
	printf(" 0 - Salir.\n");
	printf("\tOpcion: ");
	scanf("%d", &opt);
	linea();
	return opt;
}

void linea() {
	int i = 0;
	for (; i < LINEA; i++) printf("-");
	printf("\n");
}

int pregunta(const char* pregunta) {
	char respuesta;
	printf("%s [s] para si y cualquier otra letra para no: ", pregunta);
	fflush(stdin);
	scanf("%s", &respuesta);
	return respuesta == 's' ? 1 : 0;
}

Proceso* crearProceso(int pid, const char* usuario, const char* comando, int tam) {
	Proceso* nuevo = (Proceso*) malloc(sizeof(Proceso));
	if (nuevo != NULL) {
		nuevo->pid = pid;
		strcpy(nuevo->usuario, usuario);
		strcpy(nuevo->comando, comando);
		nuevo->tam = tam;
	}
	return nuevo;
}

Particion* crearParticion(int tam, int direccion, Proceso* proceso) {
	Particion* nuevo = (Particion*) malloc(sizeof(Particion));
	if (nuevo != NULL) {
		nuevo->tam = tam;
		nuevo->direccion = direccion;
		nuevo->proceso = proceso;
	}
	return nuevo;
}

int agregarPorBestFit(List* particiones, Proceso* proceso) {
	IteratorList iter, biter = NULL;
	Particion *aux, *best = NULL;
	for (iter = beginlist(*particiones); iter != NULL; iter = nextlist(iter)) {
		aux = (Particion*) dataiterlist(iter);
		if (aux->proceso == NULL && aux->tam >= proceso->tam)
			if (best == NULL || proceso->tam < best->tam) {
				best = aux;
				biter = iter;
			}
	}
	if (best != NULL && biter != NULL) {
		if (best->tam == proceso->tam) {
			best->proceso = proceso;
		} else {
			aux = crearParticion(proceso->tam, best->direccion, proceso);
			best->tam -= proceso->tam;
			best->direccion += proceso->tam;
			pushiterlist(particiones, biter, aux);
		}
		return 1;
	}
	return 0;
}

int eliminarProceso(List* particiones, int pid) {
	Particion* particion;
	IteratorList iter;
	for (iter = beginlist(*particiones); iter != NULL; iter = nextlist(iter)) {
		particion = (Particion*) dataiterlist(iter);
		if (particion->proceso != NULL && particion->proceso->pid == pid) {
			free(particion->proceso);
			particion->proceso = NULL;
			return 1;
		}
	}
	return 0;
}

void imprimeTablaParticiones(List* particiones) {
	int partid = 0;
	Particion* particion;
	IteratorList iter;
	if (!isemptylist(*particiones)) {
		printf("Part. \tTam \tDir \tEstado \n");
		linea();
		for (iter = beginlist(*particiones); iter != NULL; iter = nextlist(iter)) {
			particion = (Particion*) dataiterlist(iter); 
			printf("%d \t%d \t%d ", partid++, particion->tam, particion->direccion);
			if (particion->proceso == NULL) {
				printf("\tLIBRE \n");
		} else {
			printf("\tPID %d (usuario: %s, comando: %s, tamaño: %d)\n",
				particion->proceso->pid,
				particion->proceso->usuario,
				particion->proceso->comando,
				particion->proceso->tam);
			}
		}
	} else {
		printf("Tabla vacia.\n");
	}
}

void compactacionContigua(List* particiones) {
	Particion *actual, *siguiente;
	IteratorList ic, in;
	int continuar;
	do {
		continuar = 0;
		if (sizelist(*particiones) > 1) {
			for (ic = beginlist(*particiones); ic != NULL && nextlist(ic) != NULL; ic = nextlist(ic)) {
				in = nextlist(ic);
				actual = (Particion*) dataiterlist(ic);
				siguiente = (Particion*) dataiterlist(in);
				if (actual->proceso == NULL && siguiente->proceso == NULL) {
					siguiente = (Particion*) popiterlist(particiones, in);
					actual->tam += siguiente->tam;
					free(siguiente);
					continuar = 1;
					break;
				}
			}
		}
	} while (continuar);
}

void compactacionCompleta(List* particiones) {
	Particion* particion;
	IteratorList iter;
	int nuevodir = 0, memoria = 0, continuar;
	do {
		continuar = 0;
		if (sizelist(*particiones) > 1) {
			for (iter = beginlist(*particiones); iter != NULL; iter = nextlist(iter)) {
				particion = (Particion*) dataiterlist(iter);
				if (particion != NULL && particion->proceso == NULL) {
					particion = (Particion*) popiterlist(particiones, iter);
					memoria += particion->tam;
					free(particion);
					continuar = 1;
					break;
				}
			}
		}
	} while (continuar);
	if (memoria > 0) {
		pushbacklist(particiones, crearParticion(memoria, 0, NULL));
		for (iter = beginlist(*particiones); iter != NULL; iter = nextlist(iter)) {
			particion = (Particion*) dataiterlist(iter);
			particion->direccion = nuevodir;
			nuevodir += particion->tam;
		}
	}
}

int memoriaLibre(List particiones) {
	int libre = 0;
	IteratorList iter;
	Particion* particion;
	if (!isemptylist(particiones)) {
		for (iter = beginlist(particiones); iter != NULL; iter = nextlist(iter)) {
			particion = (Particion*) dataiterlist(iter);
			libre += particion->proceso == NULL ? particion->tam : 0;
		}
	}
	return libre;
}



