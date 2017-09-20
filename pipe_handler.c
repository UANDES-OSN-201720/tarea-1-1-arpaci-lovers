#include "pipe_handler.h"

void* main_office_comunication(void* args)
{
  char* readbuffer = malloc(sizeof(char)*80);
  moc_args* m = (moc_args*) args;
  while (true)
  {
    for (int i=0; i<*(m->total_branches); i++)
    {
      int position = i*2+1;
      
      read(*(m->pipes)[position][0], readbuffer, strlen(readbuffer));
      
      char* operation = reach_input(&readbuffer, 1);
      char* origin = reach_input(&readbuffer, 2);
      char type = readbuffer[0];
      char* destiny;
      char* state = reach_input(&readbuffer, 5);
      char* ammount = reach_input(&readbuffer, 4);
      
      if (type == 'i')
      {
        char* random_account = reach_input(&readbuffer, 3);
        char* pointer;
        int random = strtol(random_account, &pointer, 10);
        
        if (random >= *(m->total_accounts))
        {
          state[0] = 'i';
          destiny = malloc(sizeof(char)*15);
          sprintf(destiny, "xxx-xxx-xxxxxx");
        } 
        else
        {
          destiny = malloc(strlen(*(m->accounts)[random]));
          destiny = *(m->accounts)[random];
          if (destiny[0] == 'x')
          {
            state[0] = 'i';
          }
        }
        
        char* message = malloc(9+strlen(origin)+strlen(destiny)+strlen(ammount));
        sprintf(message, "%c %s %s %s %s %s", type, operation, origin, destiny, ammount, state);
        
        for (int j=0; j<*(m->total_accounts);j++)
        {
          write(*(m->pipes)[j*2][1], message, strlen(message));
        }
      }
    
      if (type == 'v')
      {
        write(*(m->pipes)[position][1], readbuffer, strlen(readbuffer));
      }
    }
  }
  
  return NULL;
}
