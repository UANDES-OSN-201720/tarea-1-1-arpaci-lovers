#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

typedef struct
{
	unsigned int banco;
	unsigned int sucursal;
	unsigned int numero; 
} cuenta;
int main()
{
	cuenta c;
	c.banco = 45;
	c.sucursal = 86;
	c.numero = 1;

	printf (string(c.banco));
}


