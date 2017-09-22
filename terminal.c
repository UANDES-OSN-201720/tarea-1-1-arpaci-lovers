#include "terminal.h"

extern pthread_mutex_t pipes_m;
extern pthread_mutex_t total_branches_m;
extern pthread_mutex_t branches_m;
extern pthread_mutex_t total_accounts_m;
extern pthread_mutex_t accounts_codes_m;
extern pthread_mutex_t transactions_m;
extern pthread_mutex_t transactions_id_m;

void* terminal(void* args)
{

	ter_args* ta = (ter_args*) args;
	account** accounts = ta->accounts;
	int n_accounts = ta->n_accounts;
	int* total_accounts = ta->total_accounts;
	char*** transactions = ta->transactions;
	int** pipe = ta->pipe;
	int* transactions_id = ta->transactions_id;
	
	
	while (true)
	{
		usleep(random_number(100000,500000));
		
		int random_account = random_number(0, *total_accounts);
		
		pthread_mutex_lock(&transactions_id_m);
		(*transactions_id)++;
		
		pthread_mutex_lock(&transactions_m);
		*transactions = realloc(*transactions, sizeof(transactions)+1);
		pthread_mutex_unlock(&transactions_m);
		
		*transactions[(*transactions_id)-1] = create_transaction(n_accounts, accounts, random_account, *pipe, transactions_id);
		
		pthread_mutex_unlock(&transactions_id_m);
	}
}
