 /*
 * validacao_senhas.c
 *
 * Created: 21/07/2023 14:41:58
 *  Author: Vini
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "teclado.h"
#include "lcdio.h"
#include "timers.h"
#include "deteccao_intruso.h"
#include "subRotinaAdm.h"
#include "serial.h"

int nr_digitados = 0;
char senha[6] = {'\0', '\0', '\0', '\0', '\0', '\0'};
const char* validacao;	
	
	

typedef struct Senhas 
{
	char central[6];
	char usuario1[6];
	char usuario2[6];
	char adm[6];
	char intruso[6];
	
	char centralDefault[6];
	char usuario1Default[6];
	char usuario2Default[6];
	char admDefault[6];
	char intrusoDefault[6];
	
}Senhas;

Senhas senhas = {"89134", "12369","32190","01267","90171", "89134", "12369","32190","01267","90171"};


void lendo_senha(char tecla){
			
	senha[nr_digitados] = tecla;
			
	//envia asterisco
	send_data(0x2A);
	nr_digitados++;
	
}

const char* subRotinaTrocaSenha()
{
	for (int i = 0; i < 5; i++) {
		senha[i] = '\0';
	}
	nr_digitados = 0;

	while(nr_digitados < 5)
	{
		if(nr_digitados < 5){
			tecla = procuraTecla();
			delay_200ms(); //nao tirar
			delay_200ms();
			PORTK = 0b00001111;
			lendo_senha(tecla);
		}
	}

	if(strcmp(senha,senhas.adm) && strcmp(senha,senhas.central) && strcmp(senha,senhas.intruso) && strcmp(senha,senhas.usuario1) && strcmp(senha,senhas.usuario2))
		return senha;

	return "INVAL";
}

const char* validar_senha(){
	if((!strcmp(senha,senhas.usuario1)) && usuario1Status){
		return "usuario1";
	}else if((!strcmp(senha,senhas.usuario2)) && usuario2Status){
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
	
	validacao = validar_senha();
	
	if(!strcmp(validacao,"senha incorreta")){
		limpa_reseta_cursor();
		nr_digitados = 0;
		send_string("SENHA INCORRETA");
		proxima_linha();
		send_string("SENHA:");
		return 0;
	}else{
		limpa_reseta_cursor();
		nr_digitados = 0;
		send_string("SENHA CORRETA");
		proxima_linha();
		
		if(!strcmp(validacao,"central"))
		{
			serialEnviarString("AMV");
		}
		if(!strcmp(validacao,"intruso"))
		{
			serialEnviarString("AMI");
		}
		
		if (strcmp(validacao, "adm") == 0){
			send_string("BEM-VINDO ADM");
			delay_1s();
			// chama rotina do adm no main
			return 2; // p nao ativar o alarme quando for adm
		}
		return 1;
	}	
}