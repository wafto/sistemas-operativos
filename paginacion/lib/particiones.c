#include "particiones.h"

Solicitud* crearSolicitud(int tam, const char* usuario) {
	Solicitud* solicitud = (Solicitud*) malloc(sizeof(Solicitud));
	if (solicitud != NULL) {
		solicitud->tam = tam;
		strcpy(solicitud->usuario, usuario);
	}
	return solicitud;
}

Proceso* crearProceso(int pid, int tam, int tampag, const char* usuario) {
	Proceso* proceso;
	int i;
	if (pid < 0 || tam <= 0 || tampag <= 0) return NULL;
	proceso = (Proceso*) malloc(sizeof(Proceso));
	if (proceso != NULL) {
		proceso->pid = pid;
		proceso->tam = tam;
		proceso->xpag = 0;
		proceso->npag = (tam / tampag) + (tam % tampag == 0 ? 0 : 1);
		strcpy(proceso->usuario, usuario);
		proceso->paginas = (Pagina*) malloc(sizeof(Pagina) * proceso->npag);
		if (proceso->paginas == NULL) {
			free(proceso);
			proceso = NULL;
		} else {
			for (i = 0; i < proceso->npag; i++) {
				proceso->paginas[i].tscont = 1 + (int)(5.0 * rand() / (RAND_MAX + 1.0));
				proceso->paginas[i].marco = NULL;
			}
		}
	}
	return proceso;
}

Tabla* crearTabla(int tam) {
	Tabla* tabla = NULL;
	int i;
	if (tam > 0) {
		tabla = (Tabla*) malloc(sizeof(Tabla));
		if (tabla != NULL) {
			tabla->marcos = (Marco*) malloc(sizeof(Marco) * tam);
			if (tabla->marcos == NULL) {
				free(tabla);
				tabla = NULL;
			} else {
				tabla->tam = tam;
				for (i = 0; i < tam; i++) {
					tabla->marcos[i].estado = LIBRE;
					tabla->marcos[i].proceso = NULL;
					tabla->marcos[i].pagina = 0;
				}
			}
		}
	}
	return tabla;
}

int inicializarPaginacion(Paginacion* paginacion, int memoria, int tampag) {
	int mem;
	if (memoria > 0 && tampag > 0) {
		mem = memoria / tampag;
		paginacion->cpids = 0;
		paginacion->tampag = tampag;
		paginacion->memfisica = crearTabla(mem);
		paginacion->memvirtual = crearTabla(mem);
		initlist(&paginacion->solicitudes);
		initlist(&paginacion->procesos);
		srand((unsigned int)time(NULL));
		paginacion->actual = NULL;
		paginacion->meta.ant = paginacion->meta.actual = paginacion->meta.sig = NULL;
		return paginacion->memfisica != NULL && paginacion->memvirtual != NULL ? 1 : 0;
	}
	return 0;
}

int estaVaciaSolicitudes(Paginacion paginacion) {
	return isemptylist(paginacion.solicitudes);
}

int agregarSolicitud(Paginacion* paginacion, int tam, const char* usuario) {
	Solicitud* solicitud = crearSolicitud(tam, usuario);
	if (solicitud != NULL)
		return pushbacklist(&paginacion->solicitudes, solicitud);
	return 0;
}

int paginasLibresMemFisica(Paginacion paginacion) {
	int i, libre = 0;
	for (i = 0; i < paginacion.memfisica->tam; i++)
		libre += paginacion.memfisica->marcos[i].proceso == NULL ? 1 : 0;
	return libre;
}

int paginasLibresMemVirtual(Paginacion paginacion) {
	int i, libre = 0;
	for (i = 0; i < paginacion.memvirtual->tam; i++)
		libre += paginacion.memvirtual->marcos[i].proceso == NULL ? 1 : 0;
	return libre;
}

int paginasLibres(Paginacion paginacion) {
	return paginasLibresMemFisica(paginacion) + paginasLibresMemVirtual(paginacion);
}

int cargarSolicitud(Paginacion* paginacion, int* err) {
	Solicitud* solicitud;
	Proceso* proceso;
	int i, j, libresFisica, libresVirtual;
	*err = 0;
	libresFisica = paginasLibresMemFisica(*paginacion);
	libresVirtual = paginasLibresMemVirtual(*paginacion);
	if (!estaVaciaSolicitudes(*paginacion) && (libresFisica + libresVirtual) > 0) {
		solicitud = (Solicitud*) popfrontlist(&paginacion->solicitudes);
		proceso = crearProceso(
			++paginacion->cpids,
			solicitud->tam,
			paginacion->tampag,
			solicitud->usuario
		);
		if (proceso != NULL) {
			if ((libresFisica + libresVirtual) >= proceso->npag) {
				free(solicitud);
				if (pushbacklist(&paginacion->procesos, proceso)) {
					j = 0;
					if (libresFisica > 0) {
						for (i = 0; i < paginacion->memfisica->tam && j < proceso->npag; i++) {
							if (paginacion->memfisica->marcos[i].proceso == NULL) {
								paginacion->memfisica->marcos[i].estado = LISTO;
								paginacion->memfisica->marcos[i].proceso = proceso;
								paginacion->memfisica->marcos[i].pagina = j;
								proceso->paginas[j].marco = &(paginacion->memfisica->marcos[i]);
								j++;
							} 
						}
					}
					if (libresVirtual > 0 && j < proceso->npag) {
						for (i = 0; i < paginacion->memvirtual->tam && j < proceso->npag; i++) {
							if (paginacion->memvirtual->marcos[i].proceso == NULL) {
								paginacion->memvirtual->marcos[i].estado = LISTO;
								paginacion->memvirtual->marcos[i].proceso = proceso;
								paginacion->memvirtual->marcos[i].pagina = j;
								proceso->paginas[j].marco = &(paginacion->memvirtual->marcos[i]);
								j++;
							} 
						}
					}
					return 1;
				}
			} else {
				if (proceso != NULL) free(proceso);
				pushbacklist(&paginacion->solicitudes, solicitud);
				*err |= NO_MEMORIA;
			}
		} else {
			*err |= NO_MALLOC;
		}
	} else {
		*err |= NO_SOLICITUDES;
	}
	return 0;
}

void imprimeTablaSolicitudes(Paginacion paginacion) {
	IteratorList iter;
	Solicitud* solicitud;
	int j, i = 0;
	if (estaVaciaSolicitudes(paginacion)) {
		printf("La cola de solicitudes esta vacia.\n");
	} else {
		printf("%-12s%-12s%-12s\n", "Numero", "Tamaño", "Usuario");
		for (j = 0; j < 36; j++) printf("-");
		printf("\n");
		for (iter = beginlist(paginacion.solicitudes); iter != NULL; iter = nextlist(iter)) {
			solicitud = (Solicitud*) dataiterlist(iter); 
			printf("%-12d%-12d%-12s\n", ++i, solicitud->tam, solicitud->usuario);
		}
	}
}

void imprimeTablaProcesos(Paginacion paginacion) {
	IteratorList iter;
	Proceso* proceso;
	int i;
	if (isemptylist(paginacion.procesos)) {
		printf("La lista de procesos esta vacia.\n");
	} else {
		printf("%-12s%-12s%-12s%-12s%-12s\n", "PID", "Tamaño", "Paginas", "Ejecucion", "Usuario");
		for (i = 0; i < 60; i++) printf("-");
		printf("\n");
		for (iter = beginlist(paginacion.procesos); iter != NULL; iter = nextlist(iter)) {
			proceso = (Proceso*) dataiterlist(iter); 
			printf("%-12d%-12d%-12d%-12d%-12s\n",
				proceso->pid,
				proceso->tam,
				proceso->npag,
				proceso->xpag,
				proceso->usuario
			);
		}
	}
}

void imprimeTablaMemorias(Paginacion paginacion) {
	int i;
	printf("%-62s%-62s\n\n", "Memoria Física", "Memoria Virtual");
	printf("%-10s%-10s%-10s%-10s%-10s%-10s | %-10s%-10s%-10s%-10s%-10s%-10s\n",
		"Marco", "Estado", "Proceso", "Pagina", "Tiempo", "Usuario",
		"Marco", "Estado", "Proceso", "Pagina", "Tiempo", "Usuario"
	);
	for (i = 0; i < 122; i++) printf("-");
	printf("\n");
	for (i = 0; i < paginacion.memfisica->tam; i++) {
		printf("%-10d", i);
		switch (paginacion.memfisica->marcos[i].estado) {
			case LISTO:     printf("%-10s", "listo");     break;
			case ESPERA:    printf("%-10s", "espera");    break;
			case EJECUCION: printf("%-10s", "ejecucion"); break;
			case PARADA:    printf("%-10s", "parada");    break;
			case FIN:       printf("%-10s", "fin");       break;
			default: printf("%-10s", "libre");
		}
		if (paginacion.memfisica->marcos[i].proceso == NULL) {
			printf("%-10s%-10s%-10s%-10s | ", "-", "-", "-", "-");
		} else {
			printf("%-10d%-10d%-10d%-10s | ",
				paginacion.memfisica->marcos[i].proceso->pid,
				paginacion.memfisica->marcos[i].pagina,
				paginacion.memfisica->marcos[i].proceso->paginas[paginacion.memfisica->marcos[i].pagina].tscont,
				paginacion.memfisica->marcos[i].proceso->usuario
			);
		}
		printf("%-10d", i);
		switch (paginacion.memvirtual->marcos[i].estado) {
			case LISTO:     printf("%-10s", "listo");     break;
			case ESPERA:    printf("%-10s", "espera");    break;
			case EJECUCION: printf("%-10s", "ejecucion"); break;
			case PARADA:    printf("%-10s", "parada");    break;
			case FIN:       printf("%-10s", "fin");       break;
			default: printf("%-10s", "libre");
		}
		if (paginacion.memvirtual->marcos[i].proceso == NULL) {
			printf("%-10s%-10s%-10s%-10s", "-", "-", "-", "-");
		} else {
			printf("%-10d%-10d%-10d%-10s",
				paginacion.memvirtual->marcos[i].proceso->pid,
				paginacion.memvirtual->marcos[i].pagina,
				paginacion.memvirtual->marcos[i].proceso->paginas[paginacion.memvirtual->marcos[i].pagina].tscont,
				paginacion.memvirtual->marcos[i].proceso->usuario
			);
		}
		printf("\n");
	}
}

int quantum(Paginacion* pag, int* err) {
	Proceso *proceso = NULL, *aux = NULL;
	IteratorList iter;
	if (isemptylist(pag->procesos)) {
		*err = NO_PROCESOS;
		return 0;
	}
	if (pag->actual == NULL) pag->actual = beginlist(pag->procesos);
	if (iter = pag->actual)
		proceso = (Proceso*) dataiterlist(iter);
	if (proceso != NULL) {
		if (pag->meta.ant != NULL) {
			if (pag->meta.ant->proceso->paginas[pag->meta.ant->pagina].tscont <= 0) {
				pag->meta.ant->estado = LIBRE;
				pag->meta.ant->proceso = NULL;
				pag->meta.ant->pagina = 0;
			} else {
				pag->meta.ant->estado = ESPERA;
			}
		}
		pag->actual = nextlist(pag->actual);
		pag->meta.actual = proceso->paginas[proceso->xpag].marco;
		pag->meta.actual->estado = EJECUCION;
		proceso->paginas[proceso->xpag].tscont -= 1;
		if (proceso->paginas[proceso->xpag].tscont <= 0)
			proceso->xpag += 1;
		if (proceso->xpag >= proceso->npag) {
			proceso = (Proceso*) popiterlist(&pag->procesos, iter);
			free(proceso->paginas);
			free(proceso);
		}
		pag->meta.ant = pag->meta.actual;
		aux = (Proceso*) dataiterlist(pag->actual);
		pag->meta.sig = (aux != NULL) ? aux->paginas[aux->xpag].marco : NULL;
		if (pag->meta.sig == NULL && isemptylist(pag->procesos)) {
			pag->meta.actual->estado = LIBRE;
			pag->meta.actual->proceso = NULL;
			pag->meta.actual->pagina = 0;
		}
		return 1;
	}
	*err = CRITICO;
	return 0;
}

