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

uint8_t dia = 0;
uint8_t hora = 0;
uint8_t min = 0;
int nr_ciclos_relogio = 0; //vai ate 64

void configRelogio(){
	sei();
	TCCR3A = 0; //modo normal
	TCCR3B = 0x5; //prescaler de 1024
	TCNT3 = 0; //como o processador tem 16Mhz e o timer tem 16bits, precisamos de 4000000 contagens p bater meio periodo da onda de 2hz. 4000000/64 = 62500
	TIMSK3 = 1;
}

void conversaoMinHr(){
	if (min == 60){//incremeta dia, hora e minuto de acordo com a contagem dos minutos
		hora++;
		min = 0;
	}
	if (hora == 24){
		dia++;
		hora = 0;
		min = 0;
	}
	if (dia == 32){//no caso desse mes tem 31 dias, ogo botamos 32
		dia = 0;
		hora = 0;
		min = 0;
	}
	modoNoturno();
}