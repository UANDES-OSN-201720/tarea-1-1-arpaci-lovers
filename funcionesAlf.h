#include "header.h"

void crear_cuentas(int n_cuentas, pid_t bancid, pid_t sucid, cuenta** cuentas)
{
	for(int i=0;i<n_cuentas;i++){
		cuenta* c = malloc(sizeof(cuenta));
		c->sucursal = sucid % 1000;
		c->numero = i;
		c->saldo = random_number(1000, 500000000);
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
	/*
	OJO aca que la division entre ambas cuentas esta delimitada
	por dos guiones en lugar de uno.
	*/
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
	else
	{
		retorno->er_ex = "Fracaso";
	}

	return retorno;

}