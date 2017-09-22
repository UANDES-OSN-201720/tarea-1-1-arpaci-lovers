#ifndef HEADER_H
#define HEADER_H
#define _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <sys/types.h>
#include <assert.h>

typedef struct
{	
	int branch;
	int number; 
	int balance;
	char* code;
} account;

typedef struct
{
	char* mensaje;
	int* pipe;
}info;


int random_number(int min, int max);

#endif
