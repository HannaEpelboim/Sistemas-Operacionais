#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void){
    int segmento;
    char *p;

    segmento = shmget (7000,100,0);
    if (segmento == -1) {
        perror("Erro ao alocar memória compartilhada");
        exit(1);
    }

    p = (char *) shmat (segmento, 0, 0); 
    if (p == (void *)-1) {
        perror("Erro ao associar a memória compartilhada");
        exit(1);

    }

    printf("Sua mensagem foi: %s",p);

    shmdt(p);

    return 0;    
}