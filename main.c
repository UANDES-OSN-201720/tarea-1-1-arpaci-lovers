
#include "funcionesAlf.h"

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


// Cuenten con este codigo monolitico en una funcion
// main como punto de partida.
// Idealmente, el codigo del programa deberia estar
// adecuadamente modularizado en distintas funciones,
// e incluso en archivos separados, con dependencias
// distribuidas en headers. Pueden modificar el Makefile
// libremente para lograr esto.
int main(int argc, char** argv) {
  srand(time(NULL));
  size_t bufsize = 512;
  char* commandBuf = malloc(sizeof(char)*bufsize);
  char** cuentas = malloc(sizeof(char**));
  int total_cuentas=0;
  pid_t* sucursales = malloc(sizeof(sucursales));
  int total_sucursales = 0;
  int** pipes = malloc(sizeof(int[2]));
  
  char readbuffer[80]; // buffer para lectura desde pipe
  
  const int bankId = getpid() % 1000;
  printf("Bienvenido a Banco '%d'\n", bankId);
  
  int sucursal_creada = 0;

  while (true) {
  
    if (sucursal_creada){
      sucursal_creada = 0;
      char *str_total_cuentas = malloc(sizeof(char)*sizeof(total_cuentas));
      sprintf(str_total_cuentas, "%d", total_cuentas);
      printf("%s\n", str_total_cuentas);
      for (int i=0; i<total_sucursales; i++){
        write(pipes[i][1], str_total_cuentas, (strlen(str_total_cuentas)+1));
      }
    }
  
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
      
      sucursal_creada = 1;
      
      int bankPipe[2];
      total_sucursales++;
      pipes = realloc(pipes, sizeof(int[2])*total_sucursales);
      pipes[total_sucursales-1] = bankPipe;
      
      pipe(pipes[total_sucursales-1]);
      
      char* c;
      if (strlen(commandBuf)> strlen("init")){
        int j = 0;
        c = malloc(sizeof(char)*(strlen(commandBuf)-strlen("init")));
        for (int i=strlen("init")+1;i<strlen(commandBuf);i++){
          if (commandBuf[i]!=0){
            c[j] = commandBuf[i];
            j++;
          }
        }
      }
      else{
        c = malloc(sizeof(char)*4);
        sprintf(c, "1000");
      }
      
      total_cuentas += atoi(c);
      
      char *strnum = malloc(sizeof(char)*(3+sizeof(total_cuentas)+strlen(c)));
      sprintf(strnum, "%d %s", total_cuentas, c);
      printf("strnum: %s\n", strnum);
      pid_t sucid = fork();

      if (sucid > 0) {
      
        printf("Sucursal creada con ID '%d'\n", sucid);

        // Enviando cantidad de cuentas a la sucursal a la sucursal
        sucursales = realloc(sucursales, sizeof(pid_t)*total_sucursales);
        sucursales[total_sucursales-1] = sucid;
        write(pipes[total_sucursales-1][1], strnum, (strlen(strnum)+1));

        continue;
      }
      // Proceso de sucursal-----------------------------------------------
      else if (!sucid) {
        movimiento* movimientos = malloc(sizeof(movimiento));
        int total_cuentas;
        cuenta** cuentas = malloc(sizeof(cuenta*));
        int cantidad_cuentas = 1000;
        int sucId = getpid() % 1000;
        printf("\n--------------\n");
        printf("Hola, soy la sucursal '%d'\n", sucId);
        int bytes = read(bankPipe[0], readbuffer, sizeof(readbuffer));
        printf("readbuffer: %s\n", readbuffer);
        //Sacando la informacion del buffer
        int i;
         
        char *tc = malloc(sizeof(char));
        int counter = 0;
        
        for (i=0;i<strlen(readbuffer);i++){
          if(readbuffer[i]==' '){
            counter = 0;
            break;
          } else {
            counter++;
            tc = realloc(tc, sizeof(char)*counter);
            tc[counter-1] = readbuffer[i];
          }
        }
        i++;
        
        char *cc = malloc(sizeof(char));
        for (i; i<strlen(readbuffer);i++){
          counter++;
          cc = realloc(cc, sizeof(char)*counter);
          cc[counter-1] = readbuffer[i];
        }
        
        printf("En sucursal... bID:%d tc:%s cc:%s\n", bankId, tc, cc);
        
        cantidad_cuentas = atoi(cc);
        total_cuentas = atoi(tc);
        printf("Tengo '%d' cuentas\n", cantidad_cuentas);

        crear_cuentas(cantidad_cuentas, bankId, sucid, cuentas);
        
        while (true) {
          printf("En el while\n");
          bytes = read(bankPipe[0], readbuffer, sizeof(readbuffer));
          printf("Soy la sucursal '%d' y leí el mensaje: '%s'\n", sucId, readbuffer);
          
          if (!strncmp("quit", readbuffer, strlen("quit"))){
            free(movimientos);
            free(cuentas);
            free(tc);
            free(cc);
            close(bankPipe[0]);
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
      for (int i=0; i<total_sucursales; i++){
        write(pipes[i][1], "quit", (strlen("quit")+1));
      }
    }
    else if (!strncmp("list", commandBuf, strlen("list"))) {
      if (isdigit(sucursales[0]) && isdigit(atoi(cuentas[0][0]))){
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
  free(sucursales);
  free(commandBuf);
  free(cuentas);
  free(sucursales);
  for (int i=0; i<total_sucursales;i++){
    close(pipes[i][1]);
  }
  free(pipes);

  return(EXIT_SUCCESS);
}
