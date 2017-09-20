
# Este Makefile define los parametros de
# compilacion (CFLAGS) y reglas de compilacion
# para cada archivo objeto y el ejecutable final
# (bamain
CC=gcc
CFLAGS=-g -std=c11 -Wall -Werror -pthread 

all:tarea1

# Si se agregan archivos de codigo .c
# para cada uno se debe agregar regla
# como las siguientes:
tarea1: main2.o funcionesAlf.o init.o pipe_handler.o
	$(CC) $(CFLAGS) -o tarea1 main2.o funcionesAlf.o init.o pipe_handler.o -lm 

main2.o: main2.c
	$(CC) $(CFLAGS) -c main2.c

funcionesAlf.o: funcionesAlf.c
	$(CC) $(CFLAGS) -c funcionesAlf.c

init.o: init.c
	$(CC) $(CFLAGS) -c init.c
	
pipe_handler.o: pipe_handler.c
	$(CC) $(CFLAGS) -c pipe_handler.c

clean:
	rm -f tarea1 *.o
