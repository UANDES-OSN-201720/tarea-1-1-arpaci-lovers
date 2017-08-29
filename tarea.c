#include <stdio.h>
#include <unistd.h>


int *sucursales;
int *cuentas;
struct sucursal{
	int id;
	struct cuenta *cts;
	struct movimiento *mov;
};
struct cuenta{
	int numero;//numero no tiene la id de la sucursal
	int balance;
};
struct movimiento{
	//int id;
	char[10] tipo;
	int origen;//es la cuenta con el numero de sucursal
	int destino;//idem^, pero puede ser vacio si tipo es retiro
	int monto;
	char[20] er-ex;//error-exito, error se puede considerar numero de cuenta invalido, para el random de transaccionesd
};
	
int main(int argv, char** argv){
	//la idea es hacer un "ciclo" de fork con exec(), luego cada vez que se genera una  nueva sucursal, cada sucursal genera structs para 
	//sus cuentas, entonces, cada sucursal tiene su propio arreglo de structs con las cuentas, y cada cuenta es un struct simple con
	//su estado de cuenta y numero. tambien cada sucursal tiene un struct con el historial de transacciones, sea deposito o retiro
	//este struct tiene: tipo de giro, cuenta origen, cuenta destino, monto, tipo de error o exito. Estos valores pueden ser separados
	//por coma o espacio. cada vez que se genera una nueva cuenta se hace un realloc, para no perder con la memoria. las transacciones
	//de cuentas se hacen con pipe a no ser que sea la misma cuenta. Y quedaria hacer los backups cuando los piden qque son simples 
	//operaciones I/O. Robustecer la tarea con verificaciones a mallocos, y llamadas de systema, pipe etc....  
	
	//para los pipes, se crean 2 pipes por proceso creado, el padre, cuando detecta info de un pipe, decodifica la instruccion, cuentas
	//y monto, para enviarle la instruccion a otro proceso. cada proceso se encarga de verificar si es posible la trasnsaccion, y debe enviar
	//el mensaje de exito o falla al proceso que empezo la transaccion, ambos procesos guardan la info en sus respectivos structs
	
	//hacer funcion que decodifique la info dell buffer, y comprueba que exista saldo suficiente
	//codifique la info y la solicitud de transferencia
	//hacer la funcion que "decodifique" y reenvie la solicitud a las otras sucursales 
	//hacer los pipes
	//hacer la funcion que genere la transaccion
	//hacer las funciones que requieren stdin
	//hacer las funciones que escriben archivos
	// if pid = 0, generar los structs correspondientes a sucursal, empezar a generar cuentas, luego transacciones. Cada vez que se 
	//detecte comunicacion del pipe, el proceso que inicia la tarea weitea a que sus pipe reciba la info del proceso que necesita
	//la info, para que ambos tengan la misma info si se realiza o no la operacion
	
	//usar una variable que guarde la cantidad de procesos creados, para qe el semaforo tenga claro, tambien se puede usa un
	//semaforo binario para que solo se corra una tarea a la vez en la relacion pedir, puente, responder, puente, recibir
	
	
}
