#include "particiones.h"

int inicializar(ColaIPC* ipc) {
	ipc->llave = ftok(FLLAVE, CLAVE);
	return (ipc->cola = msgget(ipc->llave, IPC_CREAT | 0600)) == -1 ? 0 : 1;
}

int enviar(ColaIPC* ipc, long tipo, int accion) {
	ipc->nodo.tipo = tipo;
	ipc->nodo.accion = accion;
	return msgsnd(ipc->cola, &ipc->nodo, IPC_LONGITUD, IPC_NOWAIT);
}

int recibir(ColaIPC* ipc, long tipo) {
	return msgrcv(ipc->cola, &ipc->nodo, IPC_LONGITUD, tipo, 0);
}

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