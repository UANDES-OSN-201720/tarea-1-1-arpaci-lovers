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
	unsigned int id;
	cuenta* origen;
	cuenta* destino;
	unsigned int monto;
	char er_ex[20];
} movimiento;