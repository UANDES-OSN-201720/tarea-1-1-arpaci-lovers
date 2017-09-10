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

pthread_mutex_t pipes_m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t total_branches_m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t branches_m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t total_accounts_m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t accounts_codes_m = PTHREAD_MUTEX_INITIALIZER;


typedef struct
{
	int branch;
	int number; 
	int balance;
	char* code;
} account;

typedef struct
{
	char* type;
	char* origin;
	char* destination;
	unsigned int amount;
	char* state;
} transaction;


int random_number(int min, int max){
    int t = (rand() % (max + 1 - min)) + min;
    return t;
}
