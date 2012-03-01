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

typedef struct {
	char nombre[10];
	int cantidad;
} Item;

int menu() {
	int opt;
	printf("---------------------------------------------------------\n");
	printf(" 1 - Capturar Solicitud.\n");
	printf(" 2 - Cargar Proceso.\n");
	printf(" 3 - Terminar Proceso.\n");
	printf(" 4 - Mostrar Estado.\n");
	printf(" 5 - Salir.\n");
	printf("\tOpcion: ");
	scanf("%d", &opt);
	printf("---------------------------------------------------------\n");
	return opt;
}

Item* crearitem(char nombre[10], int cantidad) {
	Item* item = (Item*) malloc(sizeof(Item));
	if (item != NULL) {
		strcpy(item->nombre, nombre);
		item->cantidad = cantidad;
	}
	return item;
}

int main() {

	List inventario;
	Item* item;

	initlist(&inventario);



}

