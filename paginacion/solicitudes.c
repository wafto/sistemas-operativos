/* peticion.c
 * Programa que ejemplifica la parte de peticiones para la paginacion en
 * relacion a la admon de memoria, utiliza cola de mensajes por lo que los
 * demas programas en la carpeta deben ser ejecutados tambien.
 * 
 * Autor: Aftab Hussain Miranda
 * Fecha de Elaboracion: 7 de Mayo del 2012.
 *  
 * Repositorio: https://github.com/wafto/sistemas-operativos
 * Compilacion: gcc peticion.c ../util/list.c  lib/particiones.c -o peticion
 * Ejecucion: ./peticion
 */
#include <stdlib.h>
#include <stdio.h>
#include "lib/particiones.h"
#include "../util/list.h"

int main() {

	ColaIPC cola;
	List solicitudes;
	Solicitud* solicitud;
	IteratorList iter;
	int num, opcion;

	if (!inicializar(&cola)) {
		perror("Error al obtener el identificador de la cola de mensajes.");
		exit(-1);
	}
	initlist(&solicitudes);

	do {
		opcion = 1;
		system("clear");
		if (isemptylist(solicitudes)) {
			printf("Cola de Solicitudes vacia.\n");
		} else {
			printf("Num \tTam \tUsuario \n");
			printf("---------------------------------\n");
			for (iter = beginlist(solicitudes), num = 0; iter != NULL; iter = nextlist(iter)) {
				solicitud = (Solicitud*) dataiterlist(iter);
				printf("%d \t%d \t%s \n", ++num, solicitud->tam, solicitud->usuario);
			}
		}
		recibir(&cola, CONTROL_SOLICITUD);
		switch (cola.nodo.accion) {
			case AGREGAR:
				solicitud = crearSolicitud(cola.nodo.dato.solicitud.tam, cola.nodo.dato.solicitud.usuario);
				pushbacklist(&solicitudes, solicitud);
				enviar(&cola, SOLICITUD_CONTROL, EXITO);
				break;
			case SACAR:
				if (!isemptylist(solicitudes)) {
					solicitud = (Solicitud*) popfrontlist(&solicitudes);
					cola.nodo.dato.solicitud.tam = solicitud->tam;
					strcpy(cola.nodo.dato.solicitud.usuario, solicitud->usuario);
					free(solicitud);
					enviar(&cola, SOLICITUD_CONTROL, EXITO);
				} else
					enviar(&cola, SOLICITUD_CONTROL, VACIA);
				break;
			case SALIR:
				opcion = 0;
				break;
			default:
				enviar(&cola, SOLICITUD_CONTROL, FRACASO);
				break;
		}

	} while(opcion != 0);

	/* responder a la terminacion */
	enviar(&cola, SOLICITUD_CONTROL, EXITO);
	emptylist(&solicitudes);

	return 0;
}
