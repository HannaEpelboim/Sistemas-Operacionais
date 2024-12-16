#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void handler(int sig){
    printf("Erro! Divisão por zero!\n");
    exit(1);
}

int main(void){
    int numero1,numero2;
    
    //signal(SIGFPE,handler);
    
    printf("Digite o primeiro número inteiro: ");
    scanf("%d", &numero1);

    printf("Digite o segundo número inteiro: ");
    scanf("%d", &numero2);
    
    printf("Soma: %d\nSubtração: %d\nMultiplicação: %d\nDivisão: %d\n", numero1+numero2, numero1-numero2, numero1*numero2, numero1/numero2);

    return 0;
}
