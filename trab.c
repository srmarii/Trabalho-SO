//compilador online: https://www.onlinegdb.com/

#include <stdio.h>
#include <pthread.h>

int n, atual = 1, primos = 0;

pthread_mutex_t trava;

int ehPrimo(int x) {
    int i;

    if (x < 2)
        return 0;

    for (i = 2; i < x; i++) {
        if (x % i == 0)
            return 0;
    }

    return 1;
}

void *ourThread(void *arg) {
    int id = *(int *)arg, numero;

    while (1) {
        pthread_mutex_lock(&trava);

        if (atual > n)
        {
            pthread_mutex_unlock(&trava);
            break;
        }

        numero = atual;
        atual++;

        pthread_mutex_unlock(&trava);

        if (ehPrimo(numero)) {
            pthread_mutex_lock(&trava);
            primos++;
            pthread_mutex_unlock(&trava);
            
            printf("Thread %d: O numero %d eh primo\n", id, numero);
        } else {
            printf("Thread %d: O numero %d nao eh primo\n", id, numero);
        }
    }

    pthread_exit(NULL);
}

int main() {
    int qtdThreads, i;

    printf("Digite a quantidade de numeros: ");
    scanf("%d", &n);

    printf("Digite a quantidade de threads: ");
    scanf("%d", &qtdThreads);
    
    printf("\n\n");

    pthread_t threads[qtdThreads];
    int ids[qtdThreads];

    pthread_mutex_init(&trava, NULL);

    for (i = 0; i < qtdThreads; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, ourThread, &ids[i]);
    }

    for (i = 0; i < qtdThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("O numero total de numeros primos encontrados foi: %d\n", primos);

    pthread_mutex_destroy(&trava);

    return 0;
}