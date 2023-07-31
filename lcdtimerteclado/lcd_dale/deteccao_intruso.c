#include <avr/io.h>
#include <avr/interrupt.h>
#include <ctype.h>
#include <string.h>
#include "lcdio.h"
#include "timers.h"
#include "teclado.h"
#include "validacao_senhas.h"
#include "deteccao_intruso.h"
#include "subRotinaAdm.h"
#include "serial.h"
#include "timerRelogio.h"

char alarme_ativo = 0;
char intruso_detectado = 0;
char chamada = 0;
long tempo_TA = 25; //tempo delay com o led piscando
char sensorAtivo;
int horarioSensor[3];


void delay_piscaled(){
	for(long i = 0; i<(tempo_TA*4); i++){
		if(alarme_ativo && intruso_detectado){
			delay_250ms();
			PORTB ^= (1 << 7); //bipa o led em 2hz
		}
	}
}



void ativa_buzzer(){
	while(alarme_ativo && intruso_detectado){
		for (long i = 0; i<400 ; i++){
			if (alarme_ativo && intruso_detectado){
				onda_400hz();
				PORTB ^= (1 << 6); //bipa o buzzer em 400hz por 1s
			}
		}
		for (long i = 0; i<800 ; i++){
			if (alarme_ativo && intruso_detectado){
				onda_800hz();
				PORTB ^= (1 << 6); //bipa o buzzer em 800hz por 1s
			}
		}
	}
	PORTB = 0;
}



void desativa_buzzer(){
	buzzer = 0;
	TCCR4B = 0x00; //desliga timer 4
	TIMSK4 = 0x00;
}

void enviaMotivo(){
	send_string("CAUSA: SINAL ");// mostra o motivo do sinal
	send_string(&sensorAtivo);
	proxima_linha();
	printa_horarioreal(horarioSensor[0], horarioSensor[1], horarioSensor[2]);//mostra o horário em que o sensor foi ativado
	delay_1s();
	delay_1s();
	delay_1s();
	telaMotivoAlarme();
	delay_1s();
	delay_1s();
	instrucaoDigitada = '\0';
	PCMSK2 = 0x01;//desligar interrupção das teclas dos números 
	telaSelecionarMotivoAlarme();
	while(1)
	{
		instrucaoDigitada = procuraTecla();
		if(instrucaoDigitada == '*')//opção alarme falso
		{
			enviarStringSerial("AMA");//enviar alarme falso e que os sensores não foram ativados 
			enviarStringSerial("AS");
			enviarByteSerial(0b0);
			break;
		}
		if(instrucaoDigitada == '#')//mais opções
		{
			instrucaoDigitada = '\0';
			telaOpcaoIntrusoOuInvadido();
			while(1)
			{
				instrucaoDigitada = procuraTecla();
				if(instrucaoDigitada == '*')//opção intruso no local
				{
					enviarStringSerial("AMI");
					break;
				}
				if(instrucaoDigitada == '#')//opçao constatação de invasão
				{
					enviarStringSerial("AMC");
					break;
				}
			}
			break;
		}
	}
		telaInicio();
	PCMSK2 = 0x0F;//ligar interrupção de todas as teclas
}