#include "init.h"
#include "funcionesAlf.h"
#include "pipe_handler.h"

pthread_mutex_t pipes_m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t total_branches_m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t branches_m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t total_accounts_m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t accounts_codes_m = PTHREAD_MUTEX_INITIALIZER;

void* init(void* args)
{
  init_args* ia = (init_args*) args;
  char* commandBuffer = ia->commandBuffer;
  int bankId = ia->bankId;
  int* total_branches = ia->total_branches;
  int** branches = ia->branches;
  int*** pipes = ia->pipes;
  int* total_accounts = ia->total_accounts;
  char*** accounts_codes = ia->accounts_codes;
  
  char* str_accounts = reach_input(&commandBuffer, 1);
  char* char_terminals = reach_input(&commandBuffer, 2);

  char *tmp_ptr; //Puntero para la conversion de string a int (strtol)
  
  pthread_mutex_lock(&total_accounts_m);
  *total_accounts += strtol(str_accounts, &tmp_ptr, 10);
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
  sprintf(message, "%s %s", str_accounts, char_terminals);
  
  write((*pipes)[position*2][1], message, strlen(message));
  
  pthread_mutex_unlock(&pipes_m);
  
  //free(str_accounts);
  
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
		
		char* branch_accounts = reach_input(&commandBuffer, 1);
		char* p;
		int ba = strtol(branch_accounts, &p, 10);
		
		pthread_mutex_lock(&accounts_codes_m);
		*accounts_codes = realloc(*accounts_codes, sizeof(*accounts_codes)+ba);
		pthread_mutex_unlock(&accounts_codes_m);
		
		int branchId = pid_branch % 1000;
		char code[6];
		
		for (int i=0; i<ba; i++)
		{
			for (int j=0; j<6; j++)
			{
				code[j] = (i/(int)pow(10,5-j))%10 +'0';
			}
			
			char* new_account = malloc(sizeof(char)*15);
			sprintf(new_account, "000%d-%d-%s", bankId, branchId, code);
			
			pthread_mutex_lock(&accounts_codes_m);
			(*accounts_codes)[(*total_accounts)+i] = malloc(strlen(new_account));
			(*accounts_codes)[(*total_accounts)+i] = new_account;
			pthread_mutex_unlock(&accounts_codes_m);
			
		}
  
  
  }
  if (!pid_branch) //Proceso Sucursal
  {
    printf("En el hijo %d\n", getpid());
    
    char* readbuffer = malloc(sizeof(char)*80); // buffer para lectura desde pipe
    
    pthread_mutex_lock(&total_branches_m);
    int branch_position = *total_branches - 1;
    pthread_mutex_unlock(&total_branches_m);
    
    //pthread_mutex_t total_transactions_m = PTHREAD_MUTEX_INITIALIZER;
    //int total_transactions = 0;
    
    //pthread_mutex_t transactions_m = PTHREAD_MUTEX_INITIALIZER;
    //transaction* transactions = malloc(sizeof(transaction));
 
    read((*pipes)[branch_position][0], readbuffer, sizeof(readbuffer));
    //printf("%s - %d\n", readbuffer, (int)strlen(readbuffer));

    str_accounts = reach_input(&readbuffer, 0);
    char_terminals = reach_input(&readbuffer, 1);
    
    int n_accounts = atoi(str_accounts);
    int n_terminals = *char_terminals - '0';
    
    printf("cuentas: %d | terminales: %d\n", n_accounts, n_terminals);
    
    pthread_mutex_t accounts_m = PTHREAD_MUTEX_INITIALIZER;
    account* accounts = malloc(sizeof(account)*n_accounts);

    pthread_mutex_lock(&accounts_m);
    create_accounts(n_accounts, bankId, getpid(), &accounts);
    pthread_mutex_unlock(&accounts_m);
		
		pthread_mutex_lock(&pipes_m);
		write((*pipes)[branch_position+1][1], "null", strlen("null"));
		pthread_mutex_unlock(&pipes_m);

    /*for (int i=0; i<n_terminals; ++i)
    {
      pthread_t p;
      int rc = 0;
      rc = pthread_create(&p, NULL, create_transaction, )
    }*/

  }
  return NULL;
}


void cpy_str(char *tar, const char *ori){
  char c;
  int i = 0;
  while ((c=ori[i])!='\0'){
    tar[i]=c;
    i++;
  }
  tar[i] = '\0';

  return;
}
