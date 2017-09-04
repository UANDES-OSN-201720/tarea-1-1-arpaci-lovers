												Informe Tarea 1 parte 1 arpaci lovers


1. Funciones principales

	Dentro de los distintos archivos .h que hicimos se presentan diferentes funciones que reciben los structs que componen la escencia de
	las cuentas y retornan un string con la información necesaria para transmitir de una sucursal a otra el mensaje que simboliza una transacción.
	Además se presentan funciones que decodifican estos strings en la casa matriz y permiten comunicar el mensaje a la sucursal apropiada a travez de pipes.
	Una vez que se crea la primera cuenta se pretendia que se comenzaran a crear transacciones libremente y que estas se codificaran y fueran
	transmitidas por los pipes.


2. Problemas

	Tuvimos varios problemas en crear un tipo de codificación util para poder incluir toda la información necesaria dentro del buffer para ser enviada a otro
	proceso. Los pipes que implementamos no funcionan ya que la información que deberian recibir no esta llegando de manera apropiada, por lo que todo el tema 
	de las transacciones no funciona. Sabemos que este era el fin de la tarea, y notamos que tenemos un entendimiento mucho más debil de lo que creiamos
	respcto a la aplicación de lo aprendido en clases, por lo que comenzaremos la tarea desde cero para poder entregar una buena segunda parte.
		
		
3. Complicaciones:


	No implementamos el uso de pipes ya que no pudimos entender bien el funcionamiento de estos y la estructura de codigo necesaria para que estos funcionaran.
	Debido a esto las transacciones no se realizan y el objetivo de la tarea no fue cumplido. Todas nuestras funciones de codificación y decodoficación estaban
	hechas para llegar ser enviadas por los pipes, pero nunca pudimos utilizarlas ya que no tenian lugar en nuestro main sin lo otro. Más que nada esto de debio 
	a falta de tiempo, pero por sobre todo por falta de coordinación por parte del grupo. Esperamos poder mejorar para la proxima entrega. 


