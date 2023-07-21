#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcdio.h"
#include "timers.h"
#include "teclado.h"
#include "validacao_senhas.h"

int main(void)
{
	//------------- organizacao das interrup�oes do teclado:
	//DDRB = 0xFF;
	DDRK = 0b11110000; //  (1) PORTK(0-3) output / (0) PORTK(4-7) input
	PORTK = 0b00001111; //pull up do input ativado
	sei(); //set enable interrupts -- seta 1 no bit I do status register
	PCICR |= 0b00000100; //ativa interrup��o dos pinos PCINT16 ao PCINT23 = K0 ao K7
	PCMSK2 |= 0x0F; //�nterrup��o nos pinos K0 at� K7;
	
	DDRL = 0x03; //L0 led e L1 buzzer como saida
	PORTL = 0x00;
	//-------------------------------
	config_porta_avr();
	config_lcd_padrao();
	limpa_reseta_cursor();
	//send_data(0x21);
    while (1){
	}
	}
ISR(PCINT2_vect){ //pinos K0 at� K7
	tecla = procuraTecla();
	
	/*
	if ((tecla == '1')){
		send_data(0b00110001);
	}
	if ((tecla == '2')){
		send_data(0b00110010);
	}
	if ((tecla == '3')){
		send_data(0b00110011);
	}
	if ((tecla == '4')){
		send_data(0b00110100);
	}
	if ((tecla == '5')){
		send_data(0b00110101);
	}
	if ((tecla == '6')){
		send_data(0b00110110);
	}
	if ((tecla == '7')){
		send_data(0b00110111);
	}
	if ((tecla == '8')){
		send_data(0b00111000);
	}
	if ((tecla == '9')){
		send_data(0b00111001);
	}
	if ((tecla == 'A')){
		send_data(0b01000001);
	}
	if ((tecla == 'B')){
		send_data(0b01000010);
	}
	if ((tecla == 'C')){
		send_data(0b01000011);
	}
	if ((tecla == 'D')){
		send_data(0b01000100);
	}
	*/
	if (tecla == 'A'){
		
		limpa_reseta_cursor();
		for (int i = 0; i < 5; i++) {
			senha[i] = '\0';
		}
		nr_digitados = 0;
		
	}else if(tecla != 'A'){
		if(nr_digitados < 5){
			lendo_senha(tecla);
	
		}
		if(nr_digitados == 5){
			resultado_validacao();
		}
	}
	
	//PCIFR = (1<<2); //reseta flag da interrup��o
	delay_1s(); //resolve bug de n conseguir enviar as outras teclas dps
}
ISR(TIMER1_OVF_vect){
	TCCR1A = 0; //modo normal
	TCCR1B = 0x1; //sem prescaler //se botar em 0 para o timer
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