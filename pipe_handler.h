#include "header.h"

typedef struct
{
  int total_branches;
  int*** pipes;
} mac_args;

void* main_office_comunication(void* args)
{
  char readbuffer[80];
  mac_args* m = (mac_args*) args;
  while (true)
  {
    for (int i=0; i<m->total_branches; i++)
    {
      int position = (m->total_branches)*2 + 1;
      read(m->(*pipes)[position][0], readbuffer, strlen(
    }
  }
  
  return NULL;
}
