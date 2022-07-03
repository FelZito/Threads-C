#ifndef MATRIZ_H
#define MATRIZ_H

#include <pthread.h>

//Definições de condições
#define TRUE 1
#define FALSE 0

//Definições da matriz
#define TAM_LINHA 20000
#define TAM_COLUNA 20000
#define MAX_RANDOM 29999

//Definições da submatriz
#define SEED 7 //Semente srandom()
#define NUM_THREADS 2
#define LIN_BLOCO 200 //Linhas da Submatriz
#define COL_BLOCO 200 //Colunas da Submatriz

//Variáveis globais -main-
extern int contaNumeroPrimo;
extern int gerenciadorSubmatriz;
extern int contaSubmatriz;
extern pthread_mutex_t mutexSubmatriz;
extern pthread_mutex_t mutexPrimeiroNumero;


typedef struct  {
    int **pos, lin, col;
} Matriz;

typedef struct  {
    int inicioLinha, inicioColuna, fimLinha, fimColuna;
} Submatriz;

int ehPrimo(int);
Matriz* criarMatriz(int, int);
void popularMatriz();
Submatriz* divideMatriz();
void imprimeMatriz();
void iniciarBusca();
void *functionThread(void*);
void* procuraSubmatriz();
int freeMatriz();

#endif