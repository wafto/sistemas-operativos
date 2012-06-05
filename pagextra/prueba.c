#include "funciones.h"

int main() {
	Paginacion paginacion;
	int i;

	crearPaginacion(&paginacion, 10);


	for (i = 0; i < paginacion.memfisica.tam; i++) {
		printf("%d : %d\n",
			paginacion.memfisica.marcos[i].indice, 
			paginacion.memfisica.marcos[i].pagina.num
		);
	}


	return 0;
}

