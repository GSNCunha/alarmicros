/*
 * deteccao_intruso.c
 *
 * Created: 22/07/2023 16:47:34
 *  Author: vitor
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timers.h"
char alarme_ativo = 0;
char intruso_detectado = 0;
char chamada = 0;
long tempo_TA = 25; //tempo de delay com o led piscando ---- tem q mudar pra 25s


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
}



void desativa_buzzer(){
	buzzer = 0;
	TCCR4B = 0x00; //desliga timer 4
	TIMSK4 = 0x00;
}