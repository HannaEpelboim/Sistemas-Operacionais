#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void handler(int signo) { 
printf("Sinal recebido e capturado: %d\n", signo); 
}

int main(void) {
  signal(SIGKILL, handler);
  signal(SIGUSR1, handler);

  printf("%d\n",getpid());

  while (1) {
    
  }

  return 0;
}
