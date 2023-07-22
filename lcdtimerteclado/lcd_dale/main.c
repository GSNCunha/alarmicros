#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcdio.h"
#include "timers.h"
#include "teclado.h"
#include "validacao_senhas.h"

char alarme_ativo = 1;
char intruso_detectado = 0;
char chamada = 0;

long tempo_TA = 5;

int main(void)
{
	//------------- organizacao das interrupçoes do teclado:
	DDRB = 0xFF;
	PORTB = 0;
	DDRK = 0b11110000; //  (1) PORTK(0-3) output / (0) PORTK(4-7) input
	PORTK = 0b00001111; //pull up do input ativado
	PCICR |= 0b00000100; //ativa interrupção dos pinos PCINT16 ao PCINT23 = K0 ao K7
	PCMSK2 |= 0x0F; //ínterrupção nos pinos K0 até K7;
	sei(); //set enable interrupts -- seta 1 no bit I do status register
	//-------------------------------
	config_porta_avr();
	config_lcd_padrao();
	limpa_reseta_cursor();
    while (1){
		if (alarme_ativo && intruso_detectado){
			for(long i = 0; i<(tempo_TA*4); i++){
				onda_2hz();
				PORTL ^= (1 << 0); //bipa o led em 2hz
			}
			while(alarme_ativo && intruso_detectado){
				for (long i = 0; i<400 ; i++){
					onda_400hz();
					PORTL ^= (1 << 1); //bipa o buzzer em 400hz por 1s
				}
				for (long i = 0; i<800 ; i++){
					onda_800hz();
					PORTL ^= (1 << 1); //bipa o buzzer em 800hz por 1s
				}
			}
		}
		
	}
}
	
ISR(PCINT2_vect){ //pinos K0 até K7
	if(chamada == 0){
		chamada = 1;
	send_data(0b00100011);
	tecla = procuraTecla();
	send_data(0b00100011);
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
		intruso_detectado = 1;
	}
	if ((tecla == 'D')){
		send_data(0b01000100);
		intruso_detectado = 0;
	}
	/*
	if (tecla == 'A'){
		
		limpa_reseta_cursor();
		for (int i = 0; i < 5; i++) {
			senha[i] = '\0';
		}
		nr_digitados = 0;
		
	}*//*else if(tecla != 'A'){
		if(nr_digitados < 5){
			lendo_senha(tecla);
	
		}
		if(nr_digitados == 5){
			resultado_validacao();
		}
	}*/
	
	//PCIFR = (1<<2); //reseta flag da interrupção
	delay_1s(); //resolve bug de n conseguir enviar as outras teclas dps
	}else{
		chamada = 0;
		PORTK = 0b00001111; //reseta a porta pra proxima leitura
	}
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