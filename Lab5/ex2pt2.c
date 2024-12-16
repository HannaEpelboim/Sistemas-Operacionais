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
#define N_THREADS 10 // Número de tarefa

int p[TAM_VETOR];
int soma[N_THREADS] = {0};
pthread_t tid_filho[N_THREADS];

void* trabalhador(void* seg){
    // int*ptemp = (int*) seg;
    // int segmento = *ptemp;
    for (int j = 0; j < TAM_VETOR; j++) {
        p[j] *= 2;
        p[j] += 2;
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
            printf("Criou o trabalhador %d\n", i);
        }
    }

    for (int i = 0; i < N_THREADS; i++) {
        pthread_join(tid_filho[i], NULL);
    }
    int valor = p[0];
    int somaTotal = 1;
    // printf("Vetor: ");
    // for (int i = 0; i < TAM_VETOR; i++) {
    //     printf("%d\n", p[i]);
    // }

    for (int j = 0; j < TAM_VETOR; j++) {
        // printf("%d\n",p[j]);
        if(p[j]!=valor){
           printf("Valores diferentes!!! Vetor[0] = %d\t Vetor[%d] = %d\n", valor, j, p[j]);
           somaTotal = 0;
        }
    }

   if(somaTotal){
       printf("Todos os valores iguais à %d!!!\n", valor);
   }

    return 0;
}