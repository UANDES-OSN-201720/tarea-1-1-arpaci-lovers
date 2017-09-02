#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <sys/types.h>

srand(time(NULL));

int dump_input(char *cmd, int *sucursales, char *buff){
  char strnum[4];
  int num;
  int exists = 0;
  if (strlen(buff)> strlen(cmd)){
    strcpy(strnum, "");
    int j = 0;
    for (int i=strlen(cmd)+1;i<strlen(buff);i++){
      if (buff[i]!=0){
        strnum[j] = buff[i];
        j++;
      }
    }
    num = atoi(strnum);
  }
  else{
    printf("Especifique sucursal:\n");
    scanf("%d", &num);
  }     
  for (int i=0; i<sizeof(sucursales); i++){
    if (sucursales[i]==num){
      exists = 1;
    }
  }
  if (!exists){
    printf("La sucursal ingresada no existe.\n");
  }
  return num;
}


typedef struct
{
	unsigned int banco;
	unsigned int sucursal;
	unsigned int numero; 
  unsigned int saldo;
} cuenta;

typedef struct
{
	unsigned int id;
	cuenta origen;
	cuenta destino;
	unsigned int monto;
	unsigned int erex; //0 para error, 1 para exito
} movimiento;


// Cuenten con este codigo monolitico en una funcion
// main como punto de partida.
// Idealmente, el codigo del programa deberia estar
// adecuadamente modularizado en distintas funciones,
// e incluso en archivos separados, con dependencias
// distribuidas en headers. Pueden modificar el Makefile
// libremente para lograr esto.
int main(int argc, char** argv) {
  size_t bufsize = 512;
  char* commandBuf = malloc(sizeof(char)*bufsize);
  char** cuentas;
  int* sucursales = malloc(sizeof(sucursales));
  movimiento* movimientos = malloc(sizeof(movimiento));

  // Para guardar descriptores de pipe
  // el elemento 0 es para lectura
  // y el elemento 1 es para escritura.
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

    if (!strncmp("quit", commandBuf, strlen("quit"))) {
        break;
    }
    else if (!strncmp("init", commandBuf, strlen("init"))) {
      // OJO: Llamar a fork dentro de un ciclo
      // es potencialmente peligroso, dado que accidentalmente
      // pueden iniciarse procesos sin control.
      // Buscar en Google "fork bomb"

      char strnum[512]; //Cantidad de cuentas ingresadas por el usuario, si es que lo hizo.
      strcpy(strnum, "None");
      if (strlen(commandBuf)> strlen("init")){
        strcpy(strnum, "");
        int j = 0;
        for (int i=strlen("init")+1;i<strlen(commandBuf);i++){
          if (commandBuf[i]!=0){
            strnum[j] = commandBuf[i];
            j++;
          }
        }
      }
      pid_t sucid = fork();

      if (sucid > 0) {
        printf("Sucursal creada con ID '%d'\n", sucid);

        // Enviando cantidad de cuentas a la sucursal a la sucursal
        sucursales = (int *) realloc(sucursales, sizeof(sucursales)+1);
        sucursales[0] = sucid % 1000;
        write(bankPipe[1], strnum, (strlen(strnum)+1));

        continue;
      }
      // Proceso de sucursal-----------------------------------------------
      else if (!sucid) {
        int total_cuentas;
        cuenta* cuentas;
        int accNumber = 1000;
        int sucId = getpid() % 1000;
        printf("\nHola, soy la sucursal '%d'\n", sucId);
        int bytes = read(bankPipe[0], readbuffer, sizeof(readbuffer));
        printf("%s\n", readbuffer);
        if (strncmp("None", readbuffer, strlen("None"))!=0){
          accNumber = atoi(readbuffer);
        }
        printf("Tengo '%d' cuentas\n", accNumber);

        //ACA SE TIENEN QUE CREAR LAS CUENTAS
        
        while (true) {
          // 100 milisegundos...
          bytes = read(bankPipe[0], readbuffer, sizeof(readbuffer));
          printf("Soy la sucursal '%d' y leí el mensaje: '%s'\n", sucId, readbuffer);
          
          //Ejecuta lo que le dice el mensaje
          
          if (!strncmp("kill", readbuffer, strlen("kill"))){
            // Cerrar lado de lectura del pipe
            close(bankPipe[0]);

            // Para terminar, el proceso hijo debe llamar a _exit,
            // debido a razones documentadas aqui:
            // https://goo.gl/Yxyuxb
            _exit(EXIT_SUCCESS);
          }

          // Usar usleep para dormir una cantidad de microsegundos
          usleep(random_number(100000, 500000));

        }
      }
      // error
      else {
        fprintf(stderr, "Error al crear proceso de sucursal!\n");
        return (EXIT_FAILURE);
      }
    }
    else if (!strncmp("kill", commandBuf, strlen("kill"))) {
      //Se elimina sucursal especificada.
      printf("Matar proceso\n");
    }
    else if (!strncmp("list", commandBuf, strlen("list"))) {
      if (isdigit(sucursales[0]) && isdigit(atoi(cuentas[0][0]))){
        //Se imprime lista de movimientos en la sucursal
        printf("Lista:\n");
        printf("Sucursal\t|n° inicial\t|n° final\t|cuentas totales\n");
        for (int s=0;s<sizeof(sucursales);s++){
          char id[3];
          sprintf(id, "%d", sucursales[s]);
          for (int j=0; j<sizeof(cuentas); j++){
            if (!strncmp(id, cuentas[j], strlen(id))){
              //Armar string
              printf("hi\n");
            }
          }
        }
      }
    }
    else if (!strncmp("dump_errs", commandBuf, strlen("dump_errs"))) {    
      int num = dump_input("dump_accs", sucursales, commandBuf);
      //Creación archivo csv de movimientos fallidos en una sucursal
      printf("Archivo creado\n");
    }
    else if (!strncmp("dump_accs", commandBuf, strlen("dump_accs"))) {
      int num = dump_input("dump_accs", sucursales, commandBuf);
      //Creación archivo csv de movimientos fallidos en una sucursal
      printf("Archivo creado\n");
    }
    else if (!strncmp("dump", commandBuf, strlen("dump"))) {
      int num = dump_input("dump_accs", sucursales, commandBuf);
      //Creación archivo csv de movimientos en una sucursal
      printf("Archivo creado\n");
    }
    else {
      fprintf(stderr, "Comando no reconocido.\n");
    }
  }

  printf("Terminando ejecucion limpiamente...\n");
  // Cerrar lado de escritura del pipe
  close(bankPipe[1]);
  free(sucursales);

  return(EXIT_SUCCESS);
}
