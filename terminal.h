#include "funcionesAlf.h"

typedef struct
{
	account** accounts;
	int n_accounts;
	int* total_accounts;
	char*** transactions;
	int** pipe;
} ter_args;

void* terminal(void* args);
