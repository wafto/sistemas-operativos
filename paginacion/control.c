/* control.c
 * Programa que controla los demas procesos para la paginacion en
 * relacion a la admon de memoria, utiliza cola de mensajes por lo que los
 * demas programas en la carpeta deben ser ejecutados tambien.
 * 
 * Autor: Aftab Hussain Miranda
 * Fecha de Elaboracion: 7 de Mayo del 2012.
 *  
 * Repositorio: https://github.com/wafto/sistemas-operativos
 * Compilacion: gcc control.c ../util/list.c  lib/particiones.c -o control
 * Ejecucion: ./control
 */
#include <stdlib.h>
#include <stdio.h>
#include "lib/particiones.h"
#include "../util/list.h"

void linea();
int pregunta(const char*);

int main() {

	ColaIPC cola;
	int opcion, invalida, pagtam, memtam = 0;

/*
	if (!inicializar(&cola)) {
		perror("Error al obtener el identificador de la cola de mensajes.");
		exit(-1);
	}
*/

	system("clear");
	do {
		linea();
		printf("Cuanta memoria quiere: ");
		scanf("%d", &memtam);
		invalida = (memtam <= 0) ? 1 : 0;
		if (invalida)
			printf("Tamaño de memoria invalida.\n");
	} while (invalida);

	do {
		linea();
		printf("Tamaño del marco: ");
		scanf("%d", &pagtam);
		invalida = (pagtam <= 0 || pagtam > memtam || memtam % pagtam != 0) ? 1 : 0;
		if (invalida) {
			printf("Tamaño de marco invalido.\n");
			printf("Debe ser multiplo del tamaño de memoria.\n");
		}
	} while (invalida);

	return 0;

	do {
		linea();
		printf("Opciones disponibles:\n");
		linea();
		printf("0 - Salir.\n");
		printf("1 - Agregar solicitud.\n");
		printf("2 - Cargar solicitud.\n");
		printf("Opcion: ");
		scanf("%d", &opcion);

		switch (opcion) {
			case 0:
				printf("Adios.\n");
				break;
			case 1:
				do {
					printf("Se procede a agregar una nueva solicitud\n");
					printf(" - Tamaño: ");
					scanf("%d", &cola.nodo.dato.solicitud.tam);
					printf(" - Usuario: ");
					scanf("%s", &(cola.nodo.dato.solicitud.usuario)[0]);
					enviar(&cola, CONTROL_SOLICITUD, AGREGAR);
					recibir(&cola, SOLICITUD_CONTROL);
					linea();
					if (cola.nodo.accion == EXITO)
						printf("Se agrego con éxito la solicitud.\n");
					else
						printf("Hubo un problema al agregar la solicitud.\n");
				} while (pregunta("Desea agregar otra solicitud?"));
				break;
			case 2:
				printf("Se procede a sacar de la cola de solicitudes\n");
				enviar(&cola, CONTROL_SOLICITUD, SACAR);
				recibir(&cola, SOLICITUD_CONTROL);
				if (cola.nodo.accion == EXITO) {
					printf("Tamaño: %d\n", cola.nodo.dato.solicitud.tam);
					printf("Usuario: %s\n", cola.nodo.dato.solicitud.usuario);
				}
				if (cola.nodo.accion == VACIA) {
					printf("La cola de solicitudes esta vacia.\n");
				}
				break;
			default:
				printf("Opcion invalida.\n");
				break;
		}


		
	} while(opcion != 0);

	/* Terminacion de solicitudes */
	enviar(&cola, CONTROL_SOLICITUD, SALIR);
	recibir(&cola, SOLICITUD_CONTROL);
	msgctl(cola.llave, IPC_RMID, NULL);

	return 0;
}

void linea() {
	int i;
	for (i = 0; i < 50; i++) printf("-");
	printf("\n");
}

int pregunta(const char* pregunta) {
	char respuesta;
	printf("%s Si(s)/No(*): ", pregunta);
	fflush(stdin);
	scanf("%s", &respuesta);
	return respuesta == 's' ? 1 : 0;
}