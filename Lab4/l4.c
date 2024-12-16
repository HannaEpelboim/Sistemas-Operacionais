#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h> 
int pid1, pid2, pid3;

void handler(int signo){
    kill(pid1,SIGKILL);
    kill(pid2,SIGKILL);
    kill(pid3,SIGKILL);
    exit(0);
}

int main(int argc, char** argv){

    signal(SIGALRM, handler);

    struct timeval tempoAtual;
    int mod;
    short flag0 = 1,flag1 = 1, flag2 = 1, flag3 = 1;
    pid1 = fork();
    if(pid1<0){
        perror("Erro no fork()");
        exit(1);       
    }
    else if(pid1 == 0){
        execvp(argv[1],argv);
    }
    kill(pid1,SIGSTOP);

    pid2 = fork();
    if(pid2<0){
        perror("Erro no fork()");
        exit(1);       
    }
    else if(pid2 == 0){
        execvp(argv[2],argv);
    }
    kill(pid2,SIGSTOP);

    pid3 = fork();
    if(pid3<0){
        perror("Erro no fork()");
        exit(1);       
    }
    else if(pid3 == 0){
        execvp(argv[3],argv);
    }
    kill(pid3,SIGSTOP);

    while(flag0){
        gettimeofday(&tempoAtual,NULL);
        mod = (tempoAtual.tv_sec)%60;
        if(mod == 0){
            flag0 = 0;
            alarm(120); //dois min
            while(1){
                gettimeofday(&tempoAtual,NULL);
                mod = (tempoAtual.tv_sec)%60;
                if(mod == 0){
                    if(flag3){
                        kill(pid2,SIGSTOP);
                        kill(pid3,SIGCONT);
                        printf("Tempo: %d - Iniciando filho 3\n",mod);
                        flag3 = 0;
                        flag2 = 1;
                    }
                }
                else if(mod == 25){
                    if(flag3){    
                        kill(pid1,SIGSTOP);
                        kill(pid3,SIGCONT);
                        printf("Tempo: %d - Iniciando filho 3\n",mod);
                        flag3 = 0;
                        flag1 = 1;
                    }
                }
                else if(mod == 45){
                    if(flag2){
                        kill(pid3,SIGSTOP);
                        kill(pid2,SIGCONT);
                        printf("Tempo: %d - Iniciando filho 2\n",mod);
                        flag2 = 0;
                        flag3 = 1;
                    }
                }
                else if(mod == 5){
                    if(flag1){
                        kill(pid3,SIGSTOP);
                        kill(pid1,SIGCONT);
                        printf("Tempo: %d - Iniciando filho 1\n",mod);
                        flag1 = 0;
                        flag3 = 1;
                    }
                }
            }
        }
    }   
    return 0;
}