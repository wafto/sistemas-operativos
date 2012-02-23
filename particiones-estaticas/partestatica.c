#include <stdio.h>
#define MAX  10
#define FREE -1

int menu () {
	int opt;
	printf("---------------------------------------------------------\n");
	printf(" 1 - Capturar Solicitud.\n");
	printf(" 2 - Cargar Proceso.\n");
	printf(" 3 - Terminar Proceso.\n");
	printf(" 4 - Mostrar Estado.\n");
	printf(" 5 - Salir.\n");
	printf("\tOpcion: ");
	scanf("%d", &opt);
	printf("---------------------------------------------------------\n");
	return opt;
}

int main(int argc, char const *argv[]) {

	int tabla[MAX][2];
	int solicitud[MAX];
	int procesos[MAX][2];
	int i, j, k, l, m, memtam = 0, partam = 0, pid = 0, partmax = 0;

	while (1) {
		printf("Cuanta memoria requiere en unidades (minimo %d): ", MAX);
		scanf("%d", &memtam);
		if (memtam < MAX)
			printf(" - Unidades de memoria invalida, vuelva a intentar\n");
		else
			break;
	}

	while (1) {
		printf("Cuantas particiones requiere en unidades (maximo %d): ", MAX);
		scanf("%d", &partam);
		if (partam > MAX || partam <= 0)
			printf(" - Unidades de particiones invalida, vuelva a intentar\n");
		else
			break;
	}

	printf("Procedemos a asignar el tamaño de cada particion.\n");
	printf("La ultima particion se le asigna el resto de memoria.\n");
	i = j = l = 0;
	for (; i < partam - 1; i++) {	
		k = memtam - partam - l + i + 1;
		printf(" - Unidades de memoria disponibles: %d\n", k);
		while (1) {
			printf("Tamaño para la particion %d: ", i);
			scanf("%d", &m);
			if (m <= 0 || m > k)
				printf(" - Tamaño de particiones invalida, vuelva a intentar\n");
			else
				break;
		}
		tabla[i][0] = m;
		tabla[i][1] = -1;
		l += m;
	}
	tabla[partam - 1][0] = memtam - l;
	tabla[partam - 1][1] = FREE;

	for (i = 0; i < MAX; i++) { solicitud[i] = FREE; procesos[i][0] = FREE; }
	for (i = 0; i < partam; i++) if (tabla[i][0] >= partmax) partmax = tabla[i][0];

	while (1) {
		switch (menu()) {
			case 1:
				for (i = 0; i < MAX; i++) {
					if (solicitud[i] == FREE) {
						while (1) {
							printf("Ingrese la cantidad de memoria para el proceso: ");
							scanf("%d", &k);
							if (k < 1 || k > partmax) {
								printf(" - Tamaño de memoria para el proceso invalido.\n");	
							} else {
								solicitud[i] = k;
								break;
							}
						}
						break;
					}
				}
				if (i == MAX - 1)
					printf("Lista de solicitudes llena.\n");
				break;
			case 2:
				j = FREE;
				i = 0;
				for (; i < MAX; i++) {
					if (solicitud[i] != FREE) {
						j = solicitud[i];
						solicitud[i] = FREE;
						for (; i < MAX - 1; i++) {
							solicitud[i] = solicitud[i + 1];
						}
						break;
					}
				}
				if (j != FREE) {
					k = FREE;
					for (i = 0; i < partam; i++) if (tabla[i][0] >= j && tabla[i][1] == FREE) { k = i; break; }

					if (k != FREE) {
						for (i = 0; i < MAX; i++) {
							if (procesos[i][0] == FREE) {
								procesos[i][0] = pid++;
								procesos[i][1] = j;
								tabla[k][1] = procesos[i][0];
								break;
							}
						}
					} else {
						printf(" - No hay suficiente espacio en alguna particion.\n");
						printf(" - Se ingresa al final de la cola de solicitudes.\n");
						for (i = 0; i < MAX; i++) if (solicitud[i] == FREE) { solicitud[i] = j; break; }
					}
				} else {
					printf(" - No hay solicitudes en la cola.\n");
				}
				break;
			case 3:
				printf("Ingrese el id del proceso que quiere terminar: ");
				scanf("%d", &l);
				k = m = FREE; 
				for (i = 0; i < partam; i++) if (tabla[i][1] == l) { k = i; break; }
				for (i = 0; i < MAX; i++) if (procesos[i][0] == l) { m = i; break; }
				if (k != FREE && m != FREE) {
					tabla[k][1] = FREE;
					procesos[m][0] = FREE;
				} else {
					printf(" - Id de proceso invalido.\n");
				}
				break;
			case 4:
				printf("Part. \tTamaño \tEstado \n");
				for (i = 0; i < partam; i++) {
					printf("%d \t%d \t%s PID(%d)\n", i, tabla[i][0], tabla[i][1] == FREE ? "Libre" : "Ocupado", tabla[i][1]);
				}
				break;
			case 5:
				return 0;
				break;
			default:
				printf("\tOpcion invalida.\n");
		}
	}
}