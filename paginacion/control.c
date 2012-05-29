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
	agregarSolicitud(&paginacion, 320, "aftab");
	agregarSolicitud(&paginacion, 230, "hussain");
	agregarSolicitud(&paginacion, 150, "root");
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
		if (err == NO_PROCESOS) break;
		nanosleep(&tim , &tim2);
	}
	
	

	return 0;
}
