#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void handler(int sig){
    printf("Erro! Divisão por zero!");
    exit(1);
}

int main(void){
    float numero1,numero2;
    
    signal(SIGFPE,handler);
    
    printf("Digite o primeiro número: ");
    scanf("%f", &numero1);

    printf("Digite o segundo número: ");
    scanf("%f", &numero2);
    
    printf("Soma: %.2f\nSubtração: %.2f\nMultiplicação: %.2f\nDivisão: %.2f\n", numero1+numero2, numero1-numero2, numero1*numero2, numero1/numero2);

    return 0;
}
