/*
Érica Regnier - 2211893
Hanna Epelboim Assunção - 2310289
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_FRAMES 1024

typedef struct {
  unsigned int pgNum;
  bool referenciada;
  bool modificada;
  unsigned int ultAcesso;
  unsigned int carregada;
} Frame;

Frame frames[MAX_FRAMES];

unsigned int clockTime = 0;
unsigned int framesNum = 0;
unsigned int pf = 0;
unsigned int pwSujas = 0;
unsigned int ptrSegundaChance = 0;
static unsigned int nruCount = 0;

int substituiNRU(unsigned int framesNum) {

  for (unsigned int i = 0; i < framesNum; i++) {
    if (frames[i].pgNum == -1) {
      return i;
    }
  }

  for (unsigned int i = 0; i < framesNum; i++) {
    if (!frames[i].referenciada && !frames[i].modificada) {
      return i;
    }
  }
  for (unsigned int i = 0; i < framesNum; i++) {
    if (!frames[i].referenciada && frames[i].modificada) {
      return i;
    }
  }
  for (unsigned int i = 0; i < framesNum; i++) {
    if (frames[i].referenciada && !frames[i].modificada) {
      return i;
    }
  }
  for (unsigned int i = 0; i < framesNum; i++) {
    if (frames[i].referenciada && frames[i].modificada) {
      return i;
    }
  }
  // return rand() % framesNum;
}

int substituiLRU(unsigned int framesNum) {
  unsigned int maisVelho = clockTime;
  int substituido = 0;
  for (unsigned int i = 0; i < framesNum; i++) {
    if (frames[i].ultAcesso < maisVelho) {
      maisVelho = frames[i].ultAcesso;
      substituido = i;
    }
  }
  return substituido;
}

int substituiClock(unsigned int framesNum) {

  while (1) {
    if (!frames[ptrSegundaChance].referenciada) {
      int substituido = ptrSegundaChance;
      ptrSegundaChance = (ptrSegundaChance + 1) % framesNum;
      return substituido;
    } else {
      frames[ptrSegundaChance].referenciada = 0;
      ptrSegundaChance = (ptrSegundaChance + 1) % framesNum;
    }
  }
}

int substituiSecondChance(unsigned int framesNum) {
  unsigned int maisVelho = clockTime;
  int substituido = 0;
  while (1) {
    for (unsigned int i = 0; i < framesNum; i++) {
      if (frames[i].carregada < maisVelho) {
        maisVelho = frames[i].carregada;
        substituido = i;
      }
    }
    if (!frames[substituido].referenciada) {
      return substituido;
    } else {
      frames[substituido].referenciada = 0;
      frames[substituido].carregada = clockTime;
      frames[substituido].ultAcesso = clockTime;
    }
  }
}

int substituiOtimo(unsigned int framesNum, unsigned int *referencias,
                   unsigned int totalRefs, unsigned int posAtual) {
  unsigned int maisDistante = 0;
  int substituido = -1;

  for (unsigned int i = 0; i < framesNum; i++) {
    if (frames[i].pgNum == -1) {
      // printf("%d pgNA\n", frames[i].pgNum);
      return i;
    }
  }

  for (unsigned int i = 0; i < framesNum; i++) {
    unsigned int pgNumAtual = frames[i].pgNum;
    bool encontrada = false;
    unsigned int distancia = 0;
    for (unsigned int j = posAtual + 1; j < totalRefs; j++) {
      if (referencias[j] == pgNumAtual) {
        encontrada = true;
        distancia = j - posAtual;
        break;
      }
    }
    if (!encontrada) {
      return i;
    }
    if (distancia > maisDistante) {
      maisDistante = distancia;
      substituido = i;
    }
  }
  return substituido;
}

int buscaPgFrame(unsigned int pgNum, unsigned int framesNum) {
  for (unsigned int i = 0; i < framesNum; i++) {
    if (frames[i].pgNum == pgNum) {
      return i;
    }
  }
  return -1;
}

void inicializa(unsigned int framesNum) {
  for (unsigned int i = 0; i < framesNum; i++) {
    frames[i].pgNum = -1;
    frames[i].referenciada = false;
    frames[i].modificada = false;
    frames[i].ultAcesso = 0;
    frames[i].carregada = 0;
  }
}

void simulador(const char *alg, const char *arqNome, unsigned int pgTam_kb,
               unsigned int memTam_mb) {
  printf("Executando o simulador...\n");
  printf("Arquivo de entrada: %s\n", arqNome);
  printf("Tamanho da memoria fisica: %dMB\n", memTam_mb);
  printf("Tamanho das páginas: %dKB\n", pgTam_kb);
  printf("Algoritmo de substituição: %s\n", alg);

  FILE *entrada = fopen(arqNome, "r");
  if (!entrada) {
    perror("Erro ao abrir o arquivo de entrada");
    return;
  }

  unsigned int totalRefs = 0;
  unsigned int pgTam = pgTam_kb * 1024;
  unsigned int memTam = memTam_mb * 1024 * 1024;
  unsigned int addr;
  char rw;

  framesNum = memTam / pgTam;
  inicializa(framesNum);
  unsigned int addrCount = 0;

  while (fscanf(entrada, "%x %c", &addr, &rw) != EOF) {
    addrCount++;
  }
  unsigned int *referencias =
      (unsigned int *)malloc(sizeof(unsigned int) * addrCount);
  rewind(entrada);

  unsigned int idx = 0;
  char rwTmp;
  while (fscanf(entrada, "%x %c", &addr, &rwTmp) != EOF) {
    referencias[idx++] = addr / pgTam;
  }

  // Agora, vamos rodar a simulação
  rewind(entrada);

  int posAtual = -1;
  while (fscanf(entrada, "%x %c", &addr, &rw) != EOF) {
    posAtual++;
    clockTime++;
    unsigned int pgNum = addr / pgTam;
    int frameIndex = buscaPgFrame(pgNum, framesNum);
    if (strcmp(alg, "NRU") == 0) {
      if (nruCount == 4) {
        for (unsigned int i = 0; i < framesNum; i++) {
          frames[i].referenciada = 0;
        }
        nruCount = 0;
      } else {
        nruCount++;
      }
    }
    if (frameIndex == -1) {
      pf++;
      int substituido;

      if (strcmp(alg, "NRU") == 0) {
        substituido = substituiNRU(framesNum);
      } else if (strcmp(alg, "LRU") == 0) {
        substituido = substituiLRU(framesNum);
      } else if (strcmp(alg, "SecondChance") == 0) {
        substituido = substituiSecondChance(framesNum);
      } else if (strcmp(alg, "Clock") == 0) {
        substituido = substituiClock(framesNum);
      } else if (strcmp(alg, "Optimal") == 0) {
        substituido =
            substituiOtimo(framesNum, referencias, addrCount, posAtual);
      } else {
        printf("ERRO: Algoritmo de substituição inválido.\n");
      }

      if (frames[substituido].modificada) { // alteracao do disco
        pwSujas++;
        frames[substituido].modificada = false;
      }

      // substituicao
      frames[substituido].pgNum = pgNum;
      frames[substituido].referenciada = true;
      if (rw == 'W') {
        frames[substituido].modificada = true;
      }
      frames[substituido].ultAcesso = clockTime;
      frames[substituido].carregada = clockTime;

    }

    else {
      frames[frameIndex].referenciada = true;
      if (rw == 'W') {
        frames[frameIndex].modificada = true;
      }
      frames[frameIndex].ultAcesso = clockTime;
    }
  }
  fclose(entrada);

  /*
    for(int i = 0; i<framesNum; i++){
        if(frames[i].modificada){
            pwSujas++;
        }
    }
  */

  printf("Relatório de Simulação:\n");
  printf("Número de Page Faults: %u\n", pf);
  printf("Número de Páginas Sujas Escritas: %u\n", pwSujas);
}

int main(int argc, char *argv[]) {
  if (argc != 5) {
    printf("Uso: %s <algoritmo> <arquivo.log> <tamanho_pagina_kb> "
           "<tamanho_memoria_mb>\n",
           argv[0]);
    return 1;
  }

  const char *alg = argv[1];
  const char *arqNome = argv[2];
  unsigned int pgTam_kb = atoi(argv[3]);
  unsigned int memTam_mb = atoi(argv[4]);

  simulador(alg, arqNome, pgTam_kb, memTam_mb);
  return 0;
}
