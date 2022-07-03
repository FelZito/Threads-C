#include "primos.h"
#include <stdlib.h>

void adicionarPrimo(primos* num){
    if (contaNumeroPrimo == 0) {
        lista->inicio = num;
        lista->fim = num;
        contaNumeroPrimo ++;
        return;
    }

    lista->fim->proximo = num;
    lista->fim = num;
    contaNumeroPrimo ++;
}


listaPrimos* criarLista() {
    listaPrimos* ll = malloc(sizeof(listaPrimos));
    ll->inicio = NULL;
    ll->fim = NULL;
    return ll;
}