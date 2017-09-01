#include "header.h"

void crear_cuentas(int n_cuentas, pid_t bancid, pid_t sucid, cuenta** cuentas)
{
	for(int i=0;i<n_cuentas;i++){
		cuenta* c = malloc(sizeof(cuenta));
		c->sucursal = sucid % 1000;
		c->numero = i;
		c->saldo = (rand() % 4999000)+1000;
		c->codigo = malloc(17*sizeof(char));
      	int u = (int)bancid;
		int itedaror = 0;
		int digito_size = 5;
		while (u > (int)pow(10, digito_size)) digito_size--;
		while (u >= 0){
			c->codigo[itedaror] = (u/(int)pow(10, digito_size))+'0';
			u = u%(int)pow(10, digito_size);
			itedaror++;
			digito_size--;
			if (digito_size < 0){
				u = -1;
				break;
			}
		}
      	int suc = c->sucursal;
      	c->codigo[6] = '-';
	  	c->codigo[7] = (suc/100)+'0';
	  	c->codigo[8] = ((suc%100)/10)+'0';
	  	c->codigo[9] = (suc%10)+'0';
	  	c->codigo[10] = '-';
	  	int cu = c->numero;
	  	int iterator = 11;
	  	int digit_size = 5;
	 	while (cu > (int)pow(10, digit_size)) digit_size--;
	  	while (cu >= 0){
			c->codigo[iterator] = (cu/(int)pow(10, digit_size))+'0';
			cu = cu%(int)pow(10, digit_size);
			iterator++;
			digit_size--;
			if (digit_size < 0){
				cu = -1;
				break;
			}
		}

		c->codigo[iterator] = '\0';

		cuentas[i] = c;

		printf("- %s: $%d\n", c->codigo, c->saldo);
	}
}

char* codificar_desde_sucursal(char operacion, cuenta* cuenta, int aleatorio, int monto)
{
	char* retorno;
	retorno = malloc(64*sizeof(char*));

	retorno[0] = operacion;
	retorno[1] = '-';
	for (int i=0;i<17;i++)
	{
		retorno[i+2] = cuenta->codigo[i];
	}
	retorno[19] = '-';

	int iterator = 20;

	int mto = aleatorio;
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
	iterator++;

	mto = monto;
	digit_size = 0;
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
	retorno[iterator] = '\0';

	return retorno;
}

char* codificar_desde_sucursal_con_resultado(char operacion, cuenta* cuentaEmisor, cuenta* cuentaReceptor, int monto, char resultado)
{
	char* retorno;
	retorno = malloc(64*sizeof(char*));

	retorno[0] = operacion;
	retorno[1] = '-';
	for (int i=0;i<17;i++)
	{
		retorno[i+2] = cuentaEmisor->codigo[i];
	}
	retorno[18] = '-';
	retorno[19] = '-';
	for (int i=0;i<17;i++)
	{
		retorno[i+20] = cuentaReceptor->codigo[i];
	}
	retorno[37] = '-';

	int iterator = 38;

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