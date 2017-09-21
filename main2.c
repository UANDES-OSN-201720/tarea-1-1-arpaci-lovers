#define _GNU_SOURCE
#include <signal.h>
#include <stdio.h>
#include "init.h"
#include "pipe_handler.h"

extern pthread_mutex_t pipes_m;
extern pthread_mutex_t total_branches_m;
extern pthread_mutex_t branches_m;
extern pthread_mutex_t total_accounts_m;
extern pthread_mutex_t accounts_codes_m;

int main(int argc, char** argv) 
{
  srand(time(NULL));

  size_t bufsize = 512;
  char* commandBuf = malloc(sizeof(char)*bufsize);
  //Manejar posible error

  // Para guardar descriptores de pipe
  // el elemento 0 es para lectura
  // y el elemento 1 es para escritura.
  int** pipes = malloc(sizeof(int*));
  //>Manejar posible error
  
  int total_branches = 0;
  pid_t* branches = malloc(sizeof(int));
  //>Manejar posible error
  
  int total_accounts = 0;
  char** accounts_codes = malloc(sizeof(char*));
  //>Manejar posible error
  

  const int bankId = getpid() % 1000;
  printf("Bienvenido a Banco '%d'\n", bankId);
  
  moc_args ma;
  ma.total_branches = &total_branches;
  ma.pipes = &pipes;
  ma.accounts = &accounts_codes;
  ma.total_accounts = &total_accounts;
  
  pthread_t pipe_handler_t;
  pthread_create(&pipe_handler_t, NULL, main_office_comunication, &ma);

  while (true) {
    printf(">>");
    getline(&commandBuf, &bufsize, stdin);

    // Manera de eliminar el \n leido por getline
    commandBuf[strlen(commandBuf)-1] = '\0';

    printf("Comando ingresado: '%s'\n", commandBuf);

    if (!strncmp("quit", commandBuf, strlen("quit"))) 
    {
        printf("In main->quit\n");
        free(pipes);
        free(branches);
        free(accounts_codes);
        pthread_mutex_lock(&total_branches_m);
        pthread_mutex_lock(&branches_m);
        if (total_branches == 0) break;
        for (int i = 0; i<total_branches; i++)
        {
          kill(branches[i], SIGKILL);
        }
        pthread_mutex_unlock(&branches_m);
        pthread_mutex_unlock(&total_branches_m);
        break;
    }
    else if (!strncmp("init", commandBuf, strlen("init"))) 
    {
      printf("In main->init\n");
      
      pthread_t init_thread;
      init_args ia;
      
      ia.bankId = bankId;
      ia.total_branches = &total_branches;
      ia.branches = &branches;
      ia.pipes = &pipes;
      ia.total_accounts = &total_accounts;
      ia.commandBuffer = commandBuf;
    
      pthread_create(&init_thread, NULL, init, &ia);
      
      char* branch_accounts = reach_input(commandBuffer, 1);
      char* p;
      int ba = strtol(branch_accounts, &p, 10);
      
      char
      
      for (int i=0; i<sa, i++)
      {
      
      }
      
      /*total_branches++;
      pipes = realloc(pipes, sizeof(int*)*total_branches);
      
      pid_t sucid = fork();

      if (sucid > 0) {
        printf("Sucursal creada con ID '%d'\n", sucid);

        // Enviando saludo a la sucursal
        char msg[] = "Hola sucursal, como estas?";
        write(bankPipe[1], msg, (strlen(msg)+1));

        continue;
      }
      // Proceso de sucursal
      else if (!sucid) {
        int sucId = getpid() % 1000;
        printf("Hola, soy la sucursal '%d'\n", sucId);
        while (true) {
          // 100 milisegundos...
          int bytes = read(bankPipe[0], readbuffer, sizeof(readbuffer));
          printf("Soy la sucursal '%d' y me llego mensaje '%s' de '%d' bytes.\n",
            sucId, readbuffer, bytes);
          
          // Usar usleep para dormir una cantidad de microsegundos
          // usleep(100000);

          // Cerrar lado de lectura del pipe
          close(bankPipe[0]);

          // Para terminar, el proceso hijo debe llamar a _exit,
          // debido a razones documentadas aqui:
          // https://goo.gl/Yxyuxb
          _exit(EXIT_SUCCESS);
        }
      
      // error
      }
      else {
        fprintf(stderr, "Error al crear proceso de sucursal!\n");
        return (EXIT_FAILURE);
      }*/
    }
    else {
      fprintf(stderr, "Comando no reconocido.\n");
    }
    // Implementar a continuacion los otros comandos
  }

  printf("Terminando ejecucion limpiamente...\n");

  return(EXIT_SUCCESS);
}
