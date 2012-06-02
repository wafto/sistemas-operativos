/* control.c
 * 
 * 
 * Autor: Aftab Hussain Miranda
 * Fecha de Elaboracion: 2 de Junio del 2012.
 *  
 * Repositorio: https://github.com/wafto/sistemas-operativos
 * Compilacion: 
 * Ejecucion:
 */
#include "lib/particiones.h"
#include "lib/comun.h"

#define SI 1
#define NO 0

int main() {

	Paginacion paginacion;
	int err, i, j, tiempo, salir, imprime, correcto;
	struct timespec tim, tim2;
	tim.tv_sec = 0;
	tim.tv_nsec = 100000000;

	do {
		printf("Cuánta memoria requiere para la paginacion: ");
		scanf("%d", &i);
		correcto = i > 0 ? SI : NO;
		if (!correcto) printf("El tamaño de memoria debe ser mayor a 0.\n");
	} while (!correcto);

	do {
		printf("Especifique el tamaño del marco: ");
		scanf("%d", &j);
		correcto = (j > 0 && i % j == 0)? SI : NO;
		if (!correcto) printf("El tamaño de marco debe ser mayor a 0 y multiplo de %d.\n", i);
	} while (!correcto);

	do {
		printf("Especifique la duracion de quanta en ms. (valor entre 1 a 10): ");
		scanf("%d", &tiempo);
		correcto = (tiempo >= 1 && tiempo <= 10)? SI : NO;
		if (!correcto) printf("La cantidad de quanta invalido.\n");
	} while (!correcto);

	inicializarPaginacion(&paginacion, i, j);
	tim.tv_nsec = 100000000 * tiempo;
	salir = NO;
	imprime = SI;

	agregarSolicitud(&paginacion, 60, "aftab");
	agregarSolicitud(&paginacion, 130, "hussain");
	agregarSolicitud(&paginacion, 120, "saass");
	agregarSolicitud(&paginacion, 270, "foo");
	agregarSolicitud(&paginacion, 140, "bar");
	agregarSolicitud(&paginacion, 240, "tazuu");
	agregarSolicitud(&paginacion, 120, "batfa");
	agregarSolicitud(&paginacion, 130, "miranda");
	agregarSolicitud(&paginacion, 150, "flemx");
	agregarSolicitud(&paginacion, 170, "lorem");
	agregarSolicitud(&paginacion, 190, "ipsum");
	agregarSolicitud(&paginacion, 220, "lol");
	
	do {
		/* Carga automatica de solicitudes */
		if (!estaVaciaSolicitudes(paginacion)) {
			i = sizelist(paginacion.solicitudes);
			for (j = 0; j < i; j++) cargarSolicitud(&paginacion, &err);
		}
		/* Ejecucion de quantum */
		system("clear");
		imprimeTablaMemorias(paginacion);
		quantum(&paginacion, &err);
		nanosleep(&tim , &tim2);
	} while (!salir);

	return 0;
}