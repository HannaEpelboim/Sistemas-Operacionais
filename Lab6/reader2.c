
#include "shared2.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
  // Abertura dos semáforos
  sem_t *sem_empty = sem_open(SEM_EMPTY_KEY, 0);
  if (sem_empty == SEM_FAILED) {
    perror("Erro ao abrir sem_empty");
    exit(1);
  }
  sem_t *sem_full = sem_open(SEM_FULL_KEY, 0);
  if (sem_full == SEM_FAILED) {
    perror("Erro ao abrir sem_full");
    exit(1);
}
  sem_t *sem_mutex = sem_open(SEM_MUTEX_KEY, 0);
  if (sem_mutex == SEM_FAILED) {
    perror("Erro ao abrir sem_mutex");
    exit(1);
}

  // Acesso à memória compartilhada
  int shm_id = shmget(5789, sizeof(SharedBuffer), 0666);
  SharedBuffer *buffer = (SharedBuffer *)shmat(shm_id, NULL, 0);

  buffer->out = 0;

  for (int i = 1; i <= 128; i++) {
    // Espera até haver uma mensagem no buffer
    sem_wait(sem_full);
    sem_wait(sem_mutex);

    // Lê a mensagem do buffer
    printf("Leitor: Recebeu %s\n", buffer->messages[buffer->out]);
    buffer->out = (buffer->out + 1) % BUFFER_SIZE;

    sem_post(sem_mutex);
    sem_post(sem_empty);

    sleep(2); // Simula atraso para visualizar o funcionamento
  }

  // Limpeza
  sem_close(sem_empty);
  sem_close(sem_full);
  sem_close(sem_mutex);
  shmdt(buffer);
  return 0;
}
