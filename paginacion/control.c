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

int main() {

	Paginacion paginacion;
	int err, i;
	struct timespec tim, tim2;
	int tiempo = 1;
   	tim.tv_sec = 0;
   	tim.tv_nsec = tiempo * 100000000;

	inicializarPaginacion(&paginacion, 1000, 20);
	
	printf("\n");
	agregarSolicitud(&paginacion, 120, "aftab");
	agregarSolicitud(&paginacion, 230, "hussain");
	agregarSolicitud(&paginacion, 250, "saass");
	agregarSolicitud(&paginacion, 570, "foo");
	agregarSolicitud(&paginacion, 490, "bar");
	agregarSolicitud(&paginacion, 440, "tazuu");
	imprimeTablaSolicitudes(paginacion);

	printf("\n");
	cargarSolicitud(&paginacion, &err);
	cargarSolicitud(&paginacion, &err);
	cargarSolicitud(&paginacion, &err);
	cargarSolicitud(&paginacion, &err);
	cargarSolicitud(&paginacion, &err);
	cargarSolicitud(&paginacion, &err);
	imprimeTablaProcesos(paginacion);

	for (i = 0; i < 450; i++) {
		system("clear");
		imprimeTablaMemorias(paginacion);
		quantum(&paginacion, &err);
		printf("%d\n", i);
		if (err == NO_PROCESOS) break;
		if ( i > 90 && i < 100)
			sleep(3);
		nanosleep(&tim , &tim2);
	}
	
	
if ( i > 100 && i < 150)
	sleep(3);
	return 0;
}
