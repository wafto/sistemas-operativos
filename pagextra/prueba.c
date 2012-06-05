#include "funciones.h"

int main() {
	Memoria mem;
	int i;

	crearMemoria(&mem, 10);


	for (i = 0; i < mem.tam; i++) {
		printf("%d : %d\n", mem.marcos[i].indice, mem.marcos[i].pagina.num);
	}


	return 0;
}