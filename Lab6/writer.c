#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include "shared.h"

int main() {
// Criação de semáforos
sem_t *sem_writer = sem_open("/sem_writer", O_CREAT, 0666, 1);
sem_t *sem_reader = sem_open("/sem_reader", O_CREAT, 0666, 0);

// Criação da memória compartilhada
int shm_id = shmget(SHM_KEY, sizeof(SharedMemory), IPC_CREAT | 0666);
SharedMemory *shared_mem = (SharedMemory *)shmat(shm_id, NULL, 0);

for (int i = 1; i <= NUM_MESSAGES; i++) {
// Espera o semáforo do escritor estar disponível
sem_wait(sem_writer);

// Escreve a mensagem na memória compartilhada
snprintf(shared_mem->message, MAX_MSG_LENGTH, "mensagem %d", i);
shared_mem->message_number = i;
printf("Escritor: %s\n", shared_mem->message);

// Sinaliza para o processo leitor que a mensagem está pronta
sem_post(sem_reader);
}

// Limpeza
sem_close(sem_writer);
sem_close(sem_reader);
shmdt(shared_mem);
sem_unlink("/sem_writer");
sem_unlink("/sem_reader");

return 0;
}
