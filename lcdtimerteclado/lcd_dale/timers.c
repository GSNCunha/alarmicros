
#include "timers.h"
#include <avr/io.h>
#include <avr/interrupt.h>

//OBS.: TIMERS SEM INTERRUPÇAO NO TIMER 5

//versão com interrupção:

int nr_ciclos_timer1 = 0;

char led2hz = 0;

char buzzer = 0;
long ciclos_buzzer = 0;

// onda = 0 = 400hz, onda = 1 = 800hz
char onda = 0;

void config_timer1(){
	sei();
	TCCR1A = 0; //modo normal
	TCNT1 = 26743; //40 vezes esse ciclo dá 80 segundos
	TIMSK1 = 1;
	TIFR1 = 1;
}

void onda_800hzint(){
	sei();
	TCCR4A = 0; //modo normal
	TCCR4B = 0x1; //sem prescaler
	TCNT4 = 55536; //como o processador tem 16Mhz e o timer tem 16bits, precisamos de 10000 contagens p bater meio periodo da onda de 800hz
	TIMSK4 = 1;
}
void onda_400hzint(){
	sei();
	TCCR4A = 0; //modo normal
	TCCR4B = 0x1; //sem prescaler
	TCNT4 = 45536; //como o processador tem 16Mhz e o timer tem 16bits, precisamos de 20000 contagens p bater meio periodo da onda de 400hz
	TIMSK4 = 1;
}
void led2hz_int(){
	sei();
	TCCR4A = 0; //modo normal
	TCCR4B = 0x3; //prescaler de 64
	TCNT4 = 3036; //como o processador tem 16Mhz e o timer tem 16bits, precisamos de 4000000 contagens p bater meio periodo da onda de 2hz. 4000000/64 = 62500
	TIMSK4 = 1;
	led2hz = 1;
}


//versão sem interrupção:
void onda_800hz(){
	TCCR5A = 0; //modo normal
	TCCR5B = 0x1; //sem prescaler
	TCNT5 = 55536; //como o processador tem 16Mhz e o timer tem 16bits, precisamos de 10000 contagens p bater meio periodo da onda de 800hz
	TIFR5 = (1<<0); //limpa flag
	while((TIFR5 & (1<<0)) == 0); //espera overflow
}
void onda_400hz(){
	TCCR5A = 0; //modo normal
	TCCR5B = 0x1; //sem prescaler
	TCNT5 = 45536; //como o processador tem 16Mhz e o timer tem 16bits, precisamos de 20000 contagens p bater meio periodo da onda de 400hz
	TIFR5 = (1<<0); //limpa flag
	while((TIFR5 & (1<<0)) == 0); //espera overflow
}
void delay_250ms(){
	TCCR5A = 0; //modo normal
	TCCR5B = 0x3; //prescaler de 64
	TCNT5 = 3036; //como o processador tem 16Mhz e o timer tem 16bits, precisamos de 4000000 contagens p bater meio periodo da onda de 2hz. 4000000/64 = 62500
	TIFR5 = (1<<0); //limpa flag
	while((TIFR5 & (1<<0)) == 0); //espera overflow
}

//esse delay acho q n vai precisar interrupção
void delay_1s(){
	TCCR5A = 0; //modo normal
	TCCR5B = 0x4; //prescaler de 256
	TCNT5 = 3036; //como o processador tem 16Mhz e o timer tem 16bits, precisamos de 4000000 contagens p bater meio periodo da onda de 2hz. 4000000/64 = 62500
	TIFR5 = (1<<0); //limpa flag
	while((TIFR5 & (1<<0)) == 0); //espera overflow
}

void delay_1ms() {
	TCCR5A = 0; // Modo normal
	TCCR5B = 0x4; // Prescaler de 256
	TCNT5 = 65024;
	TIFR5 = (1<<0); //limpa flag
	while((TIFR5 & (1<<0)) == 0); //espera overflow
}