#include "funcionesAlf.h"

typedef struct
{
  char* commandBuffer;
  int bankId;
  int* total_branches;
  int** branches;
  int*** pipes;
  int* total_accounts;
} init_args;

void* init(void* args)
{
  init_args* ia = (init_args*) args;
  char* commandBuffer = ia->commandBuffer;
  int bankId = ia->bankId;
  int* total_branches = ia->total_branches;
  int** branches = ia->branches;
  int*** pipes = ia->pipes;
  int* total_accounts = ia->total_accounts;
  
  int argc = 0;
  int acc_digit_counter = 0;
  char* str_accounts = malloc(sizeof(char));
  char char_terminals;
  printf("commandBuffer: %s\n", commandBuffer);
  for (int i=strlen("init"); i<strlen(commandBuffer); i++)
  {
    if (commandBuffer[i] == ' ')
    {
      argc++;
      continue;
    }
    //Numero de cuentas
    if (argc == 1)
    {
      acc_digit_counter++;
      str_accounts = realloc(str_accounts, acc_digit_counter*sizeof(char));
      str_accounts[acc_digit_counter-1] = commandBuffer[i];
    }
    //Numero de terminales
    if (argc == 2)
    {
      char_terminals = commandBuffer[i];
    }
  }
  
  pthread_mutex_lock(&total_accounts_m);
  *total_accounts += atoi(str_accounts);
  pthread_mutex_unlock(&total_accounts_m);
  
  pthread_mutex_lock(&total_branches_m);
  int position = *total_branches;
  pthread_mutex_unlock(&total_branches_m);
  
  pthread_mutex_lock(&pipes_m);
  
  *pipes = realloc(*pipes, sizeof(int*)*(position+1)*2);
  (*pipes)[(position)*2] = malloc(sizeof(int)*2);
  (*pipes)[(position)*2+1] = malloc(sizeof(int)*2);
  pipe(*pipes[(position)*2]);
  pipe(*pipes[(position)*2+1]);
 
  char message[512];
  sprintf(message, "%s %c", str_accounts, char_terminals);
  
  write((*pipes)[position*2][1], message, strlen(message));
  
  pthread_mutex_unlock(&pipes_m);
  
  free(str_accounts);
  
  pthread_mutex_lock(&total_branches_m);
  (*total_branches)++;
  pthread_mutex_unlock(&total_branches_m);

  
  pid_t pid_branch = fork();
  
  if (pid_branch > 0) //Proceso Matriz
  {
    printf("En el padre %d con hijo %d\n", bankId, pid_branch);
  
  pthread_mutex_lock(&branches_m);
  pthread_mutex_lock(&total_branches_m);
  
  *branches = realloc(*branches, sizeof(pid_t)*((*total_branches)+1));
  *branches[(*total_branches)] = pid_branch;
  
  pthread_mutex_unlock(&total_branches_m);
  pthread_mutex_unlock(&branches_m);
  
  }
  if (!pid_branch) //Proceso Sucursal
  {
    printf("En el hijo %d\n", getpid());
    
    char readbuffer[80]; // buffer para lectura desde pipe
    
    pthread_mutex_lock(&total_branches_m);
    int branch_position = *total_branches - 1;
    pthread_mutex_unlock(&total_branches_m);
    
    pthread_mutex_t total_transactions_m = PTHREAD_MUTEX_INITIALIZER;
    int total_transactions = 0;
    
    pthread_mutex_t transactions_m = PTHREAD_MUTEX_INITIALIZER;
    transaction* transactions = malloc(sizeof(transaction));
 
    read((*pipes)[branch_position][0], readbuffer, sizeof(readbuffer)); 
    
    argc = 0;
    int counter = 0;
    for (int i=0; i<strlen(readbuffer); i++)
    {
      if (readbuffer[i] == 0)
      {
        argc++;
        continue;
      }
      if (argc == 0)
      {
        counter++;
        str_accounts = realloc(str_accounts,sizeof(char)*counter);
        str_accounts[counter-1] = readbuffer[i];
      }
      if (argc == 1)
      {
        char_terminals = readbuffer[i];
      }
    }
    
    int n_accounts = atoi(str_accounts);
    int n_terminals = char_terminals - '0';
    
    printf("cuentas: %d | terminales: %d\n", n_accounts, n_terminals);
    
    //pthread_mutex_t accounts_m = PTHREAD_MUTEX_INITIALIZER;
    account* accounts = malloc(sizeof(account)*n_accounts);

    //void crear_cuentas(int n_cuentas, pid_t bancid, pid_t sucid, cuenta** cuentas)
    //pthread_mutex_lock(&accounts_m);
    create_accounts(n_accounts, bankId, getpid(), &accounts);
    //pthread_mutex_unlock(&accounts_m);
  }
  
  return NULL;
}
