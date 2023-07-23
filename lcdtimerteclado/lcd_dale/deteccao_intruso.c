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
long tempo_TA = 5; //tempo de delay com o led piscando


void delay_piscaled(){
	DDRB = 0xFF;
	led2hz_int();
	for(long i = 0; i<(tempo_TA); i++){
		delay_1s();
	}
	led2hz = 0;
	TCCR4B = 0x00; //desliga timer 4
	TIMSK4 = 0x00;
}

void ativa_buzzer(){
	onda_400hzint();
	buzzer = 1;
}

void desativa_buzzer(){
	buzzer = 0;
	TCCR4B = 0x00; //desliga timer 4
	TIMSK4 = 0x00;
}