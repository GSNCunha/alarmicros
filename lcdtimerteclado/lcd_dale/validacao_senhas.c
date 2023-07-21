/*
 * validacao_senhas.c
 *
 * Created: 21/07/2023 14:41:58
 *  Author: Vini
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "teclado.h"
#include "lcdio.h"
#include "timers.h"

int nr_digitados = 0;
char senha[6] = {'\0', '\0', '\0', '\0', '\0', '\0'};

typedef struct Senhas 
{
	char central[6];
	char usuario1[6];
	char usuario2[6];
	char adm[6];
	char intruso[6];
	
}Senhas;

Senhas senhas = {"89134", "12369","32190","01267","90171"};


// void mudar_senha_usuario(char usuario){
// 	
// 	validar_senha("adm");
// 	
// 	
// }


void lendo_senha(char tecla){
			
	senha[nr_digitados] = tecla;
			
	//envia asterisco
	send_data(0x2A);
	nr_digitados++;
	
}

const char* validar_senha(){
	if(!strcmp(senha,senhas.usuario1)){
		return "usuario1";
	}else if(!strcmp(senha,senhas.usuario2)){
		return "usuario2";
	}else if(!strcmp(senha,senhas.adm)){
		return "adm";
	}else if(!strcmp(senha,senhas.central)){
		return "central";
	}else if(!strcmp(senha,senhas.intruso)){
		return "intruso";
	}else{
		return "senha incorreta";
	}
	
}

int resultado_validacao(){
	
	const char* validacao;
	
	validacao = validar_senha();
	
	if( !strcmp(validacao,"senha incorreta")){
		limpa_reseta_cursor();
		nr_digitados = 0;
		send_string("senha incorreta ");
		return 0;
	}else{
		limpa_reseta_cursor();
		nr_digitados = 0;
		send_string("senha correta");
		return 1;
	}	
}