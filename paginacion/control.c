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
	inicializarPaginacion(&paginacion, 1000, 20);
	
	printf("\n");
	agregarSolicitud(&paginacion, 320, "aftab");
	agregarSolicitud(&paginacion, 230, "hussain");
	agregarSolicitud(&paginacion, 150, "root");
	agregarSolicitud(&paginacion, 570, "foo");
	agregarSolicitud(&paginacion, 490, "bar");
	imprimeTablaSolicitudes(paginacion);

	printf("\n");
	cargarSolicitud(&paginacion);
	imprimeTablaProcesos(paginacion);

	printf("\n");
	imprimeTablaMemorias(paginacion);

	return 0;
}
