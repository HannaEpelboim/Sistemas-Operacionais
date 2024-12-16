#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
int main (int argc, char *argv[])
{
    int segmento;
    char mensagem[100], *p;

    printf("Digite sua mensagem: ");
    fgets(mensagem,sizeof(mensagem),stdin);

    // aloca a memória compartilhada
    segmento = shmget (7000, sizeof (mensagem), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    if (segmento == -1) {
        perror("Erro ao alocar memória compartilhada");
        exit(1);
    }

    // associa a memória compartilhada ao processo
    p = (char *) shmat (segmento, 0, 0); // comparar o retorno com -1
    if (p == (void *)-1) {
        perror("Erro ao associar a memória compartilhada");
        exit(1);

    }
    
    mensagem[strcspn(mensagem, "\n")] = '\0';
    strncpy(p, mensagem, sizeof(mensagem));


    shmdt (p);

    return 0;
}