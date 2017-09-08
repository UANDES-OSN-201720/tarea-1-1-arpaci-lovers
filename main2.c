#include "header.h"
#include <signal.h>
#include <pthread.h>

// Cuenten con este codigo monolitico en una funcion
// main como punto de partida.
// Idealmente, el codigo del programa deberia estar
// adecuadamente modularizado en distintas funciones,
// e incluso en archivos separados, con dependencias
// distribuidas en headers.
int main(int argc, char** argv) 
{
  size_t bufsize = 512;
  char* commandBuf = malloc(sizeof(char)*bufsize);
  //Manejar posible error

  // Para guardar descriptores de pipe
  // el elemento 0 es para lectura
  // y el elemento 1 es para escritura.
  int** pipes = malloc(sizeof(int*));
  //Manejar posible error
  
  int total_sucursales = 0;
  pid_t* sucursales = malloc(sizeof(int));
  //Manejar posible error
  
  int total_cuentas = 0;
  char** cuentas = malloc(sizeof(char*));
  //Manejar posible error
  
  int bankPipe[2];
  char readbuffer[80]; // buffer para lectura desde pipe

  // Se crea un pipe...
  pipe(bankPipe);
  

  const int bankId = getpid() % 1000;
  printf("Bienvenido a Banco '%d'\n", bankId);

  while (true) {
    printf(">>");
    getline(&commandBuf, &bufsize, stdin);

    // Manera de eliminar el \n leido por getline
    commandBuf[strlen(commandBuf)-1] = '\0';

    printf("Comando ingresado: '%s'\n", commandBuf);

    if (!strncmp("quit", commandBuf, strlen("quit"))) 
    {
        free(pipes);
        free(sucursales);
        free(cuentas);
        
        for (int i = 0; i<total_sucursales; i++)
        {
          kill(sucursales[i], SIGKILL);
        }
        break;
    }
    else if (!strncmp("init", commandBuf, strlen("init"))) 
    {
      total_sucursales++;
      pipes = realloc(pipes, sizeof(int*)*total_sucursales);
      
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
      }
      // error
      else {
        fprintf(stderr, "Error al crear proceso de sucursal!\n");
        return (EXIT_FAILURE);
      }
    }
    else {
      fprintf(stderr, "Comando no reconocido.\n");
    }
    // Implementar a continuacion los otros comandos
  }

  printf("Terminando ejecucion limpiamente...\n");
  // Cerrar lado de escritura del pipe
  close(bankPipe[1]);

  return(EXIT_SUCCESS);
}
