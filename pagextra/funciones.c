#include "funciones.h"

int crearMemoria(Memoria* mem, int tam) {
	int i;
	if (tam > 0) {
		mem->tam = tam;
		mem->marcos = (Marco*) malloc(sizeof(Marco) * tam);
		if (mem->marcos != NULL) {
			for (i = 0; i < mem->tam; i++) {
				mem->marcos[i].indice = i;
				mem->marcos[i].pagina.num = 0;
				/*mem->marcos[i].pagina.proceso = NULLt;*/
			}
			return 1;
		}
	}
	return 0;
}