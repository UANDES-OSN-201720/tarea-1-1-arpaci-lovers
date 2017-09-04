//generar dump_errs
// #include "header.h"
//---------------------------------------------------------------------------------
void dump_errss(movimiento** m, int id){
	FILE *f;
	char file[23] ="dump_errs_";
	char* t;
	sprintf(t, "%d", id);
	strcat(file, t);
	strcat(file, ".csv");
	f = fopen(file, "w");
	fprintf(f, "Error, Tipo Transaccion, Cuenta Origen, Cuenta Destino, Monto\n");
	for(int i = 0; m[i]->origen!=NULL; i++){
		if(m[i]->er_ex == 'f'){
			fprintf(f, "%i,", 1);
			fprintf(f, "%s,", m[i]->tipo);
			fprintf(f, "%s,", m[i]->origen);
			fprintf(f, "%s,", m[i]->destino);
			fprintf(f, "%i\n", m[i]->monto);
		}
		else if(m[i]->er_ex == 'i'){
			fprintf(f, "%i,", 2);
			fprintf(f, "%s,", m[i]->tipo);
			fprintf(f, "%s,", m[i]->origen);
			fprintf(f, "%s,", m[i]->destino);
			fprintf(f, "%i\n", m[i]->monto);
		}
	}
	fclose(f);
}

void dump_accss(cuenta** cc, int id){//listo
	FILE *f;
	char file[23] ="dump_accs_";
        char t_cont[9999] = "";
	char* t = t_cont;
	sprintf(t, "%d", id);
	strcat(file, t);
	strcat(file, ".csv");
	f = fopen(file, "w");
	fprintf(f, "Numero cuenta, Saldo\n");
	for(int i =0; cc[i]->codigo!=NULL; i++){
		fprintf(f, "%s,%i\n", cc[i]->codigo, cc[i]->saldo);
	}
	fclose(f);
}

//----------------------------------------------------------------------------------

