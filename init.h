#ifndef FUNCIONES_ALF
#define FUNCIONES_ALF

#include "funcionesAlf.h"

pthread_mutex_t pipes_m;
pthread_mutex_t total_branches_m;
pthread_mutex_t branches_m;
pthread_mutex_t total_accounts_m;
pthread_mutex_t accounts_codes_m;

typedef struct
{
  char* commandBuffer;
  int bankId;
  int* total_branches;
  int** branches;
  int*** pipes;
  int* total_accounts;
  char*** accounts_codes;
} init_args;

void* init(void* args);  

void cpy_str(char *tar, const char *ori);

#endif
