#include <math.h>
#include <stdlib.h>
#include "funcionesAlf.h"
#include "header.h"

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

void* create_transaction(int amount_accounts, account** accounts, int random, int* pipe)
{
	char* result;
	result = malloc(64*sizeof(char*));
	int account_to_be_used = random_number(0,amount_accounts-1);
	result[0] = choose_task();
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
	result[iterator] = '\0';

	write(pipe[1], result, strlen(result));

	return NULL;
}
/*
char* codificar_desde_sucursal_con_resultado(char operacion, cuenta* cuentaEmisor, cuenta* cuentaReceptor, int monto, char resultado)
{
	//Si el ultimo parametro de la funcion es 'e', implica exito.
	//De lo contrario, debe entregarse una 'f' indicando fracaso.
	char* retorno;
	retorno = malloc(64*sizeof(char*));
	//Si se desea enviar a la casa matriz para documentar, poner
	//operacion = 'o' si se trato de un deposito, y poner 'i' en caso de
	//haberse tratado de un retiro.
	retorno[0] = operacion;
	retorno[1] = '-';
	for (int i=0;i<17;i++)
	{
		retorno[i+2] = cuentaEmisor->codigo[i];
	}
	retorno[19] = '-';
	retorno[20] = '-';
	
	//OJO aca que la division entre ambas cuentas esta delimitada
	//por dos guiones en lugar de uno.
	
	for (int i=0;i<17;i++)
	{
		retorno[i+21] = cuentaReceptor->codigo[i];
	}
	retorno[38] = '-';

	int iterator = 39;

	int mto = monto;
	int digit_size = 0;
	while (mto > (int)pow(10, digit_size)) digit_size++;
	digit_size--;
	while (mto >= 0){
		retorno[iterator] = (mto/(int)pow(10, digit_size))+'0';
		mto = mto%(int)pow(10, digit_size);
		iterator++;
		digit_size--;
		if (digit_size < 0){
			mto = -1;
			break;
		}
	}
	retorno[iterator] = '-';
	retorno[iterator+1] = resultado;
	retorno[iterator+2] = '\0';

	return retorno;
}

movimiento* crear_movimiento(char* mensaje)
{
	movimiento* retorno = malloc(sizeof(movimiento));

	if (mensaje[0] == 'o')
	{
		retorno->tipo = "Deposito";
	}
	else
	{
		retorno->tipo = "Retiro";
	}

	

	char* cuenta_origen = malloc(18*sizeof(char));
	int iterator = 2;
	for (int i=0; i<17; i++)
	{
		cuenta_origen[i] = mensaje[iterator];
		iterator++;
	}

	retorno->origen = cuenta_origen;

	iterator++; iterator++;

	char* cuenta_destino = malloc(17*sizeof(char));
	for (int i=0; i<17; i++)
	{
		cuenta_destino[i] = mensaje[iterator];
		iterator++;
	}

	retorno->destino = cuenta_destino;

	iterator++;
	int other_iterator = 0;

	char aviso = '.';
	char* monto_movimiento = malloc(10*sizeof(char));;
	while (aviso != '-')
	{
		monto_movimiento[other_iterator] = mensaje[iterator];
		iterator++; other_iterator++;
		aviso = mensaje[iterator];
	}
	int dinero;
	sscanf(monto_movimiento, "%d", &dinero);
	retorno->monto = dinero;

	iterator++;

	if (mensaje[iterator] == 'e')
	{
		retorno->er_ex = "Exito";
	}
	else if (mensaje[iterator] == 'f')
	{
		retorno->er_ex = "Fracaso por falta de fondos";
	}
	else
	{
		retorno->er_ex = "Fracaso por cuenta inexistente";
	}

	return retorno;

}

void dump_csv(movimiento** movimientos, int sucid)
{
	//Primero buscamos el largo del arreglo de movimientos
	int movimientos_totales = sizeof(*movimientos)/sizeof(movimiento);

	FILE *fp;
	fp = fopen("dump_PID.csv", "w+");
	fprintf(fp, "Tipo de transacción,Medio de origen,Cuenta de origen,Cuenta de destino\n");
	//Luego empezamos a recorrer todos los movimientos para ir registrando.
	for (int i=0; i<movimientos_totales; i++)
	{
		char* linea = malloc(sizeof(char));
		int iterator = 0;
		for (int j=0; j<sizeof(movimientos[i]->tipo); j++)
		{
			linea[iterator] = movimientos[i]->tipo[j];
			iterator++;
			linea = realloc(linea, iterator+1);
		}

		linea[iterator] = ',';
		iterator++;
		linea = realloc(linea, iterator+1);

		//ACA FALTA EL MEDIO DE ORIGEN QUE NO TENIAMOS CLARO LO QUE ERA
		for (int j=0; j<sizeof(movimientos[i]->origen); j++)
		{
			linea[iterator] = 'x';
			//linea[iterator] = movimientos[i]->origen[j];
			//iterator++;
			//linea = realloc(linea, iterator+1);
		}

		linea[iterator] = ',';
		iterator++;
		linea = realloc(linea, iterator+1);

		for (int j=0; j<sizeof(movimientos[i]->origen); j++)
		{
			linea[iterator] = movimientos[i]->origen[j];
			iterator++;
			linea = realloc(linea, iterator+1);
		}

		linea[iterator] = ',';
		iterator++;
		linea = realloc(linea, iterator+1);

		for (int j=0; j<sizeof(movimientos[i]->destino); j++)
		{
			linea[iterator] = movimientos[i]->destino[j];
			iterator++;
			linea = realloc(linea, iterator+1);
		}

		linea[iterator] = '\0';
		iterator++;

		fprintf(fp,linea);
	}

	fclose(fp);
}

*/
