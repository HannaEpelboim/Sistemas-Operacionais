
// shared.h
#ifndef SHARED_H
#define SHARED_H

#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MAX_MSG_LENGTH 16
#define BUFFER_SIZE 8

// Estrutura do buffer circular na memória compartilhada
typedef struct {
    char messages[BUFFER_SIZE][MAX_MSG_LENGTH];
    int in;    // Índice de escrita
    int out;   // Índice de leitura
} SharedBuffer;

#define SHM_KEY 1234
#define SEM_EMPTY_KEY "/sem_empty"
#define SEM_FULL_KEY "/sem_full"
#define SEM_MUTEX_KEY "/sem_mutex"

#endif // SHARED_H
