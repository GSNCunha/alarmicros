/*
 * projeto_micros.c
 *
 * Created: 13/07/2023 14:49:10
 * Author : vitor
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

char qual_delay; 
/*
1 = onda 800hz
2 = onda 400hz
3 = onda 2hz
*/
//versão com interrupção:
void onda_800hzint(){
	sei();
	TCCR1A = 0; //modo normal
	TCCR1B = 0x1; //sem prescaler
	TCNT1 = 55536; //como o processador tem 16Mhz e o timer tem 16bits, precisamos de 10000 contagens p bater meio periodo da onda de 800hz
	TIMSK1 = 1;
	qual_delay = 1;
}
void onda_400hzint(){
	sei();
	TCCR1A = 0; //modo normal
	TCCR1B = 0x1; //sem prescaler
	TCNT1 = 45536; //como o processador tem 16Mhz e o timer tem 16bits, precisamos de 20000 contagens p bater meio periodo da onda de 400hz
	TIMSK1 = 1;
	qual_delay = 2;
}
void onda_2hzint(){
	sei();
	TCCR1A = 0; //modo normal
	TCCR1B = 0x3; //prescaler de 64
	TCNT1 = 3036; //como o processador tem 16Mhz e o timer tem 16bits, precisamos de 4000000 contagens p bater meio periodo da onda de 2hz. 4000000/64 = 62500
	TIMSK1 = 1;
	qual_delay = 3;
}

//versão sem interrupção:
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


//esse delay acho q n vai precisar interrupção
void delay_1s(){
	TCCR1A = 0; //modo normal
	TCCR1B = 0x4; //prescaler de 256
	TCNT1 = 3036; //como o processador tem 16Mhz e o timer tem 16bits, precisamos de 4000000 contagens p bater meio periodo da onda de 2hz. 4000000/64 = 62500
	TIFR1 = (1<<0); //limpa flag
	while((TIFR1 & (1<<0)) == 0); //espera overflow
}


ISR(TIMER1_OVF_vect){
	if(qual_delay == 1 ){
		TCCR1A = 0; //modo normal
		TCCR1B = 0x1; //sem prescaler
		TCNT1 = 55536; //como o processador tem 16Mhz e o timer tem 16bits, precisamos de 10000 contagens p bater meio periodo da onda de 800hz
		TIFR1 = 1;
		PORTB ^= (1 << 7);
	}
	if(qual_delay == 2){
		TCCR1A = 0; //modo normal
		TCCR1B = 0x1; //sem prescaler
		TCNT1 = 45536; //como o processador tem 16Mhz e o timer tem 16bits, precisamos de 20000 contagens p bater meio periodo da onda de 400hz
		TIFR1 = 1;
		PORTB ^= (1 << 7);
	}
	if(qual_delay == 3){
		TCCR1A = 0; //modo normal
		TCCR1B = 0x3; //prescaler de 64
		TCNT1 = 3036; //como o processador tem 16Mhz e o timer tem 16bits, precisamos de 4000000 contagens p bater meio periodo da onda de 2hz. 4000000/64 = 62500
		TIFR1 = 1;
		PORTB ^= (1 << 7);
	}
}

int main(void)
{
	DDRB = (1 << 7); //b7 como saida
	onda_800hzint();
    while (1) 
    {
    }
}