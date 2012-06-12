/* control.c
 * 
 * 
 * Autor: Aftab Hussain Miranda
 * Fecha de Elaboracion: 24 de Mayo del 2012.
 *  
 * Repositorio: https://github.com/wafto/sistemas-operativos
 * Compilacion: 
 * Ejecucion:
 */
#include "lib/particiones.h"
#include "lib/comun.h"

int main() {
	ColaIPC cola;
	int opcion;

	if (!inicializar(&cola)) {
		perror("Error al obtener el identificador de la cola de mensajes.");
		exit(-1);
	}
	system("clear");
	do {
		linea();
		printf("Opciones disponibles:\n");
		linea();
		printf("0 - Salir.\n");
		printf("1 - Agregar solicitud.\n");
		printf("Opcion: ");
		scanf("%d", &opcion);
		switch (opcion) {
			case 0:
				printf("Adios.\n");
				enviar(&cola, CONTROL_MEMORIA, SALIR);
				break;
			case 1:
				do {
					linea();
					printf("Se procede a agregar una nueva solicitud\n");
					printf(" - Tamaño en memoria: ");
					scanf("%d", &cola.nodo.dato.tam);
					printf(" - Usuario: ");
					scanf("%s", &(cola.nodo.dato.usuario)[0]);
					enviar(&cola, CONTROL_MEMORIA, AGREGAR);
				} while (pregunta("Desea agregar otra solicitud?"));
				break;
			default:
				printf("Opcion invalida.\n");
				break;
		}
	} while (opcion != 0);

	return 0;
}
