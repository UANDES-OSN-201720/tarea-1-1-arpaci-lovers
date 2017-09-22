#include "terminal.h"

extern pthread_mutex_t pipes_m;
extern pthread_mutex_t total_branches_m;
extern pthread_mutex_t branches_m;
extern pthread_mutex_t total_accounts_m;
extern pthread_mutex_t accounts_codes_m;

void* terminal(void* args)
{
	ter_args* ta = (ter_args*) args;
	account** accounts = ta->accounts;
	int n_accounts = ta->n_accounts;
	int* total_accounts = ta->total_accounts;
	char*** transactions = ta->transactions;
	int** pipe = ta->pipe;
	
	printf("%s\n", (*transactions)[0]);
	
	while (true)
	{
		usleep(random_number(100000,500000));
		
		int random_account = random_number(0, *total_accounts);
		
		//void create_transaction(int amount_accounts, account** accounts, int random, int* pipe)
		create_transaction(n_accounts, accounts, random_account, *pipe);
	}
}
