#ifndef PRIMOS_H
#define PRIMOS_H


extern int contaNumeroPrimo;

typedef struct primos{
    long lin, col;
    struct primos* proximo;
} primos;

typedef struct {
    primos* inicio, *fim;
} listaPrimos;

extern listaPrimos* lista;

//Coloca os números primos em uma lista
void adicionarPrimo(primos* num);
//Cria a lista
listaPrimos* criarLista();

#endif