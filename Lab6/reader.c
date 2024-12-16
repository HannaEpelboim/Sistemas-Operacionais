#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include "shared.h"

int main() {
// Abertura de semáforos
sem_t *sem_writer = sem_open("/sem_writer", 0);
sem_t *sem_reader = sem_open("/sem_reader", 0);

// Acesso à memória compartilhada
int shm_id = shmget(SHM_KEY, sizeof(SharedMemory), 0666);
SharedMemory *shared_mem = (SharedMemory *)shmat(shm_id, NULL, 0);

for (int i = 1; i <= NUM_MESSAGES; i++) {
// Espera o semáforo do leitor
sem_wait(sem_reader);

// Lê a mensagem da memória compartilhada
printf("Leitor: %s\n", shared_mem->message);

// Sinaliza para o escritor que a mensagem foi lida
sem_post(sem_writer);
}

// Limpeza
sem_close(sem_writer);
sem_close(sem_reader);
shmdt(shared_mem);
return 0;
}
