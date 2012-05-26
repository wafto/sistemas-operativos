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
	unsigned int iseed = (unsigned int)time(NULL);
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
			srand(iseed);
			for (i = 0; i < proceso->npag; i++) {
				proceso->paginas[i].estado = LISTO;
				proceso->paginas[i].tscont = (rand() % 5) + 1;
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
					tabla->marcos[i].estado = 0;
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
		mem = (memoria / tampag) + (memoria % tampag == 0 ? 0 : 1);
		paginacion->cpids = 0;
		paginacion->tampag = tampag;
		paginacion->memfisica = crearTabla(mem);
		paginacion->memvirtual = crearTabla(mem);
		initlist(&paginacion->solicitudes);
		initlist(&paginacion->procesos);
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

int cargarSolicitud(Paginacion* paginacion) {
	Solicitud* solicitud;
	Proceso* proceso;
	int i, j, libresFisica, libresVirtual;
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
		if (proceso != NULL && (libresFisica + libresVirtual) >= proceso->npag) {
			free(solicitud);
			if (pushbacklist(&paginacion->procesos, proceso)) {
				j = 0;
				if (libresFisica > 0) {
					for (i = 0; i < paginacion->memfisica->tam && j < proceso->npag; i++) {
						if (paginacion->memfisica->marcos[i].proceso == NULL) {
							paginacion->memfisica->marcos[i].estado = LISTO;
							paginacion->memfisica->marcos[i].proceso = proceso;
							paginacion->memfisica->marcos[i].pagina = j++;
						} 
					}
				}
				if (libresVirtual > 0 && j < proceso->npag) {
					for (i = 0; i < paginacion->memvirtual->tam && j < proceso->npag; i++) {
						if (paginacion->memvirtual->marcos[i].proceso == NULL) {
							paginacion->memvirtual->marcos[i].estado = LISTO;
							paginacion->memvirtual->marcos[i].proceso = proceso;
							paginacion->memvirtual->marcos[i].pagina = j++;
						} 
					}
				}
				return 1;
			}
		} else {
			if (proceso != NULL) free(proceso);
			pushbacklist(&paginacion->solicitudes, solicitud);
		}
	}
	return 0;
}

void imprimeTablaSolicitudes(Paginacion paginacion) {
	IteratorList iter;
	Solicitud* solicitud;
	int i = 0;
	if (estaVaciaSolicitudes(paginacion)) {
		printf("La cola de solicitudes esta vacia.\n");
	} else {
		printf("Num \tTam \tUsuario \n");
		for (iter = beginlist(paginacion.solicitudes); iter != NULL; iter = nextlist(iter)) {
			solicitud = (Solicitud*) dataiterlist(iter); 
			printf("%d \t%d \t%s \n", ++i, solicitud->tam, solicitud->usuario);
		}
	}
}

void imprimeTablaProcesos(Paginacion paginacion) {
	IteratorList iter;
	Proceso* proceso;
	if (isemptylist(paginacion.procesos)) {
		printf("La lista de procesos esta vacia.\n");
	} else {
		printf("Pid \tTam \tPags. \tExec \tUsuario \n");
		for (iter = beginlist(paginacion.procesos); iter != NULL; iter = nextlist(iter)) {
			proceso = (Proceso*) dataiterlist(iter); 
			printf("%d \t%d \t%d \t%d \t%s \n",
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
	for (i = 0; i < paginacion.memfisica->tam; i++) {
		printf("%d %d %d\n",
			paginacion.memfisica->marcos[i].estado,
			paginacion.memfisica->marcos[i].proceso != NULL ? paginacion.memfisica->marcos[i].proceso->pid : 0,
			paginacion.memfisica->marcos[i].pagina
		);
	}
}


