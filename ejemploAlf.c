#include "funcionesAlf.h"
int main()
{
	cuenta* lista_maestra[2];

	cuenta* cuentaAlf = malloc(sizeof(cuenta));
	cuentaAlf->sucursal = 540;
	cuentaAlf->numero = 1;
	cuentaAlf->saldo = 875480;
	cuentaAlf->codigo = "000542-540-000001";

	lista_maestra[0] = cuentaAlf;

	cuenta* cuentaMaca = malloc(sizeof(cuenta));
	cuentaMaca->sucursal = 668;
	cuentaMaca->numero = 0;
	cuentaMaca->saldo = 100080;
	cuentaMaca->codigo = "000542-968-000000";

	lista_maestra[1] = cuentaMaca;

	//Proceso SUCURSAL-MACA
		//Maca le quiere depositar a Alf desde su sucursal

	int monto_a_depositar = 9000;
	if (cuentaMaca->saldo >= monto_a_depositar)
	{
		printf("%s\n",codificar_desde_sucursal('d', cuentaMaca, 2, 9000));
		//Este string se envia por pipe a la casa matriz y ésta la se pasa
		//al proceso SUCURSAL-ALF.
	}
	else
	{
		//Mostrar muestra un mensaje de error
	}
	
	//Proceso SUCURSAL-ALF
		/*
		La sucursal de Alf recibe el mensaje y busca si la cuenta de Alf
		//existe, luego suma la plata a la cuenta de Alf y envia mensaje
		de vuelta indicando que la transacción fue un exito.
		*/
	cuentaAlf->saldo =cuentaAlf->saldo + monto_a_depositar;
	printf("%s\n\n",codificar_desde_sucursal_con_resultado('d', cuentaMaca, cuentaAlf, 9000, 'e'));

	//Proceso SUCURSAL-MACA
		/*
		La sucursal de Maca recibe este mensaje de exito (el parametro 'e')
		de la función y descuenta el monto de su cuenta.
		*/
	//Si el parametro de la funcion es 'e', implica exito y debe resatarse el
	//monto depositado.
	cuentaMaca->saldo = cuentaMaca->saldo - monto_a_depositar;
	//Si es 'f', entonces no se debe hacer el retiro de fondos.


	cuenta** cuentas = malloc(sizeof(**cuentas));
	crear_cuentas(10, (pid_t)8574, (pid_t)857, cuentas);

	return 0;
}
