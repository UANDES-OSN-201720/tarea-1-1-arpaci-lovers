#include "header.h"

void crear_cuentas(int n_cuentas, pid_t sucid, cuenta** cuentas)
{
    for(int i=0;i<n_cuentas;i++){
      cuenta* c = malloc(sizeof(cuenta));
      c->sucursal = sucid % 1000;
      c->numero = i;
      c->saldo = (rand() % 4999000)+1000;
      cuentas[i] = c;
    }
}

char* codificar_desde_sucursal(char operacion, cuenta* cuenta, int monto)
{
	char* retorno;
	retorno = malloc(64*sizeof(char*));

	retorno[0] = operacion;
	retorno[1] = '-';

	int suc = cuenta->sucursal;
	retorno[2] = (suc/100)+'0';
	retorno[3] = ((suc%100)/10)+'0';
	retorno[4] = (suc%10)+'0';
	retorno[5] = '-';

	int cu = cuenta->numero;
	int iterator = 6;
	int digit_size = 5;
	while (cu > (int)pow(10, digit_size)) digit_size--;
	while (cu >= 0){
		retorno[iterator] = (cu/(int)pow(10, digit_size))+'0';
		cu = cu%(int)pow(10, digit_size);
		iterator++;
		digit_size--;
		if (digit_size < 0){
			cu = -1;
			break;
		}
	}
	retorno[iterator] = '-';
	iterator++;

	int mto = monto;
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