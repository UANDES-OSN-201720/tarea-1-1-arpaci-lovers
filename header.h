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


int random_number(int min, int max){
    int t = (rand() % (max + 1 - min)) + min;
    return t;
}