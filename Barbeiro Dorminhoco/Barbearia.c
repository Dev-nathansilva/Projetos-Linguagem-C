/*----TRABALHO FEITO PELO ALUNO NATHAN SILVA OLIVEIRA----*/
/*
	INSTRUÇÕES
	1 - a barbearia assim como foi pedido no slide, acontecerá de forma automática, então certos momentos
	o atendimento vai ser rápido e os clientes não vai precisar ir embora porque não vai lotar. 
	Então defina bem os números de clientes e de barbeiros!!! 
	2 - RECOMENDAVÉL que use o numero de barbeiros igual ao número de cadeiras, ou seja, 3.
	3 - a barbearia encerra quando todos os cortes tiverem finalizados.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_CADEIRAS 3

int clientesEsperando = 0;
int numeroBarbeiros;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t barbeirosDisponiveis = PTHREAD_COND_INITIALIZER;

struct Barbeiro {
    int id;
};

void *barbeiro(void *arg) {
    struct Barbeiro *barb = (struct Barbeiro *) arg;
    int id = barb->id;
    
    while (1) {
        pthread_mutex_lock(&mutex);
        while (clientesEsperando == 0) {
            printf("Barbeiro %d: nao ha clientes. (dormindo...zzz) \n", id);
            pthread_cond_wait(&barbeirosDisponiveis, &mutex);
        }

        clientesEsperando--;
        printf("Barbeiro %d atendendo um cliente.  //  Clientes esperando: %d\n", id, clientesEsperando);
        pthread_mutex_unlock(&mutex);

        sleep(rand() % 5); // tempo de corte do cabelo

        pthread_mutex_lock(&mutex);
        printf("Barbeiro %d terminou de atender um cliente.\n", id);
        pthread_cond_signal(&barbeirosDisponiveis);
        pthread_mutex_unlock(&mutex);
    }
    
    return NULL;
}

void *cliente(void *arg) {
    int id = *((int *) arg);
    pthread_mutex_lock(&mutex);

    if (clientesEsperando < MAX_CADEIRAS) {
        clientesEsperando++;
        printf("Cliente %d chegou.  //  Clientes esperando: %d\n", id, clientesEsperando);

        if (clientesEsperando == MAX_CADEIRAS) {
            printf("Todas as cadeiras estao ocupadas, quem chegar apartir de agora pode querer ir embora.\n");
            pthread_cond_signal(&barbeirosDisponiveis);
            pthread_mutex_unlock(&mutex);

            sleep(rand() % 3); 

            pthread_mutex_lock(&mutex);
            if (clientesEsperando == MAX_CADEIRAS) {
                clientesEsperando--;
                printf("Cliente %d: vixi! todas as cadeiras estao ocupadas, vou embora!\n", id);
                pthread_mutex_unlock(&mutex);
                return NULL;
            }
        }

        pthread_cond_signal(&barbeirosDisponiveis);
        pthread_mutex_unlock(&mutex);

        sleep(rand() % 5); // tempo de corte de cabelo

        pthread_mutex_lock(&mutex);
        printf("Cliente %d: corte de cabelo finalizado!!!\n", id);
        pthread_mutex_unlock(&mutex);
    } else {
        printf("Cliente %d: vixi! todas as cadeiras estao ocupadas, vou embora!\n", id);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main() {
	printf("--------- BEM VINDO A BARBEARIA!!! ------\n");
    int numClientes, i ;
    printf("[1] Digite o numero de clientes :  ");
    scanf("%d", &numClientes);
    printf("[2] Digite o numero de barbeiros :  ");
    scanf("%d", &numeroBarbeiros);

    pthread_t *barbeiros = malloc(numeroBarbeiros * sizeof(pthread_t));
    pthread_t *clientes = malloc(numClientes * sizeof(pthread_t));
    int *idsClientes = malloc(numClientes * sizeof(int));
    
    srand(time(NULL));

    // threads dos barbeiros
    struct Barbeiro *barbeirosDados = malloc(numeroBarbeiros * sizeof(struct Barbeiro));
    for (i = 0; i < numeroBarbeiros; i++) {
        barbeirosDados[i].id = i;
        pthread_create(&barbeiros[i], NULL, barbeiro, &barbeirosDados[i]);
    }

    // threads dos clientes
    for (i = 0; i < numClientes; i++) {
        idsClientes[i] = i;
        pthread_create(&clientes[i], NULL, cliente, &idsClientes[i]);
        usleep(rand() % 500); // Intervalo entre chegadas de clientes 
    }

    // Aguarda as threads terminarem
    for (i = 0; i < numeroBarbeiros; i++) {
        pthread_join(barbeiros[i], NULL);
    }
    for (i = 0; i < numClientes; i++) {
        pthread_join(clientes[i], NULL);
    }

	// liberação da memória alocada usando a função free.
    free(barbeiros);
    free(clientes);
    free(idsClientes);
    free(barbeirosDados);

    return 0;
}

