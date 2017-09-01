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
		char* mensaje_a_enviar = codificar_desde_sucursal('d', cuentaMaca, 2, 9000);
		printf("Sucursal de Maca envia:  %s\n", mensaje_a_enviar);
		//Este string se envia por pipe a la casa matriz y ésta la se pasa
		//al proceso SUCURSAL-ALF, basandose en el numero presente en el tercer paramtro
		//de la funcion (que en la practica seria aleatorio).
	}
	else
	{
		char* mensaje_de_error = codificar_desde_sucursal_con_resultado('d', cuentaMaca, cuentaMaca, 9000, 'f');
		printf("Sucursal de Maca envia:  %s\n", mensaje_de_error);
		//Aca se envia el error a la casa matriz donde crea el struct de 
		//la transacción fallida.
	}
	
	//Proceso SUCURSAL-ALF
		/*
		La sucursal de Alf recibe el mensajey luego suma la plata a 
		la cuenta de Alf. Luego envia un mensaje de vuelta indicando que
		la transacción fue un exito.
		*/
	cuentaAlf->saldo =cuentaAlf->saldo + monto_a_depositar;
	char* mensaje_de_exito = codificar_desde_sucursal_con_resultado('d', cuentaMaca, cuentaAlf, 9000, 'e');
	printf("Sucursal de Alf envia:   %s\n", mensaje_de_exito);

	//Proceso SUCURSAL-MACA
		/*
		La sucursal de Maca recibe este mensaje de exito de la función y
		descuenta el monto de su cuenta.
		*/
		
	cuentaMaca->saldo = cuentaMaca->saldo - monto_a_depositar;

	//Proceso PADRE (Casa Matriz)
		/*
		Se le debe enviar un mensaje de exito de vuelta a la casa matriz para
		que se encargue de documentar la información del proceso.
		*/
	char* mensaje_a_documentar = codificar_desde_sucursal_con_resultado('o', cuentaMaca, cuentaAlf, 9000, 'e');
	printf("Sucursal de Alf envia a la caza matriz para documentar:  %s\n\n", mensaje_a_documentar);

	movimiento* resumen = crear_movimiento(mensaje_a_documentar);
	printf("RESUMEN DE LA OPERACION:\n");
	printf("\t%s\n", resumen->tipo);
	printf("\t%s\n", resumen->origen);
	printf("\t%s\n", resumen->destino);
	printf("\t%d\n", resumen->monto);
	printf("\t%s\n", resumen->er_ex);

	//cuenta** cuentas = malloc(sizeof(**cuentas));
	//crear_cuentas(10, (pid_t)8574, (pid_t)857, cuentas);

	return 0;
}
