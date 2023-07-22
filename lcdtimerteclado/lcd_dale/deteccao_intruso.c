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
	for(long i = 0; i<(tempo_TA*4); i++){
		onda_2hz();
		PORTB ^= (1 << 7); //bipa o led em 2hz
	}
}

void ativa_buzzer(){
	while(alarme_ativo && intruso_detectado){
		for (long i = 0; i<400 ; i++){
			onda_400hz();
			PORTB ^= (1 << 6); //bipa o buzzer em 400hz por 1s
		}
		for (long i = 0; i<800 ; i++){
			onda_800hz();
			PORTB ^= (1 << 6); //bipa o buzzer em 800hz por 1s
		}
	}
}