//Érica Oliveira Regnier
//Hanna Epelboim Assunção

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#define TAM_VETOR 100000 // Tamanho total do vetor
#define N_THREADS 100 // Número de tarefas
#define SEGMENT_SIZE (TAM_VETOR / N_THREADS) // Tamanho do segmento para cada tarefa

int p[TAM_VETOR];
int soma[N_THREADS] = {0};
pthread_t tid_filho[N_THREADS];

void* trabalhador(void* seg){
    int*ptemp = (int*) seg;
    int segmento = *ptemp;
    for (int j = 0; j < SEGMENT_SIZE; j++) {
            int index = segmento * SEGMENT_SIZE + j;
            p[index] *= 2;
            soma[segmento] += p[index];
    }
    
    pthread_exit(NULL);
}

int main(){
    pthread_t tid_filho[N_THREADS];
    
    for (int i = 0; i < TAM_VETOR; i++) {
        p[i] = 5; // Definindo todos os elementos como 5
    }

    int indices[N_THREADS]; 
    for (int i = 0; i < N_THREADS; i++) {
        indices[i] = i;
        if (pthread_create(&tid_filho[i], NULL, trabalhador, &indices[i]) != 0) {
            perror("Erro na criação do novo thread\n");
            exit(2);
        } else {
            //printf("Criou o trabalhador %d\n", i);
        }
    }


    int somaTotal = 0;
    for (int i = 0; i < N_THREADS; i++) {
        pthread_join(tid_filho[i], NULL);
        //printf("Thread: %d Soma: %d\n", i, soma[i]);
        somaTotal += soma[i];
    }
        printf("Soma de todas as threads: %d\n",somaTotal);

    return 0;
}