void sendMsj(int n){
	//enviar al pipe la cantidad 
	if(!strcm("init", commandBuff, strlen("init"))){
		char msj[1000000];
		char nn[999999];
		nn = n;
		strcpy(nn, msj)
		msj[0] = "r";//actualizar el numero para el random
		write(bankPipe[1], nn, 1000000);//faltaria hacerlo para todos los pipes
	}
}

void decode(char msj[64000]){
	char bufmsj[64000];
	read(pipeBank[0], bufmsj, sizeof(bufmsj));
	if(bufmsj[0] == "d"){
		//busca la cuenta de destino
	}//deposito, busca la buenta de destino
	else if(bufmsj[0] == "r"){
		//busca la sucursal 
		char a;
		char b[5];
		int d;
		a = getc(bufmsj);
		for(int i =0; i < strlen(bufmsj); i++){//busca la sucursal para enviarla al pipe
			if((a == ",") && ((a = getc()) == ",")){
				a = getc(bufmsj);
				while(a!=","){
					c[d] = a;
					d++;
					a = getc(bufmsj);
				}
			}
		}
		//enviar al pipe con el id de sucursal b[]
		write(bankPipe[1], bufmsj, sizeof(bufmsj));
	}//retiro, busca la sucursal de donde viene la cuenta
}


