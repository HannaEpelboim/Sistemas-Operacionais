
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void executar_comando(char **args) {
    
    char comando_path[1024];
    snprintf(comando_path, sizeof(comando_path), "%s", args[0]);
    if (execvp(comando_path, args) == -1) {
        perror("Erro ao executar o comando");
    }
    exit(EXIT_FAILURE);
}


int main(void) {
	char comando[1024];
	char *args[64];
	char *token;
	char **arg_ptr;
    
	while (1) {
    	printf("$minhashell> ");
    	fgets(comando, sizeof(comando), stdin);

    	comando[strlen(comando) - 1] = '\0';
    	arg_ptr = args;  
    	token = strtok(comando, " ");

    	while (token != NULL) {
        	*arg_ptr = token;  
        	arg_ptr++;     	 
        	token = strtok(NULL, " ");
    	}
    	*arg_ptr = NULL;


	    pid_t pid = fork();
        if (pid < 0) {
    	    perror("Erro ao criar o processo");
	    } else if(pid > 0) { // Processo pai
    	    wait(NULL); // Espera o processo filho terminar
	    } else if(pid == 0){
            if (strcmp(args[0], "meuecho") == 0) {
                args[0] = "./meuecho";
                executar_comando(args);  
            } else if (strcmp(args[0], "meucat") == 0) {
                args[0] = "./meucat";
                executar_comando(args);  
            } else if(strcmp(args[0], "cat") == 0){
                args[0] = "/usr/bin/cat";
                executar_comando(args);  
            }else if(strcmp(args[0], "echo") == 0){
                args[0] = "/usr/bin/echo";
                executar_comando(args);  
            }else if(strcmp(args[0], "ls") == 0){
                args[0] = "/usr/bin/ls";
                executar_comando(args);  
            }
        }
	}

	return 0;
}

