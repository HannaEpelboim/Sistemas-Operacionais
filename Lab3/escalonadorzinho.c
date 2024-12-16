#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>


int pid1,pid2;
int tempo = 0, troca = 0;


void handler(int signo){
    if(tempo == 15){
        printf("Acabou o tempo! %d  segundos decorridos.\n",tempo);
        kill(pid1,SIGKILL);
        kill(pid2,SIGKILL);
        exit(0);
    }

    if(troca == 0){
        kill(pid1,SIGSTOP);
        kill(pid2,SIGCONT);
    }
    else{
        kill(pid2,SIGSTOP);
        kill(pid1,SIGCONT);
    }

    troca = 1-troca;
    tempo++;
    alarm(1);
}

int main (int argc, char *argv[]){

    signal(SIGALRM, handler);

    if ((pid1 = fork()) == 0)
    {
        execvp(argv[1], argv);
    }
    kill(pid1,SIGSTOP);

    if ((pid2 = fork()) == 0)
    {
        execvp(argv[2], argv);
    }
    kill(pid2,SIGSTOP);

    alarm(1);

    while(1){
        pause();
    }

    return 0;
}