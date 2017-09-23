												Informe Tarea 1 parte 2 arpaci lovers


1. Funciones principales

	El main() es el primer proceso en ejecutar, en el cual, cuando se utiliza el }
	comando "init", se crea un thread utilizando la función init(), en el cual se 
	crea la nueva sucursal, es decir, un nuevo proceso.
	Este nuevo proceso -la sucursal- crea sus cuentas con create_accounts() y crea
	sus terminales, siendo cada una un thread nuevo que utiliza la función terminal().
	Por otro lado, la casa matriz maneja la comunicación entre ella y las sucursales
	utilizando un thread a parte, el pipe_handler().
	

2. Problemas

	Sabemos que el thread init() se queda atascado (no se cae) cuando se crea una
	segunda sucursal. Esto es debido, probablemente, a un mal uso de locks. Debido
	a esto, no se crean las cuentas de la nueva sucursal ni sus terminales, aunque
	el proceso de casa matriz sigue funcionando.
		
		
3. Complicaciones

	Debido a problemas de tiempo (considerando que tuvimos que hacer la entrega 
	desde cero), no implementamos la utilización de comandos list(), dump(), 
	dump_errs() y dump_accs(), a pesar de que las funciones correspondientes están 
	creadas.


