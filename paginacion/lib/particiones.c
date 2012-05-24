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
	if (memoria > 0 && tampag > 0) {
		paginacion->cpids = 0;
		paginacion->tampag = tampag;
		paginacion->memfisica = crearTabla(memoria);
		paginacion->memvirtual = crearTabla(memoria);
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
		return pushfrontlist(&paginacion->solicitudes, solicitud);
	return 0;
}

int cargarSolicitud(Paginacion* paginacion) {
	Solicitud* solicitud;
	Proceso* proceso;
	if (!estaVaciaSolicitudes(*paginacion)) {
		solicitud = (Solicitud*) popbacklist(&paginacion->solicitudes);
		proceso = crearProceso(
			++paginacion->cpids,
			solicitud->tam,
			paginacion->tampag,
			solicitud->usuario
		);
		free(solicitud);
		if (proceso != NULL && pushbacklist(&paginacion->procesos)) {
			/* Hasta este punto tenemos el proceso en nuestra lista */
		}
		return 1;
	}
	return 0;
}



