
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <math.h>

//Bibliotecas locais
#include "primos.h"
#include "matriz.h"

//Variáveis globais
Matriz* matriz;
Submatriz *submatriz;

//controleadores
int contaNumeroPrimo = 0;
int gerenciadorSubmatriz = 0;
int contaSubmatriz = 0;

listaPrimos* lista;

pthread_mutex_t mutexSubmatriz;
pthread_mutex_t mutexPrimeiroNumero;

//Definições do cronômetro -gettimeofday-
float cronExecucao(struct timeval *start, struct timeval *end) {
    return (end->tv_sec - start->tv_sec) + 1e-6*(end->tv_usec - start->tv_usec);
}

int main() {
    //Usando timeval
    struct timeval start;
    struct timeval end;

    lista = criarLista();

	pthread_mutex_init(&mutexSubmatriz, NULL);
	pthread_mutex_init(&mutexPrimeiroNumero, NULL);

	matriz = criarMatriz(TAM_LINHA, TAM_COLUNA);
	popularMatriz();
	submatriz = divideMatriz();

    //Inicia busca e cronômetro
    gettimeofday(&start, NULL);
	iniciarBusca();
    gettimeofday(&end, NULL);

    /*
        Checa se o número de threads for igual a 1 e caso seja satisfeito
        imprime os 5 números primos encontrados
    */
    if (NUM_THREADS == 1 || NUM_THREADS > 1) {
        primos* primeiroPrimo = lista->inicio;
        for (int i = 0; i < 5; i++) {
            if (primeiroPrimo != NULL) {
                printf("Primo encontrado na posição [%ld][%ld]: %d\n", primeiroPrimo->lin, primeiroPrimo->col, matriz->pos[primeiroPrimo->lin][primeiroPrimo->col]);
                primeiroPrimo = primeiroPrimo->proximo;
            }
        }
    }



    if (NUM_THREADS == 1) {
        printf("||||  BUSCA SEQUENCIAL  ||||\n");
    } else {
        printf("||||  BUSCA CONCORRENTE  ||||\n");
    }
    printf("Quantidade de números primos encontrados: %u\n", contaNumeroPrimo);
    printf("Quantidade de threads executadas: %d\n", NUM_THREADS);
    printf("Tempo de execução: %.1fs\n\n", cronExecucao(&start, &end));

    //Após execução, libera a matriz alocada/
	freeMatriz();
}


