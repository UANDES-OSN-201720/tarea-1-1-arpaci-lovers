												Informe Tarea 1 parte 1 arpaci lovers


1. Implementción para guardar cuentas, movimientos, y tipo de procesos sucursales:


	La tarea se realizó sin utilizar exec(), por lo tanto, el código del proceso matriz y de los hijos comparten el mismo código.
	Para guardar las cuentas y los movimientos se utilizaron structs.


2. Funciones más importantes:


	2.1 Codificar y Decodificar:

		El proceso padre al recibir la información desde la consola, llama a la funcion correspondiente, y esa funcion llama a codificar,
		para luego ser enviada por medio de pipes a los procesos hijos. Estos, a su vez usan la función decodificar correspondiente, para
		acontinuación realizar los cambios solicitados o responder a una solicitud.
		Del mismo modo que el proceso padre, la sucursales codifican la informacion de transferencia, se la envian a la matriz, y esta 
		decodifica el mensaje, lee la información pertinente, para reenviar el mensaje a la sucursal correspondiente o retornar el mensaje
		con error si la sucursal no existe.
		
	2.2 Crear cuentas, Generar Movimiento:
	
		Cuando el proceso padre recibe el comando init, este envía a las sucursales un nuevo número actualizado de la cantidad de cuentas 
		existentes en el banco, para poder así generar transacciones al azar, sin que la mayoría sean inválidas poner cuenta inexistente.
		Las sucursales generar transacciones aleatorias, las cuales se las mandan mediante los pipes a la matriz, la cual verifica que la
		sucursal exista, actuando igualmente, la sucursal verifica que la cuenta que va a recibir un depósito, o se va a retirar fondos 
		sea posible, enviando el mensaje con exito o error devuelta a la matriz.
		
		
3. Complicaciones:


	 La mayor complejidad al realizar la entrega, fue la implementación de pipes. Principalmente en la asignación de los nombres de los 
	 pipes para cada sucursal, por lo que queda implementada pero no funcional.


