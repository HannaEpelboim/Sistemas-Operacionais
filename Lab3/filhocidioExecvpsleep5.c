#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#define EVER ;;

void childhandler(int signo);
int delay;
int main (int argc, char *argv[])
{
pid_t pid;
signal(SIGCHLD, childhandler);
if ((pid = fork()) < 0)
{
fprintf(stderr, "Erro ao criar filho\n");
exit(-1);
}
if (pid == 0) /* child */
execvp(argv[2], argv); /* ou sleep(3); ou, no próximo exercício, execvp(argv[2], argv);*/
else /* parent */
{
sscanf(argv[1], "%d", &delay); /* read delay from command line */
sleep(delay);
printf("Program %s exceeded limit of %d seconds!\n", argv[2], delay);
kill(pid, SIGKILL);
}
return 0;
}
void childhandler(int signo) /* Executed if child dies before parent */
{
int status;
pid_t pid = wait(&status);
printf("Child %d terminated within %d seconds com estado %d.\n", pid, delay, status);
exit(0);
}