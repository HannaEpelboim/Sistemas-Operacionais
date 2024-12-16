#ifndef SHARED_H
#define SHARED_H

#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define MAX_MSG_LENGTH 16
#define NUM_MESSAGES 128

// Estrutura para a memória compartilhada
typedef struct {
char message[MAX_MSG_LENGTH];
int message_number;
} SharedMemory;

#define SHM_KEY 1234 // Chave para a memória compartilhada
#define SEM_WRITER_KEY 5678 // Chave para o semáforo do escritor
#define SEM_READER_KEY 91011 // Chave para o semáforo do leitor

#endif // SHARED_H
