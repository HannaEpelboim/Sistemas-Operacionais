#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>


#define NUM_PROCESSOS 3
#define TIMESLICE 1 // Timeslice de 1 segundo




// Estrutura do no
typedef struct No {
   int processo;
   struct No* next;
} No;


// Estrutura da fila
typedef struct Fila {
   No* cabeca;
   No* fim;
} Fila;


int cpu = 0;
int processos[NUM_PROCESSOS]; // PIDs dos processos
int processo_atual = NUM_PROCESSOS-1; // Índice do processo que está executando
int processo_block;
// int espera[NUM_PROCESSOS] = {0}; //se processo block --> 1
int terminados[NUM_PROCESSOS] = { 0 }; //se processo terminado --> 1
int term = 0;
int flag = 0;
int* PC;
int* espera;
Fila* block;
///////////////////////////////////////////////////////////////////////////////////////////////////
//FUNCOES DE FILA
No* criaNo(int processo) {
   No* no = (No*)malloc(sizeof(No));
   if (!no) {
       perror("Erro ao alocar memoria para o no");
       exit(EXIT_FAILURE);
   }
   no->processo = processo;
   no->next = NULL;
   return no;
}


int vazia(Fila* queue) {
   return queue->cabeca == NULL;
}


Fila* criaFila() {
   Fila* fila = (Fila*)malloc(sizeof(Fila));
   if (!fila) {
       perror("Erro ao alocar memoria para a fila");
       exit(EXIT_FAILURE);
   }
   fila->cabeca = NULL;
   fila->fim = NULL;
   return fila;
}


void insereFila(Fila* fila, int processo) {
   No* no = criaNo(processo);
   if (vazia(fila)) {
       fila->cabeca = no;
       fila->fim = no;
   }
   else {
       fila->fim->next = no;
       fila->fim = no;
   }
}


No* removeFila(Fila* fila) {
   if (vazia(fila)) {
       //printf("Fila esta vazia\n");
       return NULL;
   }
   No* temp = fila->cabeca;
   fila->cabeca = fila->cabeca->next;
   if (fila->cabeca == NULL) {
       fila->fim = NULL;
   }
   return temp;
}


No* veInicio(Fila* fila) {
   if (vazia(fila)) {
       printf("Fila esta vazia\n");
       return NULL;
   }
   return fila->cabeca;
}




void imprimeFila(Fila* fila) {
   if (vazia(fila)) {
       printf("Fila está vazia\n");
       return;
   }
   No* atual = fila->cabeca;
   printf("Fila de processos espera: ");
   while (atual != NULL) {
       //printf("%d ", atual->processo);
       atual = atual->next;
   }
   printf("\n");
}




void liberaFila(Fila* fila) {
   while (!vazia(fila)) {
       No* temp = removeFila(fila);
       free(temp);
   }
   free(fila);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////




void irq0_handler(int sig) {
  
  
   printf("\n\n\nTempo CPU: %d\n", cpu);
   cpu++;
  if(cpu!=1 && !espera[processo_atual] && !terminados[processo_atual] ){
       kill(processos[processo_atual], SIGSTOP); // Pausar o processo atual
       printf("Parando processo %d\n", processo_atual);
   }


   // Avançar para o próximo processo
   processo_atual = (processo_atual + 1) % NUM_PROCESSOS;


   for (int i = 0; i < NUM_PROCESSOS; i++) {
       // printf("oi - %d\n", espera[i]);
       if (espera[i]) {
           espera[i]--;
           //printf("%d --> %d", i, espera[i]);
           if (!espera[i]) {
               //printf("livreee\n");
               kill(getpid(), SIGHUP);
           }
       }
   }




   for (int i = 0; i < NUM_PROCESSOS; i++) {


       if (!espera[processo_atual] && !terminados[processo_atual]) {
           //printf("\nespera proc atua%d\n", espera[processo_atual]);


           printf("Processo %d, PC=%d\n", processo_atual, PC[processo_atual]);//de fora
          
           kill(processos[processo_atual], SIGCONT);   // Continuar o próximo processo
           //  printf("p atual%d\n",processo_atual);
           //printf("pre:%d  ",PC[processo_atual]);
           PC[processo_atual]--;
           //printf("pos:%d/n",PC[processo_atual]);
           break;
       }
      
       if(term == NUM_PROCESSOS){
           printf("Todos processos terminaram!");
       }
      
   }
}




void syscall_handler(int signo) {
   //printf("\n%d\n", id);
   kill(processos[processo_atual],SIGSTOP);
   printf("syscall: Entrada IO\n");
   insereFila(block, processo_atual);
   //imprimeFila(block);
   espera[processo_atual] = 3;
   printf("Parando processo %d (por IO)\n", processo_atual);
  
}




void irq1_handler(int sig) {
   printf("IRQ1: Saida IO\n");
   No* desbloqueado = removeFila(block);
   if (desbloqueado != NULL) {
   int proc_id = desbloqueado->processo;
   printf("Processo %d voltou da operação de IO\n", proc_id);
   kill(processos[proc_id], SIGCONT); // Retoma o processo da fila de bloqueados
   free(desbloqueado);
}


   //kill(processos[processo_atual], SIGCONT);


   //removeFila(block);
}






void processo_funcao(int id) {
   while (PC[id] > 0) {




       sleep(1); // Simula a execução
       if ((PC[processo_atual]) == 5) {
           //printf("%d  %d", id, getpid());
           //syscall_handler(id, getpid());
           kill(getppid(), SIGUSR2);
       }
   }
   terminados[id] = 1;
   term++;
   exit(0); // Termina o processo
}








void intercontroller_sim() {
   while (1) {
       sleep(TIMESLICE); // Emula IRQ0 a cada 1 segundo
       kill(getppid(), SIGUSR1); // Envia sinal para o KernelSim
   }
}




int main() {
   signal(SIGUSR1, irq0_handler); // Registrar o tratador de IRQ0
   signal(SIGHUP, irq1_handler); // Registrar fim do tratador de tempo de io
   signal(SIGUSR2, syscall_handler); // Registrar inicio do tratador de tempo de io
   block = criaFila();
   int segmento = shmget(IPC_PRIVATE, (sizeof(int) * NUM_PROCESSOS), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
   if (segmento == -1) {
       perror("Erro ao alocar memória compartilhada");
       exit(1);
   }


   int segmentoEspera = shmget(IPC_PRIVATE, (sizeof(int) * NUM_PROCESSOS), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
   if (segmentoEspera == -1) {
       perror("Erro ao alocar memória compartilhada");
       exit(1);
   }


   // associa a memória compartilhada ao processo
   PC = (int*)shmat(segmento, NULL, 0); // comparar o retorno com -1
   if (PC == (void*)-1) {
       perror("Erro ao associar a memória compartilhada");
       exit(1);
   }


   for (int i = 0; i < NUM_PROCESSOS; i++) {
       PC[i] = 7;
   }


   espera = (int*)shmat(segmentoEspera, NULL, 0); // comparar o retorno com -1
   if (espera == (void*)-1) {
       perror("Erro ao associar a memória compartilhada");
       exit(1);
   }




   for (int i = 0; i < NUM_PROCESSOS; i++) {
       espera[i] = 0;
   }


   // Criar processos de aplicação
   for (int i = 0; i < NUM_PROCESSOS; i++) {
       if ((processos[i] = fork()) == 0) {
           // Código do processo de aplicação
           processo_funcao(i);
          
       }
   }






   // Pausar todos os processos no início
   for (int i = 0; i < NUM_PROCESSOS; i++) {
       kill(processos[i], SIGSTOP);
   }


   // Criar o controlador de interrupções (InterControllerSim)
   int pidInter = fork();
   if (pidInter == 0) {
       intercontroller_sim();
       exit(0);
   }




   // Inicializar o primeiro processo
   kill(processos[processo_atual], SIGCONT);




   // KernelSim espera a finalização de todos os processos
   for (int i = 0; i < NUM_PROCESSOS; i++) {
       wait(NULL);
   }




   kill(pidInter, SIGKILL);
    liberaFila(block);

   shmdt(PC);
   shmdt(espera);
  
   shmctl(segmento, IPC_RMID, NULL);
   shmctl(segmentoEspera, IPC_RMID, NULL);
  
 return 0;
}
