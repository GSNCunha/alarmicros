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
char admLogin;
	
	

typedef struct Senhas 
{
	char central[6]; //senha usuários 
	char usuario1[6];
	char usuario2[6];
	char adm[6];
	char intruso[6];
	
	char centralDefault[6];//senhas "de fábrica" para resetar caso solicitado
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
	PCMSK2 = 0x01;
	for (int i = 0; i < 5; i++) {//reseta a variável senha
		senha[i] = '\0';
	}
	nr_digitados = 0;

	while(nr_digitados < 5)//le 5 digitos escritos pelo usuário
	{
		if(nr_digitados < 5){
			tecla = procuraTecla();
			delay_200ms();
			delay_200ms();
			PORTK = 0b00001111;
			if ((tecla == '1') || (tecla == '2') || (tecla == '3') || (tecla == '4') || (tecla == '5') || (tecla == '6') || (tecla == '7') || (tecla == '8') || (tecla == '9') || (tecla == '0'))// não permite digitos invalidos para a senha
			{
				lendo_senha(tecla);
			}
		}
	}

	if(strcmp(senha,senhas.adm) && strcmp(senha,senhas.central) && strcmp(senha,senhas.intruso) && strcmp(senha,senhas.usuario1) && strcmp(senha,senhas.usuario2))
		return senha;

	return "INVAL";//caso a senha seja uma ja usada pelo sistemas, retornamos INVAL de invalida
}

const char* validar_senha(){ //compara senha digitada com senhas guardadas no sistema
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
		return "senha incorreta";// se não for nenhuma das anteriores, é uma senha invalida
	}
	
}

int resultado_validacao(){
	
	validacao = validar_senha();
	
	if(!strcmp(validacao,"senha incorreta")){
		limpa_reseta_cursor();
		nr_digitados = 0;
		telaSenhaIncorreta();
		return 0;//caso incorreta, o sistema não permite ações de usuário e pede novamente uma senha
	}else{
		limpa_reseta_cursor();
		nr_digitados = 0;
		telaSenhaCorreta();
		delay_1s();
		
		if(!strcmp(validacao,"central"))//caso a senha seja da central, automaticamente vamos enviar que o segurança esta no local para a central
		{
			enviarStringSerial("AMV");
		}
		if(!strcmp(validacao,"intruso"))//caso a senha seja do intruso, automaticamente vamos enviar que o intruso esta no local
		{
			enviarStringSerial("AMI");
		}
		
		if (strcmp(validacao, "adm") == 0){
			telaBemVindoAdm();
			delay_1s();
			admLogin = 1;
			// chama rotina do adm no main
			return 2; // p nao ativar o alarme quando for adm
		}
		return 1;
	}	
}