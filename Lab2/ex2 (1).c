//Érica Oliveira Regnier
//Hanna Epelboim Assunção


#include <stdio.h>


#include <stdlib.h>


#include <sys/shm.h>


#include <sys/wait.h>


#include <sys/types.h>


#include <unistd.h>


#define TAM_VETOR 10000 // Tamanho total do vetor


#define N_PROCESSES 100 // Número de processos


int main() {


   int *p, segmento;


   // Criação da memória compartilhada


   segmento = shmget(IPC_PRIVATE, TAM_VETOR * sizeof(int), IPC_CREAT | 0666);


   if (segmento == -1) {


       perror("Erro ao alocar memória compartilhada");


       exit(1);


   }


   // Associação da memória compartilhada ao processo principal


   p = (int *)shmat(segmento, NULL, 0);




   if (p == (void *)-1) {


       perror("Erro ao associar a memória compartilhada");


       exit(1);


   }


   // Preenchendo o vetor


   for (int i = 0; i < TAM_VETOR; i++) {


       p[i] = 5;


   }


  


   // Exibindo o vetor para referência


   printf("Vetor: ");


   for (int i = 0; i < TAM_VETOR; i++) {


       printf("%d ", p[i]);


   }


   printf("\n");


   // Criando processos para buscar a chave no vetor


   for (int i = 0; i < N_PROCESSES; i++) {


       pid_t pid = fork();


       if (pid < 0) {


           perror("Erro na criação do novo processo");


           exit(2);


       } else if (pid == 0) {  // Processo filho


           for (int j = 0; j < TAM_VETOR; j++) {


               p[j] *= 2;


               p[j] += 2;




           }


           // Exibindo o vetor para referência


   // printf("Vetor: ");


   // for (int i = 0; i < TAM_VETOR; i++) {


   //     printf("%d ", p[i]);


   // }


   printf("\n");


           shmdt(p); // Desanexa a memória compartilhada


           exit(0);  // Termina o processo filho


       }


   }


   // Processo pai espera todos os filhos terminarem


   for (int i = 0; i < N_PROCESSES; i++) {


       waitpid(-1,NULL,0);


   }


   int valor = p[0];


   int igual = 1;


   for (int j = 0; j < TAM_VETOR; j++) {


       if(p[j]!=valor){


           printf("Valores diferentes!!!\n Vetor[0] = %d\n Vetor[%d] = %d\n", valor, j, p[j]);


           igual = 0;


       }


   }


   if(igual){
       printf("Todos os valores iguais à %d!!!\n", valor);
   }


  


   // Libera a memória compartilhada


   shmdt(p);


   shmctl(segmento, IPC_RMID, NULL);


   return 0;


}

