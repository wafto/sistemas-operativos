#include "funciones.h"

int crearMemoria(Memoria* mem, int tam, int tipo) {
	int i;
	if (tam > 0) {
		mem->tam = tam;
		mem->tipo = tipo;
		mem->marcos = (Marco*) malloc(sizeof(Marco) * tam);
		if (mem->marcos != NULL) {
			for (i = 0; i < mem->tam; i++) {
				mem->marcos[i].indice = i;
				mem->marcos[i].pagina.num = 0;
				mem->marcos[i].pagina.proceso = NULL;
			}
			return 1;
		}
	}
	return 0;
}

int crearPaginacion(Paginacion* pag, int tam) {
	crearMemoria(&pag->memfisica, tam, FISICA);
	crearMemoria(&pag->memvirtual, tam, VIRTUAL);
	return 1;
}