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