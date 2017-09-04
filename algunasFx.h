#include "funcionesAlf.h"

/*void sendMsj(int n){
	//enviar al pipe la cantidad 
	if(!strcm("init", commandBuff, strlen("init"))){
		char msj[1000000];
		char nn[999999];
		nn = n;
		strcpy(nn, msj)
		msj[0] = "r";//actualizar el numero para el random
		write(bankPipe[1], nn, 1000000);//faltaria hacerlo para todos los pipes
	}
}*/

char* decodificar_casa_matriz(char *mensaje_e, cuenta *cuentas, int total_cuentas, movimiento **movimientos, int* total_movimientos){

  //mensaje: o-bbbbbb-sss-ccc-rrr-mmmmmm
  //salida: o-bbbbbb-sss-ccc-bbbbbb-sss-ccc-mmmmm-e
  char operacion = mensaje_e[0];
  char estado = 'e';
  int i;
  
  char cuenta_origen[14];
  for (i=0; i<14; i++){
    cuenta_origen[i] = mensaje_e[i+2];
  }
  printf("cuenta origen: %s\n", cuenta_origen);
  int contador = 0;
  char *str_random = malloc(sizeof(char));
  for (i=17; i<strlen(mensaje_e); i++){
    contador++;
    if (mensaje_e[i]=='-'){
      break;
    } else {
      str_random = realloc(str_random, sizeof(char)*contador);
      str_random[contador-1] = mensaje_e[i];
    }
  }
  int cuenta_random = atoi(str_random);
  printf("cuenta random: %d\n", cuenta_random);
  
  cuenta cuenta_destino;
  if (cuenta_random >= total_cuentas){
    estado = 'i';
  } 
  if (!strncmp(cuentas[cuenta_random].codigo, "d", strlen("d"))){
    estado = 'i';
  } 
  if (estado!='i') {
    cuenta_destino = cuentas[cuenta_random];
	}
	i++;
	contador=0;
	int estado_encontrado = 0;
	char *monto = malloc(sizeof(char));
	int j;
	for (j=i; j<strlen(mensaje_e); j++){
	  if (mensaje_e[j]=='-'){
	    estado_encontrado = 1;
	    j++;
	    break;
	  } else {
	    monto = realloc(monto, sizeof(char)*contador);
	    monto[contador-1] = mensaje_e[j];
	  }
	}
	printf("monto: %s\n", monto);
	int largo_salida = 6+strlen(cuenta_origen)+strlen(cuenta_destino.codigo)+strlen(monto);
	char *mensaje_salida = malloc(largo_salida*sizeof(char));
	
	if (estado_encontrado){
	
	  estado = mensaje_e[j];
	  
	  sprintf(mensaje_salida, "%c-%s-%s-%s-%c", operacion, cuenta_origen, cuenta_destino.codigo, monto, estado);
	  
	  movimiento* m = crear_movimiento(mensaje_salida);
	  total_movimientos++;
	  movimientos = realloc(movimientos, sizeof(movimiento**)*(*total_movimientos));
	  movimientos[*total_movimientos-1] = m;
	  
	} else {
	
	  sprintf(mensaje_salida, "%c-%s-%s-%s-%c", operacion, cuenta_origen, cuenta_destino.codigo, monto, estado);
	}
	
	return mensaje_salida;
}


