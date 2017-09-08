#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

typedef struct
{
	char* mensaje;
	int** pipes;
}info_util;

void *imprimir(void* string)
{
	printf("%s\n", (char *)string);
	return NULL;
}

void *decodificar_y_enviar(void* informacion)
{

}

int encontrar_indice(int* procesos, pid_t pid_hijo, int hijos_totales)
{
	for (int i=0; i<hijos_totales; i++)
	{
		if (procesos[i] == pid_hijo)
		{
			return i;
		}
	}
}

char** split(char* a_str, const char a_delim)
{
	char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

int decodificar_pid(char* mensaje)
{
	char pid_como_string[4];
	for (int i=0; i<4; i++)
	{
		pid_como_string[i] = mensaje[i];
	}
	return atoi(pid_como_string);
}


int main(int argc, char* argv[])
{
	int** pipes = malloc(sizeof(int*)); //Arreglo con todos los pipes.
	int hijos_totales = 0; //Contador de cuantos procesos hijos se han originado.
	int* procesos = malloc(sizeof(int)); //Arreglo con los PID de los procesos hijos.
	char readbuffer[80]; //Buffer
	
	for (int i=0; i<256; i++)
	{
		pipes[i] = malloc(sizeof(int)*2);
	}

	pipe(pipes[hijos_totales]);
	pipe(pipes[hijos_totales+1]);
	pid_t id_hijo = fork();
	//Proceso padre, este valor será el PID del hijo.
	//Proceso hijo, este valor será igual a cero.

	//----Proceso hijo
	if (id_hijo == 0)
	{
		printf("Hola, soy el hijo\n");
		char codigo[24];
		sprintf(codigo, "%d permiso para hablar.", getpid());
		write(pipes[hijos_totales][1], codigo, strlen(codigo));							//hijos_totales = 0
		int bytes = read(pipes[hijos_totales+1][0], readbuffer, sizeof(readbuffer));	//hijos_totales+1 = 1
		printf("%d: Mi padre me dijo '%s'\n", getpid(), readbuffer);
	}

	//----Proceso padre
	else
	{
		procesos[hijos_totales] = id_hijo;
		procesos = realloc(procesos, sizeof(procesos)+sizeof(int));
		hijos_totales++;
		printf("Hola, soy el padre.\n");
		//el padre tiene que recibir la información de todos los hijos (leyendo todos los pipes en un for) y
		//armar un struct info_utilcon el mensaje y el arreglo de pipes, y hacer un thread nuevo de lo decodifique
		//y envie al hijo correspondiente, entregando a esa funcion el struct como parametro en el pthreat_create.
		//Esto tiene que ir en un metodo (puntero a void) y recibir un struct con todo el input
		int bytes = read(pipes[hijos_totales-1][0], readbuffer, sizeof(readbuffer));
		pid_t quiere_hablar = decodificar_pid(readbuffer);
		printf("Mi hijo %d me dijo algo\n", quiere_hablar);
		int indice_deseado = encontrar_indice(procesos, quiere_hablar, hijos_totales);
		write(pipes[indice_deseado+1][1], "Hola hijo", 10);
	}

	/*pthread_t p1, p2;
	int rc;
	rc = pthread_create(&p1, NULL, imprimir, "Hola que tal."); assert(rc == 0);
	rc = pthread_create(&p2, NULL, imprimir, "Todo bien y tu."); assert(rc == 0);

	rc = pthread_join(p1, NULL); assert(rc == 0);
	rc = pthread_join(p2, NULL); assert(rc == 0);*/

	return 0;
}