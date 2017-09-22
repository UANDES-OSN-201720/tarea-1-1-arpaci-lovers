#include "pipe_handler.h"

extern pthread_mutex_t pipes_m;
extern pthread_mutex_t total_branches_m;
extern pthread_mutex_t branches_m;
extern pthread_mutex_t total_accounts_m;
extern pthread_mutex_t accounts_codes_m;

void* main_office_comunication(void* args)
{
	printf("en el pipe handler\n");
  char* readbuffer = malloc(sizeof(char)*80);
  moc_args* m = (moc_args*) args;
  while (true)
  {
  	pthread_mutex_lock(&total_branches_m);
    for (int i=0; i<*(m->total_branches); i++)
    {
    
      int position = i*2+1;
      
      pthread_mutex_lock(&pipes_m);
      read(*(m->pipes)[position][0], readbuffer, strlen(readbuffer));
      pthread_mutex_unlock(&pipes_m);
      
      if (readbuffer[0] == 'n') continue;
      
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
        
        pthread_mutex_lock(&total_accounts_m);
        if (random >= *(m->total_accounts))
        {
          state[0] = 'i';
          destiny = malloc(sizeof(char)*15);
          sprintf(destiny, "xxx-xxx-xxxxxx");
        } 
        else
        {
        	pthread_mutex_lock(&accounts_codes_m);
          destiny = malloc(strlen(*(m->accounts_codes)[random]));
          destiny = *(m->accounts_codes)[random];
          pthread_mutex_unlock(&accounts_codes_m);
          if (destiny[0] == 'x')
          {
            state[0] = 'i';
          }
        }
        pthread_mutex_unlock(&total_accounts_m);
        
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
    pthread_mutex_unlock(&total_branches_m);
  }
  
  return NULL;
}
