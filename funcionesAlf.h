#ifndef FUNCALF_H
#define FUNCALF_H
#include "header.h"

//pthread_mutex_t transactions_id_m;

char choose_task();
void create_accounts(int n_cuentas, pid_t bancid, pid_t sucid, account** cuentas);
char* create_transaction(int amount_accounts, account** accounts, int random, int* pipe, int* transactions_id);
char* reach_input(char** input, int argumento);
int lenght_transaction(char t);
void dump(char*** info, int suc, int amount_trans, FILE *fp);
void dump_accs(account** reg, int amount_acco, FILE* fp);
void dump_errs(char*** info, int suc, int amount_trans, FILE *fp);

#endif
