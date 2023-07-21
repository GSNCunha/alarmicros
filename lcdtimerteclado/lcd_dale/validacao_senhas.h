/*
 * validacao_senhas.h
 *
 * Created: 21/07/2023 14:42:20
 *  Author: Vini
 */ 


#ifndef VALIDACAO_SENHAS_H_
#define VALIDACAO_SENHAS_H_

int nr_digitados;
char senha[6];

typedef struct Senhas
{
	char central[6];
	char usuario1[6];
	char usuario2[6];
	char adm[6];
	char intruso[6];
	
}Senhas;

Senhas senhas;

void lendo_senha(char tecla);

const char* validar_senha();

int resultado_validacao();




#endif /* VALIDACAO_SENHAS_H_ */