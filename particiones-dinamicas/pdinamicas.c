/* pdinamicas.c
 * Programa que ejemplifica el funcionamiento de una particion dinamica en relacion a
 * los sistemas operativos.
 * 
 * Autor: Aftab Hussain Miranda
 * Fecha de Elaboracion: 13 de Marzo del 2012.
 *  
 * Repositorio: https://github.com/wafto/sistemas-operativos
 * Compilacion: gcc pdinamicas.c ../util/list.c -o app
 * Ejecucion: ./app
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../util/list.h"

#define STRSIZE 30
#define MEMUNITS 1000
#define LINEA 100
#define LIBRE -1

typedef struct {
	int number;
	int size;
	int address;
	int status;
} Partition;

typedef struct {
	int pid;
	char user[STRSIZE];
	int size;
	Partition* partition;
	char command[STRSIZE];	
} Process;

Process* creaProcess(int, const char*, int, Partition*, const char*);
Partition* creaPartition(int, int, int, int);
void linea();
void limpia();
int pregunta(const char*);
Partition* bestFitPara(Process*, List, int*);
int agregarProceso(List*, Partition*, Process*, int*, int*);
void compactacionContigua(List*);

int menu () {
	int opt;
	linea();
	printf(" 1 - Capturar Solicitud.\n");
	printf(" 2 - Cargar Proceso.\n");
	printf(" 3 - Terminar Proceso.\n");
	printf(" 4 - Mostrar Tabla de Procesos.\n");
	printf(" 5 - Mostrar Tabla de Particiones.\n");
	printf(" 0 - Salir.\n");
	printf("\tOpcion: ");
	scanf("%d", &opt);
	linea();
	return opt;
}

int main(int argc, char const *argv[]) {
	List processList, partitionList;
	int pidCounter = 0;
	int partitionCounter = 0;
	Process* process;
	Partition* partition;
	char user[STRSIZE], command[STRSIZE];
	int tam, correcto, bandera, stat;
	IteratorList iter;

	initlist(&processList);
	initlist(&partitionList);

	/* Particion inicial */
	partition = creaPartition(partitionCounter++, MEMUNITS, 0, LIBRE);
	pushbacklist(&partitionList, partition);

	while(1) {
		switch (menu()) {
			case 0:
				if (pregunta("Desea salir del programa?")) {
					printf("Adios\n");
					return 0;
				} else {
					printf("Accion de salir cancelada.\n");
				}
				break;
			case 1:
				do {
					printf("Usuario para la solicitud: ");
					scanf("%s", &user[0]);
					printf("Comando para la solicitud: ");
					scanf("%s", &command[0]);
					do {
						printf("Tamaño para la solicitud: ");
						scanf("%d", &tam);
						correcto = (tam > 0 && tam <= MEMUNITS) ? 1 : 0;
						if (!correcto)
							printf("Tamaño de solicitud no valido, vuelva a intentar.\n");
					} while(!correcto);
					process = creaProcess(LIBRE, user, tam, NULL, command);
					if (process != NULL) {
						pushbacklist(&processList, process);
					} else {
						printf("Error al crear solicitud.\n");
					}
					linea();
				} while(pregunta("Desea Capturar otra solicitud?"));
				break;
			case 2:
				do {
					stat = correcto = 0;
					for (iter = beginlist(processList); iter != NULL; iter = nextlist(iter)) {
						process = (Process*) dataiterlist(iter);
						if (process->pid == LIBRE && process->partition == NULL) {
							printf("Se procede a cargar la solicitud del usuario %s con tamaño %d.\n", process->user, process->size);
							partition = bestFitPara(process, partitionList, &bandera);
							if (bandera) {
								agregarProceso(&partitionList, partition, process, &pidCounter, &partitionCounter);
								correcto = 1;
							} else {
								printf("Insuficiente memoria para la solicitud, se vuelve a encolar.\n");
								pushbacklist(&processList, popiterlist(&processList, iter));
								stat = 1;
							}
							break;
						}
					}
					if (correcto) {
						printf("Se cargo el proceso con el pid %d.\n", process->pid);
					} else {
						printf("No se pudo cargar un proceso.\n");
						if (stat == 0)
							printf("Debido a que no hay solicitudes pendientes.\n");
						break;
					}
					linea();
				} while(pregunta("Desea Cargar otro proceso?"));
				break;
			case 3:
				printf("Ingrese el PID del proceso que desea eliminar: ");
				scanf("%d", &bandera);
				correcto = 0;
				for (iter = beginlist(processList); iter != NULL; iter = nextlist(iter)) {
					process = (Process*) dataiterlist(iter);
					if (process->pid != LIBRE && process->partition != NULL && process->pid == bandera) {
						popiterlist(&processList, iter);
						process->partition->status = LIBRE;
						compactacionContigua(&partitionList);
						free(process);
						correcto = 1;
						break;
					}
				}
				if (correcto == 1)
					printf("Se elimino el proceso inicado.\n");
				else
					printf("No se pudo encontrar el proceso indicado para eliminarlo.\n");
				break;
			case 4:
				if (!isemptylist(processList)) {
					printf("pid \tuser \tsize \tpart \taddr \tcommand \n");
					linea();
					for (iter = beginlist(processList); iter != NULL; iter = nextlist(iter)) {
						process = (Process*) dataiterlist(iter);
						if (process->pid != LIBRE && process->partition != NULL) {
							printf("%d \t%s \t%d \t%d \t%d \t%s \n",
								process->pid,
								process->user,
								process->size,
								process->partition->number,
								process->partition->address,
								process->command);
						}
					}
				}
				break;
			case 5:
				if (!isemptylist(partitionList)) {
					printf("num \tsize \taddr \tstatus \n");
					linea();
					for (iter = beginlist(partitionList); iter != NULL; iter = nextlist(iter)) {
						partition = (Partition*) dataiterlist(iter);
						printf("%d \t%d \t%d \t%d \n", 
							partition->number,
							partition->size,
							partition->address,
							partition->status);
					}
				}
				break;
			default:
				printf("\tOpcion invalida.\n");
		}
	}
}

Process* creaProcess(int pid, const char* user, int size, Partition* partition, const char* command) {
	Process* process = (Process*) malloc(sizeof(Process));
	if (process != NULL) {
		process->pid = pid;
		strcpy(process->user, user);
		process->size = size;
		process->partition = partition;
		strcpy(process->command, command);
	}
	return process;
}

Partition* creaPartition(int number, int size, int address, int status) {
	Partition* partition = (Partition*) malloc(sizeof(Partition));
	if (partition != NULL) {
		partition->number = number;
		partition->size = size;
		partition->address = address;
		partition->status = status;
	}
	return partition;
}

void linea() {
	int i = 0;
	for (; i < LINEA; i++)
		printf("-");
	printf("\n");
}

void limpia() {
	#ifdef WINDOWS
		system("cls");
	#else
		system("clear");
	#endif
}

int pregunta(const char* pregunta) {
	char respuesta;
	printf("%s [s] para si y cualquier otra letra para no: ", pregunta);
	fflush(stdin);
	scanf("%s", &respuesta);
	return respuesta == 's' ? 1 : 0;
}

Partition* bestFitPara(Process* process, List partList, int* flag) {
	Partition *aux, *best = NULL;
	IteratorList iter;
	*flag = 0;
	if (!isemptylist(partList) && process != NULL) {
		for (iter = beginlist(partList); iter != NULL; iter = nextlist(iter)) {
			aux = (Partition*) dataiterlist(iter);
			if (aux != NULL && aux->status == LIBRE && aux->size >= process->size) {
				if (best == NULL || aux->size - process->size < best->size - process->size) {
					best = aux;
					*flag = 1;
				}
			}
		}
	}
	return best;
}

int agregarProceso(List* lista, Partition* particion, Process* proceso, int* cproc, int* cpart) {
	IteratorList iter;
	Partition *aux, *nuevo;
	unsigned long index = 0;
	int tam, addr;
	if (!isemptylist(*lista) && particion != NULL && proceso != NULL && proceso->size <= particion->size) {
		if (proceso->size != particion->size) {
			for (iter = beginlist(*lista); iter != NULL; iter = nextlist(iter)) {
				index += 1;
				aux = (Partition*) dataiterlist(iter);
				if (aux == particion) {
					tam = particion->size - proceso->size;
					addr = particion->address + proceso->size;
					nuevo = creaPartition((*cpart)++, tam, addr, LIBRE);
					pushatlist(lista, index, nuevo);
					particion->size -= tam; 
					break;
				}
			}
		}
		proceso->pid = ++(*cproc);
		proceso->partition = particion;
		proceso->partition->status = proceso->pid;
		return 1;
	}
	return 0;
}

void compactacionContigua(List* lista) {
	IteratorList current, next;
	Partition *particion, *siguiente;
	int bandera;
	do {
		bandera = 0;
		if (sizelist(*lista) > 1) {
			for (current = beginlist(*lista); current != NULL && nextlist(current) != NULL; current = nextlist(current)) {
				next = nextlist(current);
				particion = (Partition*) dataiterlist(current);
				siguiente = (Partition*) dataiterlist(next);
				if (particion->status == LIBRE && siguiente->status == LIBRE) {
					siguiente = (Partition*) popiterlist(lista, next);
					particion->size += siguiente->size;
					free(siguiente);
					bandera = 1;
					break;
				}
			}
		}
	} while(bandera);
}

