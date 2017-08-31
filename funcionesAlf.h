#include "header.h"

void crear_cuentas(int n_cuentas, pid_t sucid, cuenta* cuentas)
{
    for(int i=0;i<n_cuentas;i++){
      cuenta c;
      c.sucursal = sucid % 1000;
      c.numero = i;
      c.saldo = (rand() % 4999000)+1000;
      cuentas[i] = c;
    }
}

char* codificar_desde_sucursal(cuenta* cuenta, int monto)
{
	char* retorno;
	retorno = malloc(64*sizeof(char*));

	retorno[0] = 'd';
	retorno[1] = '-';

	int suc = cuenta->sucursal;
	retorno[2] = (suc/100)-'0';
	retorno[3] = ((suc%100)/10)-'0';
	retorno[4] = (suc%10)-'0';
	retorno[5] = '-';

	int cu = cuenta->numero;
	int iterator = 6;
	int digit_size = 5;
	while (cu > 10**digit_size) digit_size--;
	while (cu != None){
		retorno[iterator] = (cu/(10**digit_size))-'0';
		iterator++;
		digit_size--;
		if (digit_size < 0){
			break;
		}
	}
	retorno[iterator] = '-';
	iterator++;

	int mto = monto;
	digit_size = 0;
	while (mto < 10**digit_size) digit_size++;
	digit_size--;
	while (cu != None){
		retorno[iterator] = (mto/(10**digit_size))-'0';
		iterator++;
		digit_size--;
		if (digit_size < 0){
			break;
		}
	}



}
