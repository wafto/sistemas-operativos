#include "comun.h"

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

int pregunta(const char* pregunta) {
	char respuesta;
	printf("%s [s] para si y cualquier otra letra para no: ", pregunta);
	fflush(stdin);
	scanf("%s", &respuesta);
	return respuesta == 's' ? 1 : 0;
}