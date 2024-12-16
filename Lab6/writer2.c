
#include "shared2.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
  // Criação dos semáforos
  sem_t *sem_empty = sem_open(SEM_EMPTY_KEY, O_CREAT, 0666, BUFFER_SIZE); // Controle de espaços vazios
  if (sem_empty == SEM_FAILED) {
    perror("Erro ao abrir sem_empty");
    exit(1);
  }
  sem_t *sem_full = sem_open(SEM_FULL_KEY, O_CREAT, 0666, 0); // Controle de mensagens presentes
  if (sem_full == SEM_FAILED) {
    perror("Erro ao abrir sem_full");
    exit(1);
}
  sem_t *sem_mutex = sem_open(SEM_MUTEX_KEY, O_CREAT, 0666, 1); // Exclusão mútua
if (sem_mutex == SEM_FAILED) {
    perror("Erro ao abrir sem_mutex");
    exit(1);
}
  // Criação da memória compartilhada
  int shm_id = shmget(5789, sizeof(SharedBuffer), IPC_CREAT | 0666);
  
    if (shm_id == -1) {
        perror("Erro ao criar memória compartilhada");
        exit(1);
    }
  SharedBuffer *buffer = (SharedBuffer *)shmat(shm_id, NULL, 0);
 

  buffer->in = 0;

  for (int i = 1; i <= 128; i++) {
    // Gera a mensagem
    char msg[MAX_MSG_LENGTH];
    snprintf(msg, MAX_MSG_LENGTH, "mensagem %d", i);

    // Espera até haver espaço no buffer
    sem_wait(sem_empty);
    sem_wait(sem_mutex);

    // Insere a mensagem no buffer
    strcpy(buffer->messages[buffer->in], msg);
    printf("Escritor: Enviou %s\n", msg);
    buffer->in = (buffer->in + 1) % BUFFER_SIZE;

    sem_post(sem_mutex);
    sem_post(sem_full);

    sleep(1); // Simula atraso para visualizar o funcionamento
  }

  // Limpeza
  sem_close(sem_empty);
  sem_close(sem_full);
  sem_close(sem_mutex);
  shmdt(buffer);
  sem_unlink(SEM_EMPTY_KEY);
  sem_unlink(SEM_FULL_KEY);
  sem_unlink(SEM_MUTEX_KEY);
  return 0;
}
