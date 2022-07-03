#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include "matriz.h"
#include "sys/time.h"
#include "primos.h"

extern Matriz* matriz;
extern Submatriz* submatriz;


int ehPrimo(int n) {
    double limite;
	int i = 3, controle = 1;

	if (n == 2) {
		return 1;
	}
	else if (n < 2 || n % 2 == 0) {
		return 0;
	}
	else {
		limite = sqrt(n) + 1;
		while (i < limite && controle) {
			if (n % i == 0) {
				controle = 0;
			}
			i += 2;
		}
		return controle;
    }
}

Matriz* criarMatriz(int lin, int col) {
    int i;
	Matriz *matriz = malloc(sizeof(matriz));

	matriz->pos = malloc(lin * sizeof(int*));

	for (i = 0; i < lin; i++) {
		matriz->pos[i] = malloc(col * sizeof(int));
	}

	matriz->lin = lin;
	matriz->col = col;
	return matriz;
}

void popularMatriz() {
    int i, j;

	srand(SEED);
	for (i = 0; i < matriz->lin; i++) {
		for (j = 0; j < matriz->col; j++) {
			matriz->pos[i][j] = rand() % (MAX_RANDOM + 1);
		}
	}
}

Submatriz* divideMatriz() {
    Submatriz *mb;
	int SubmatrizLine, SubmatrizColumn, SubmatrizNumber = 0;

	contaSubmatriz = (matriz->lin * matriz->col) / (LIN_BLOCO * COL_BLOCO);

	mb = malloc(contaSubmatriz * sizeof(Submatriz));

	for (SubmatrizLine = 0; SubmatrizLine < matriz->lin; SubmatrizLine += LIN_BLOCO) {
		for (SubmatrizColumn = 0; SubmatrizColumn < matriz->col; SubmatrizColumn += COL_BLOCO, SubmatrizNumber += 1) {
			mb[SubmatrizNumber].inicioLinha = SubmatrizLine;
			mb[SubmatrizNumber].fimLinha = SubmatrizLine + LIN_BLOCO;
			mb[SubmatrizNumber].inicioColuna = SubmatrizColumn;
			mb[SubmatrizNumber].fimColuna = SubmatrizColumn + COL_BLOCO;
		}
	}

	return mb;
}

void imprimeMatriz() {
    int i, j;

	printf("MATRIZ %u X %u:\n\n", TAM_LINHA, TAM_COLUNA);

	for (i = 0; i < matriz->lin; i++) {
		for (j = 0; j < matriz->col; j++) {
			printf(" %5d ", matriz->pos[i][j]);
		}
		printf("\n");
	}
}

void iniciarBusca() {
    int contaThreads, respostaThread;
	pthread_t threads[NUM_THREADS];

	// Criando as threads e inicializando-as para já executarem suas buscas.
	for (contaThreads = 0; contaThreads < NUM_THREADS; contaThreads++) {
		respostaThread = pthread_create(&threads[contaThreads], NULL, &functionThread, NULL);
		if (respostaThread) {
			printf("ERROR code is %d\n", respostaThread);
			exit(-1);
		}
	}
	// Esperando que as threads finalizem as buscas.
	for (contaThreads = 0; contaThreads < NUM_THREADS; contaThreads++) {
		pthread_join(threads[contaThreads], NULL);
	}
}

void *functionThread(void* nothing) {
    Submatriz* mb;
	int i, j;
	unsigned int contaPrimoThread;

	while (TRUE) {
		pthread_mutex_lock(&mutexSubmatriz);
		mb = (Submatriz*)procuraSubmatriz();
		pthread_mutex_unlock(&mutexSubmatriz);

		if (mb == NULL) {
			pthread_exit(NULL);
		}

		contaPrimoThread = 0;

		for (i = mb->inicioLinha; i < mb->fimLinha; i++) {
			for (j = mb->inicioColuna; j < mb->fimColuna; j++) {
                if (ehPrimo(matriz->pos[i][j])) {
                    primos* primo = malloc(sizeof(primos));
                    primo->lin = i;
                    primo->col = j;
                    primo->proximo = NULL;
                    pthread_mutex_lock(&mutexPrimeiroNumero);
                    adicionarPrimo(primo);
                    pthread_mutex_unlock(&mutexPrimeiroNumero);
                }
			}
		}
	}
}

void* procuraSubmatriz() {
    return (gerenciadorSubmatriz >= contaSubmatriz) ? NULL : &submatriz[gerenciadorSubmatriz++];
}

int freeMatriz() {
    int i;
	for (i = 0; i < matriz->lin; i++) {
		free(matriz->pos[i]);
	}
	free(matriz->pos);
	free(matriz);
	return 1;
}