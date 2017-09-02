#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <sys/types.h>

typedef struct
{
	int sucursal;
	int numero; 
	int saldo;
	char* codigo;
} cuenta;

typedef struct
{
	char* tipo;
	char* origen;
	char* destino;
	unsigned int monto;
	char* er_ex;
} movimiento;

void dump(movimiento* movimientos, int sucursal){
  FILE *fp;
  int i;
  
  char filename[12];
  fprintf(filename, "dump_%d.csv", &sucursal);
  fp=fopen(filename, "w+");
   
  fprintf(fp,"TIPO DE TRANSACCION, MEDIO DE ORIGEN, CUENTA DE ORIGEN, CUENTA DE DESTINO");
   
  for(i=0;i<sizeof(movimientos);i++){
    movimiento m = movimientos[i];
    
    fprintf(fp,"%s, %d, %s, %s, %s\n", m.tipo, &m.monto, m.origen, m.destino, m.er_ex); 
  fclose(fp);
  }
}
