/* procesos.c
 * Se encarga de adminsitrar los "procesos".
 * 
 * Autor: Aftab Hussain Miranda
 * Fecha de Elaboracion: 17 de Mayo del 2012.
 *  
 * Repositorio: https://github.com/wafto/sistemas-operativos
 * gcc procesos.c ../util/list.c  lib/particiones.c -o ~/Desktop/procesos
 * Ejecucion: ./procesos
 */
#include <stdlib.h>
#include <stdio.h>
#include "lib/particiones.h"
#include "../util/list.h"

int main() {
/* 	ColaIPC cola;
	List procesos;

	IteratorList iter;
	int num, opcion;

	if (!inicializar(&cola)) {
		perror("Error al obtener el identificador de la cola de mensajes.");
		exit(-1);
	}
	initlist(&solicitudes);
*/
	int i;
	Proceso* proceso = crearProceso(0, 101, 20, "aftab");


	printf("pid: %d, tam: %d, xpag: %d, npag: %d, usuario: %s\n", 
		proceso->pid,
		proceso->tam,
		proceso->xpag,
		proceso->npag,
		proceso->usuario
	);

	for (i = 0; i < proceso->npag; i++)
		printf("pag[%d] : %d\n", i, proceso->paginas[i].tscont);

 	return 0;
}