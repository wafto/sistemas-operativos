/**
 * inventario.c
 * Programa que ejemplifica el uso de una lista, para ello se creo una lista
 * doblemente ligada de proposito general.
 * 
 * Autor: Aftab Hussain Miranda
 * Fecha de Elaboracion: 1ro de Marzo del 2012.
 *  
 * Repositorio: https://github.com/wafto/sistemas-operativos
 * Compilacion: gcc -o programa ../util/list.c inventario.c
 * Ejecucion: ./programa
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../util/list.h"

#define LINEA   100
#define NOMTAM  30 

typedef struct {
	char nombre[NOMTAM];
	int cantidad;
} Item;

int menu();
void limpiar();
void linea();
int pregunta(const char*);
Item* crearitem(const char*, int);
IteratorList busquedapornombre(List, const char*, int*);

int main() {

	List inventario;
	Item* item;
	int i, bandera;
	char nombre[NOMTAM];
	int cantidad;
	IteratorList iter;

	initlist(&inventario);

	while (1) {
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
				if (isemptylist(inventario)) {
					printf("Inventario ya esta inicializado.\n");
				} else {
					if (pregunta("El inventario ya tiene items. Si acepta se eliminaran.")) {
						emptylist(&inventario);
						printf("Inventario vacio y listo para agregar nuevos items.\n");
					} else {
						printf("Accion de inicializar cancelada.\n");
					}
				}
				break;
			case 2:
				do {
					printf("Agregar un nuevo item.\n");
					printf(" - nombre: ");
					scanf("%s", &nombre[0]);
					printf(" - cantidad: ");
					scanf("%d", &cantidad);
					item = crearitem(nombre, cantidad);
					if (item != NULL) {
						pushbacklist(&inventario, item);
					} else {
						printf("Error al agragar item, sin memoria para dicha accion.\n");
					}
				} while (pregunta("Desea agregar otro item?"));
				break;
			case 3:
				printf("Eliminacion de item por nombre, se eliminara la primera que se encuentre.\n");
				printf("Nombre: ");
				scanf("%s", &nombre[0]);
				iter = busquedapornombre(inventario, nombre, &bandera);
				if (bandera) {
					printf("Se encontro el item con el nombre %s.\n", nombre);
					item = (Item*) dataiterlist(iter);
					printf(" - Nombre: %s\n", item->nombre);
					printf(" - Cantidad: %d\n", item->cantidad);
					if (pregunta("Desea eliminar dicho item?")) {
						printf("Se procede a eliminar el item con el nombre %s\n", item->nombre);
						free(popiterlist(&inventario, iter));
						printf("Se elimino el item.\n");
					} else {
						printf("Item no eliminado.\n");
					}
				} else {
					printf("No se encontro el item especificado.\n");
				}
				break;
			case 4:
				printf("Listado de items en el inventario. items(%ld)\n", sizelist(inventario));
				if (!isemptylist(inventario)) {
					i = 0;
					linea();
					printf("Num \tNombre \tCantidad\n");
					for (iter = beginlist(inventario); iter != NULL; iter = nextlist(iter)) {
						item = (Item*) dataiterlist(iter);
						printf("%d\t%s\t%d\n", ++i, item->nombre, item->cantidad);
					}
				} else {
					printf("El inventario se encuentra vacio.\n");
				}
				break;
			case 5:
				printf("Busqueda de item por nombre, solo se muestra la primera que se encuentra.\n");
				printf("Nombre: ");
				scanf("%s", &nombre[0]);
				iter = busquedapornombre(inventario, nombre, &bandera);
				if (bandera) {
					printf("Se encontro el item con el nombre %s.\n", nombre);
					item = (Item*) dataiterlist(iter);
					printf(" - Nombre: %s\n", item->nombre);
					printf(" - Cantidad: %d\n", item->cantidad);
				} else {
					printf("No se encontro el item especificado.\n");
				}
				break;
			case 6:
				printf("Edicion de item por nombre, se editara la primera que se encuentre.\n");
				printf("Nombre: ");
				scanf("%s", &nombre[0]);
				iter = busquedapornombre(inventario, nombre, &bandera);
				if (bandera) {
					printf("Se encontro el item con el nombre %s.\n", nombre);
					item = (Item*) dataiterlist(iter);
					printf(" - Nombre: %s\n", item->nombre);
					printf(" - Cantidad: %d\n", item->cantidad);
					if (pregunta("Desea editar dicho item?")) {
						printf("Se procede a editar el item con el nombre %s\n", item->nombre);
						printf(" - nombre: ");
						scanf("%s", &nombre[0]);
						printf(" - cantidad: ");
						scanf("%d", &cantidad);
						strcpy(item->nombre, nombre);
						item->cantidad = cantidad;
						printf("Se edito el item.\n");
					} else {
						printf("Item no editado.\n");
					}
				} else {
					printf("No se encontro el item especificado.\n");
				}
				break;
			default:
				break;
		}
	}
}

int menu() {
	int opt;
	linea();
	printf(" 1 - Iniciaizar el inventario.\n");
	printf(" 2 - Agregar item(s) al inventario.\n");
	printf(" 3 - Eliminar item del inventario.\n");
	printf(" 4 - Mostrar items del inventario.\n");
	printf(" 5 - Buscar item del inventario.\n");
	printf(" 6 - Editar item del inventario.\n");
	printf(" 0 - Salir.\n");
	printf("\tOpcion: ");
	scanf("%d", &opt);
	linea();
	return opt;
}

void linea() {
	int i = 0;
	for (; i < LINEA; i++)
		printf("-");
	printf("\n");
}

void limpiar() {
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

Item* crearitem(const char *nombre, int cantidad) {
	Item* item = (Item*) malloc(sizeof(Item));
	if (item != NULL) {
		strcpy(item->nombre, nombre);
		item->cantidad = cantidad;
	}
	return item;
}

IteratorList busquedapornombre(List list, const char* nombre, int* bandera) {
	*bandera = 0;
	IteratorList iter;
	Item* item;
	for (iter = beginlist(list); iter != NULL; iter = nextlist(iter)) {
		item = (Item*) dataiterlist(iter);
		if (item != NULL && strcmp(item->nombre, nombre) == 0) {
			*bandera = 1;
			break;
		}
	}
	return iter;
}



