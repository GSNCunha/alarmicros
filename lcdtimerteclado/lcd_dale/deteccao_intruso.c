/*
 * deteccao_intruso.c
 *
 * Created: 22/07/2023 16:47:34
 *  Author: vitor
 */ 
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
long tempo_TA = 25; //tempo de delay com o led piscando ---- tem q mudar pra 25s
char sensorAtivo;
int horarioSensor[3];


/*void delay_piscaled(){
	DDRB = 0xFF;
	led2hz_int();
	for(long i = 0; i<(tempo_TA); i++){
		delay_1s();
	}
	led2hz = 0;
	TCCR4B = 0x00; //desliga timer 4
	TIMSK4 = 0x00;
}*/


void delay_piscaled(){
	for(long i = 0; i<(tempo_TA*4); i++){
		if(alarme_ativo && intruso_detectado){
			delay_250ms();
			PORTB ^= (1 << 7); //bipa o led em 2hz
		}
	}
}


/*void ativa_buzzer(){
	onda_400hzint();
	buzzer = 1;
}*/
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
	send_string("CAUSA: SINAL ");
	send_string(&sensorAtivo);
	proxima_linha();
	printa_horarioreal(horarioSensor[0], horarioSensor[1], horarioSensor[2]);
	delay_1s();
	delay_1s();
	delay_1s();
	telaLcd3();
	delay_1s();
	delay_1s();
	instrucaoDigitada = '\0';
	PCMSK2 = 0x01;
	telaLcd();
	while(1)
	{
		instrucaoDigitada = procuraTecla();
		if(instrucaoDigitada == '*')
		{
			serialEnviarString("AMA");
			serialEnviarString("AS");
			serialEnviarByte(0b0);
			break;
		}
		if(instrucaoDigitada == '#')
		{
			instrucaoDigitada = '\0';
			telaLcd2();
			while(1)
			{
				instrucaoDigitada = procuraTecla();
				if(instrucaoDigitada == '*')
				{
					serialEnviarString("AMI");
					break;
				}
				if(instrucaoDigitada == '#')
				{
					serialEnviarString("AMC");
					break;
				}
			}
			break;
		}
	}
	limpa_reseta_cursor();
	send_string("---ALARMICROS---");
	proxima_linha();
	send_string("SENHA:");
	PCMSK2 = 0x0F;
}