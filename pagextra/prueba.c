#include "funciones.h"


typedef struct {
	int x;
	int y;
} Punto;

void imprimePunto(Punto p) {
	/*p.x += 1;*/
	printf("punto(%d, %d)\n", p.x, p.y);
}

void imprimePuntoR(const Punto* p) {
	p->x += 1;
	printf("punto(%d, %d)\n", p->x, (*p).y);
}


int main() {
/*	Paginacion paginacion;
	int i;

	crearPaginacion(&paginacion, 10);
	for (i = 0; i < paginacion.memfisica.tam; i++) {
		printf("%d : %d\n",
			paginacion.memfisica.marcos[i].indice, 
			paginacion.memfisica.marcos[i].pagina.num
		);
	}
	imprimePaginacion(paginacion);
*/
	Punto origen, destino;

	origen.x = 0;
	origen.y = 0;


	destino.x = 10;
	destino.y = 30;

	imprimePuntoR(&origen);
	imprimePuntoR(&destino);


	imprimePunto(origen);
	imprimePunto(destino);


	
	tam(char* cadena);


	return 0;
}

