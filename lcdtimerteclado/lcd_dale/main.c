#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcdio.h"
#include "timers.h"
#include "teclado.h"
int main(void)
{
	//------------- organizacao das interrupçoes do teclado:
	DDRB = 0xFF;
	DDRK = 0b11110000; //  (1) PORTK(0-3) output / (0) PORTK(4-7) input
	PORTK = 0b00001111; //pull up do input ativado
	sei(); //set enable interrupts -- seta 1 no bit I do status register
	PCICR |= 0b00000100; //ativa interrupção dos pinos PCINT16 ao PCINT23 = K0 ao K7
	PCMSK2 |= 0x01; //ínterrupção só no pino K0;
	//-------------------------------
	
	config_porta_avr();
	config_lcd_padrao();
	limpa_reseta_cursor();
	delay_1s();
	send_data(0x30);
	send_command(0xC0);
	send_data(0x41);
	send_data(0x21);
	send_data(0x21);
	send_data(0x21);
    while (1) 
    {
		tecla = procuraTecla();
		if (tecla == '1'){
			send_data(0x2A);
		}
	}
}

ISR(PCINT2_vect){ //pinos K0 até K7
	tecla = procuraTecla();
	if (tecla == 'A'){
		send_data(0x41);
	}
	PCIFR = (1<<2); //reseta flag da interrupção
	delay_1ms(); //resolve bug de n conseguir enviar as outras teclas dps
}
ISR(TIMER1_OVF_vect){
	TCCR1A = 0; //modo normal
	TCCR1B = 0x1; //sem prescaler
	TCNT1 = 55536; //como o processador tem 16Mhz e o timer tem 16bits, precisamos de 10000 contagens p bater meio periodo da onda de 800hz
	TIFR1 = 1;
}
ISR(TIMER3_OVF_vect){
	TCCR3A = 0; //modo normal
	TCCR3B = 0x1; //sem prescaler
	TCNT3 = 45536; //como o processador tem 16Mhz e o timer tem 16bits, precisamos de 20000 contagens p bater meio periodo da onda de 400hz
	TIFR3 = 1;
}
ISR(TIMER4_OVF_vect){
	TCCR4A = 0; //modo normal
	TCCR4B = 0x3; //prescaler de 64
	TCNT4 = 3036; //como o processador tem 16Mhz e o timer tem 16bits, precisamos de 4000000 contagens p bater meio periodo da onda de 2hz. 4000000/64 = 62500
	TIFR4 = 1;
}