#ifndef
#include "ondas_quadradas.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void onda_800hz(){
	TCCR1A = 0; //modo normal
	TCCR1B = 0x1; //sem prescaler 
	TCNT1 = 55536; //como o processador tem 16Mhz e o timer tem 16bits, precisamos de 10000 contagens p bater meio periodo da onda de 800hz 
	TIFR1 = (1<<0); //limpa flag
	while((TIFR1 & (1<<0)) == 0); //espera overflow
}
void onda_400hz(){
	TCCR1A = 0; //modo normal
	TCCR1B = 0x1; //sem prescaler
	TCNT1 = 45536; //como o processador tem 16Mhz e o timer tem 16bits, precisamos de 20000 contagens p bater meio periodo da onda de 400hz
	TIFR1 = (1<<0); //limpa flag
	while((TIFR1 & (1<<0)) == 0); //espera overflow
}
void onda_2hz(){
	TCCR1A = 0; //modo normal
	TCCR1B = 0x3; //prescaler de 64
	TCNT1 = 3036; //como o processador tem 16Mhz e o timer tem 16bits, precisamos de 4000000 contagens p bater meio periodo da onda de 2hz. 4000000/64 = 62500
	TIFR1 = (1<<0); //limpa flag
	while((TIFR1 & (1<<0)) == 0); //espera overflow
}
#endif