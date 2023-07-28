/*
 * timerRelogio.c
 *
 * Created: 27/07/2023 23:45:48
 *  Author: vitor
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <ctype.h>
#include "lcdio.h"
#include "timers.h"
#include "teclado.h"
#include "validacao_senhas.h"
#include "deteccao_intruso.h"
#include "subRotinaAdm.h"
#include "serial.h"

int dia = 0;
int hora = 0;
int min = 0;
int nr_ciclos_relogio = 0; //vai ate 64

void configRelogio(){
	sei();
	TCCR3A = 0; //modo normal
	TCCR3B = 0x2; //prescaler de 8
	TCNT3 = 3036; //como o processador tem 16Mhz e o timer tem 16bits, precisamos de 4000000 contagens p bater meio periodo da onda de 2hz. 4000000/64 = 62500
	TIMSK3 = 1;
}

void conversaoMinHr(){
	if (min == 60){
		hora++;
		min = 0;
	}
	if (hora == 24){
		dia++;
		hora = 0;
		min = 0;
	}
	if (dia == 31){
		dia = 0;
		hora = 0;
		min = 0;
	}
}