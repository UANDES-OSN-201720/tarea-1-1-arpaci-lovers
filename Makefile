
# Este Makefile define los parametros de
# compilacion (CFLAGS) y reglas de compilacion
# para cada archivo objeto y el ejecutable final
# (bamain
CC=gcc
CFLAGS=-g -lm -std=c11 -Wall  -D _BSD_SOURCE -pthread 

all:tarea1

# Si se agregan archivos de codigo .c
# para cada uno se debe agregar regla
# como las siguientes:
tarea1: main.o
	$(CC) $(CFLAGS) -o tarea1 main.o -lm

main.o:main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f tarea1
