
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
  movimiento* movimientos;
  
  char readbuffer[80]; // buffer para lectura desde pipe
  
  const int bankId = getpid() % 1000;
  printf("Bienvenido a Banco '%d'\n", bankId);
  
  int sucursal_creada = 0;

  char* mensaje;

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
      
      char *strnum = malloc(sizeof(char)*(3+sizeof(total_cuentas)+strlen(c)+1));
      sprintf(strnum, "%d %s %d\0", total_cuentas, c, total_sucursales);
      //printf("strnum: %s\n", strnum);
      pid_t sucid = fork();

      if (sucid > 0) {
      
        printf("Sucursal creada con ID '%d'\n", sucid%1000);

        // Enviando cantidad de cuentas a la sucursal a la sucursal
        sucursales = realloc(sucursales, sizeof(pid_t)*total_sucursales);
        sucursales[total_sucursales-1] = sucid;
        write(pipes[total_sucursales-1][1], strnum, (strlen(strnum)+1));
        //int conf = read(pipes[total_sucursales-1][0], readbuffer, sizeof(readbuffer));
        //printf("Soy la casa Matriz y leí el mensaje: '%s'\n", readbuffer);
        //write(pipes[total_sucursales-1][1], "Hola", (strlen("Hola")));



        continue;
      }
      // Proceso de sucursal-----------------------------------------------
      else if (!sucid) {
        movimientos = malloc(sizeof(movimiento));   //La sucursal guarda todos los movimientos que se originaron de ella
        int total_cuentas; //Tiene un int en el que guarda el total de cuentas de todo el banco
        int numero_child;  //Corresponde al numero de child creado (primer hijo, segundo hijo...)
        cuenta** cuentas = malloc(sizeof(cuenta*));   //Tiene un arreglo con todas sus cuentas
        int cantidad_cuentas = 1000;    //Cantidad predeterminada de cuentas
        int sucId = getpid() % 1000;    //Saca el PID del proceso sucursal
        printf("\n--------------\n");
        printf("Hola, soy la sucursal '%d'\n", sucId);
        int bytes = read(pipes[numero_child][0], readbuffer, sizeof(readbuffer));
        mensaje = readbuffer;
        //printf("readbuffer: %s\n", mensaje);
        //Sacando la informacion del buffer
        int i;

        //Aca lee la cantidad de cuentas que existen en todo el banco         
        char *tc = malloc(sizeof(char));
        int counter = 0;
        
        for (i=0;i<strlen(mensaje);i++){
          if(mensaje[i]==' '){
            counter = 0;
            break;
          } else {
            counter++;
            tc = realloc(tc, sizeof(char)*counter);
            tc[counter-1] = mensaje[i];
          }
        }
        i++;
        
        //Aca lee la cantidad de cuentas que debe crear esta sucursal
        char *cc = malloc(sizeof(char));
        for (i; i<strlen(mensaje);i++){
          if(mensaje[i]==' '){
            counter = 0;
            break;}
          counter++;
          cc = realloc(cc, sizeof(char)*counter);
          cc[counter-1] = mensaje[i];
        }

        char* nc = malloc(sizeof(char));
        for (i; i<strlen(mensaje);i++){
          counter++;
          nc = realloc(nc, sizeof(char)*counter);
          nc[counter-1] = mensaje[i];
        }
        
        //Despliega información sobre la sucursal y guarda valores en variables
        //printf("En sucursal... bID:%d tc:%s cc:%s nc:%s\n", bankId, tc, cc, nc);
        
        cantidad_cuentas = atoi(cc);
        total_cuentas = atoi(tc);
        numero_child = atoi(nc)-1;
        printf("Tengo '%d' cuentas\n\n", cantidad_cuentas);
        //write(pipes[numero_child][1], "Hola", (strlen("Hola")+1));

        //Procede a crear las cuentas
        crear_cuentas(cantidad_cuentas, bankId, sucId, cuentas);
        
        while (true) {
          char* mensaje = codificar_desde_sucursal('d', cuentas[random_number(0,cantidad_cuentas)], random_number(0, total_cuentas), random_number(1, 5000000));
          //write(pipes[numero_child][1], mensaje, strlen(mensaje));
          //printf("En el while\n");
          bytes = read(pipes[numero_child][0], readbuffer, sizeof(readbuffer));
          mensaje = readbuffer;
          printf("Soy la sucursal '%d' y leí el mensaje: '%s'\n", sucId, mensaje);
          /*
          pthread_t p1, p2;
          int rc = 0;

          rc = pthread_create(&p1, NULL, enviar_como_emisor, info);
          rc = pthread_create(&p2, NULL, recibir_mensaje, info);
          */
          
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
      if (getpid() == 0)
      {
        dump_csv(movimientos, getpid()%1000);
        printf("Archivo creado\n");
      }
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
