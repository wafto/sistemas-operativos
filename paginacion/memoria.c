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

typedef struct {
	int threadId;
	int salir;
	Paginacion* paginacion;
	ColaIPC* cola;
} ThreadData;

void* escuchaSolicitudes(void*);

int main() {

	Paginacion paginacion;
	ColaIPC cola;
	pthread_t escuchador;
	ThreadData hiloDatos;
	int err, i, j, tiempo, correcto, rc;
	struct timespec tim, tim2;
	tim.tv_sec = 0;
	tim.tv_nsec = 100000000;

	if (!inicializar(&cola)) {
		perror("Error al obtener el identificador de la cola de mensajes.");
		exit(-1);
	}
	system("clear");
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
	hiloDatos.threadId = 1;
	hiloDatos.paginacion = &paginacion;
	hiloDatos.cola = &cola;
	hiloDatos.salir = NO;
	rc = pthread_create(&escuchador, NULL, escuchaSolicitudes, (void *) &hiloDatos);
	if (rc) {
		printf("Errror al crear hilo con pthread_create() : %d\n", rc);
		exit(-1);
	}
	tim.tv_nsec = 100000000 * tiempo;
	
	do {
		if (!estaVaciaSolicitudes(paginacion)) {
			for (j = sizelist(paginacion.solicitudes); j > 0; j--)
				cargarSolicitud(&paginacion, &err);
		}
		system("clear");
		imprimeTablaMemorias(paginacion);

		printf("\n\n");
		linea();
		printf("Tabla de Procesos\n");
		linea();
		imprimeTablaProcesos(paginacion);

		printf("\n\n");
		linea();
		printf("Tabla de Solicitudes\n");
		linea();
		imprimeTablaSolicitudes(paginacion);

		quantum(&paginacion, &err);
		nanosleep(&tim , &tim2);
	} while (hiloDatos.salir == NO);

	system("clear");
	printf("Adios\n");
	pthread_exit(NULL);
	msgctl(cola.llave, IPC_RMID, NULL);
	return 0;
}

void* escuchaSolicitudes(void* threaddata) {
	int salir = NO;
	ThreadData* data = (ThreadData*) threaddata;
	while (salir == NO) {
		recibir(data->cola, CONTROL_MEMORIA);
		switch(data->cola->nodo.accion) {
			case SALIR:
				data->salir = SI;
				salir = SI;
				break;
			case AGREGAR:
				agregarSolicitud(data->paginacion, data->cola->nodo.dato.tam, data->cola->nodo.dato.usuario);
				break;
			default:
				break;
		}
	}
}




