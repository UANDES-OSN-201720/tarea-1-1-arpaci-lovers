#include <math.h>
#include <stdlib.h>
#include "funcionesAlf.h"
#include "header.h"

//pthread_mutex_t transactions_id_m = PTHREAD_MUTEX_INITIALIZER;

int random_number(int min, int max){
    int t = (rand() % (max + 1 - min)) + min;
    return t;
}

char choose_task()
{
	int choice = random_number(0,1);
	if (choice == 0) return 'd';
	else return 'r';
}

char* reach_input(char** input, int argument)
{
	char* goal = malloc(sizeof(char));
	char* given = *input;
	int traveler = 0;
	int counter = 0;
	while (counter<argument)
	{
		if (given[traveler] == ' ')
		{
			counter++;
		}
		else if (given[traveler] == '\0')
		{
			return NULL;
		}

		traveler++;

		if (counter == argument)
		{
			break;
		}
	}

	counter = 0;
	while (given[traveler] != ' ')
	{
		if (given[traveler] == '\0')
		{
			goal[counter] = '\0';
			break;
		}
		goal[counter] = given[traveler];
		counter++; traveler++;
		goal = realloc(goal, sizeof(char)*(counter)+1);
	}

	return goal;
}

void create_accounts(int n_cuentas, pid_t bancid, pid_t sucid, account** cuentas)
{
	for(int i=0;i<n_cuentas;i++) {
		account* a = malloc(sizeof(account));
		a->branch = sucid % 1000;
		a->number = i;
		a->balance = random_number(1000, 500000000);
		a->code = calloc(15,sizeof(char));
    	int u = (int)bancid;
		int iterator = 0;
		int digito_size = 5;
		while (u > (int)pow(10, digito_size)) digito_size--;
		while (u >= 0){
			a->code[iterator] = (u/(int)pow(10, digito_size))+'0';
			u = u%(int)pow(10, digito_size);
			iterator++;
			digito_size--;
			if (digito_size < 0){
				u = -1;
				break;
			}
		}
    	int suc = a->branch;
    	a->code[6] = '-';
	  	a->code[7] = (suc/100)+'0';
	  	a->code[8] = ((suc%100)/10)+'0';
	  	a->code[9] = (suc%10)+'0';
	  	a->code[10] = '-';
	  	int ac = a->number;
	  	iterator = 11;
	  	int digit_size = 5;
	 	while (ac > (int)pow(10, digit_size)) digit_size--;
	  	while (ac >= 0){
			a->code[iterator] = (ac/(int)pow(10, digit_size))+'0';
			ac = ac%(int)pow(10, digit_size);
			iterator++;
			digit_size--;
			if (digit_size < 0){
				ac = -1;
				break;
			}
		}

		a->code[iterator] = '\0';

		cuentas[i] = a;

		printf("- %s: $%d\n", a->code, a->balance);
	}
}

int lenght_transaction(char t)
{
	if (t == 'd')
	{
		return 36;
	}
	return 26;
}

int digits(int n)
{
	printf("in digits\n");
	int counter = 0;
	
	while (true)
	{
		int k = n/pow(10,counter);
		printf("k: %d\n", k);
		if (n/pow(10,counter) < 1) break;
		else counter++;
	}
	printf("counter: %d\n", counter);
	return counter;
}

char* create_transaction(int amount_accounts, account** accounts, int random, int* pipe, int* transactions_id)
{
	printf("hi0\n");
	//pthread_mutex_lock(&transactions_id_m);
	printf("hi1\n");
	printf("transactions_id: %d\n", *transactions_id);
	int id_digits = digits(*transactions_id);
	printf("id_digits: %d\n", id_digits);
	//pthread_mutex_unlock(&transactions_id_m);

	printf("Hi\n");
	int account_to_be_used = random_number(0,amount_accounts-1);
	char type = choose_task();
	printf("Hi2\n");
	//pthread_mutex_lock(&transactions_id_m);
	char* str_id = malloc(sizeof(char)*id_digits);
	printf("Hi3\n");
	sprintf(str_id, "%d", *transactions_id);
	char* result = malloc(sizeof(char)*(lenght_transaction(type)+id_digits));
	printf("Hi4\n");
	//pthread_mutex_unlock(&transactions_id_m);
	
	
	result[0] = type;
	result[1] = ' ';
	for (int i=0;i<14;i++)
	{
		result[i+2] = accounts[account_to_be_used]->code[i];
	}
	result[19] = ' ';

	int iterator = 20;

	int mto = random;
	int digit_size = 0;
	while (mto > (int)pow(10, digit_size)) digit_size++;
	digit_size--;
	while (mto >= 0){
		result[iterator] = (mto/(int)pow(10, digit_size))+'0';
		mto = mto%(int)pow(10, digit_size);
		iterator++;
		digit_size--;
		if (digit_size < 0){
			mto = -1;
			break;
		}
	}

	result[iterator] = ' ';
	iterator++;

	mto = random_number(0,5000000);
	digit_size = 0;
	while (mto > (int)pow(10, digit_size)) digit_size++;
	digit_size--;
	while (mto >= 0){
		result[iterator] = (mto/(int)pow(10, digit_size))+'0';
		mto = mto%(int)pow(10, digit_size);
		iterator++;
		digit_size--;
		if (digit_size < 0){
			mto = -1;
			break;
		}
	}
	
	result[iterator] = ' ';
	
	char* p;
	int id = strtol(str_id, &p, 10);
	for (int i=0; i<digits(id); i++)
	{
		iterator++;
		result[iterator] = str_id[i];
	}
	
	result[iterator] = '\0';
	
	printf("transaction: %s\n", result);

	write(pipe[1], result, strlen(result));

	return result;
}

void dump(char*** info, int suc, int amount_trans, FILE *fp)
{
	fprintf(fp, "Tipo de Transaccion,Medio de Origen,Cuenta de Origen,Cuenta de Destino\n");

	char** data = *info;
	for (int j=0; j<amount_trans; j++)
	{
		if (data[j][2] == 'd')
		{
			fprintf(fp, "Deposito,%d,", suc);
			for (int i=4; i<18; i++)
			{
				fprintf(fp, "%c", data[j][i]);
			}
			fprintf(fp, ",");
			for (int i=19; i<33; i++)
			{
				fprintf(fp, "%c", data[j][i]);
			}
			fprintf(fp, "\n");
		}
		else
		{
			fprintf(fp, "Retiro,");
			for (int i=4; i<7; i++)
			{
				fprintf(fp, "%c", data[j][i]);
			}
			fprintf(fp, ",");
			for (int i=9; i<23; i++)
			{
				fprintf(fp, "%c", data[j][i]);
			}
			fprintf(fp, ",");
			for (int i=9; i<23; i++)
			{
				fprintf(fp, "%c", 'x');
			}
			fprintf(fp, "\n");
		}
	}
}

void dump_accs(account** reg, int amount_acco, FILE* fp)
{
	fprintf(fp, "Numero de Cuenta,Saldo\n");

	for (int i=0; i<amount_acco; i++)
	{
		fprintf(fp, "%s,%d\n", reg[i]->code, reg[i]->balance);
	}
}

/*
RECIBO

v d BBB-SSS-CCCCCC BBB-SSS-CCCCCC $$$$$$ e/f/i
v r SSS BBB-SSS-CCCCCC $$$$$$ e/f/i
*/

void dump_errs(char*** info, int suc, int amount_trans, FILE *fp)
{
	fprintf(fp, "Tipo de Error,Numero de Cuenta,Saldo Previo a la Transaccion,Monto que se quiso retirar\n");

	char** data = *info;
	for (int j=0; j<amount_trans; j++)
	{
		if (data[j][2] == 'd')
		{
			if (data[j][41] == 'f')
			{
				fprintf(fp, "1,");
			}
			else if (data[j][41] == 'i')
			{
				fprintf(fp, "2,");
			}
			else
			{
				continue;
			}
			for (int i=4; i<18; i++)
			{
				fprintf(fp, "%c", data[j][i]);
			}
			fprintf(fp, ",saldo_anterior,");
			int index = 34;
			char walker = '-';
			while(walker == ' ')
			{
				fprintf(fp, "%c", data[j][index]);
				walker++;
			}
			fprintf(fp, "\n");
		}
		
		else
		{
			if (data[j][30] == 'f')
			{
				fprintf(fp, "1,");
			}
			else if (data[j][30] == 'i')
			{
				fprintf(fp, "2,");
			}
			else
			{
				continue;
			}
			for (int i=8; i<22; i++)
			{
				fprintf(fp, "%c", data[j][i]);
			}
			fprintf(fp, ",saldo_anterior,");
			int index = 23;
			char walker = data[j][index];
			while(walker != ' ')
			{
				fprintf(fp, "%c", walker);
				index++;
				walker = data[j][index];
			}
			fprintf(fp, "\n");
		}
	}
}
