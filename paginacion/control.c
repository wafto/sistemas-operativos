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
	inicializarPaginacion(&paginacion, 1000, 80);
	

	printf("\nTabla de Solicitudes\n");
	agregarSolicitud(&paginacion, 320, "aftab");
	agregarSolicitud(&paginacion, 230, "hussain");
	agregarSolicitud(&paginacion, 150, "root");
	agregarSolicitud(&paginacion, 570, "foo");
	agregarSolicitud(&paginacion, 490, "bar");
	imprimeTablaSolicitudes(paginacion);

	printf("\nTabla de Procesos\n");
	cargarSolicitud(&paginacion);
	imprimeTablaProcesos(paginacion);

	printf("\nMemorias\n");
	imprimeTablaMemorias(paginacion);

	return 0;
}
