#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main(void){
    int vetor[10] = {0};
    int pid;
    pid = fork();
    if(pid<0){
    	perror("Erro ao criar o processo");
    }
    else if(pid>0){ //pai
        for(int i = 0; i < 10; i++){
            vetor[i] += 1;
        }        
        printf("[");
        for(int i = 0; i < 10; i++){
           printf("%d ",vetor[i]);
        }        
        printf("]\n");

        waitpid(-1,NULL,0);
        exit(0);
    }
    else{ //filho
        for(int i = 0; i < 10; i++){
            vetor[i] -= 1;
        }        
        printf("[");
        for(int i = 0; i < 10; i++){
           printf("%d ",vetor[i]);
        }        
        printf("]\n");
        exit(0);
    }
}