#ifndef PIPE_HANDLER_H
#define PIPE_HANDLER_H
#include "header.h"
#include "funcionesAlf.h"


typedef struct
{
  int total_branches;
  int** pipes;
  char** accounts_codes;
  int total_accounts;
} moc_args;

void* main_office_comunication(void* args);

#endif
