#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcdio.h"
#include "timers.h"
#include "teclado.h"
#include "validacao_senhas.h"
#include "deteccao_intruso.h"
#include "subRotinaAdm.h"

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
	config_timer1();
	config_porta_avr();
	config_lcd_padrao();
	limpa_reseta_cursor();
    while (1){
		if (alarme_ativo && intruso_detectado){
			delay_piscaled();
			ativa_buzzer();
		}
		
	}
}
	
ISR(PCINT2_vect){ //pinos K0 até K4
	PORTL = 0; //so p nao dar bip no buzzer
	if(chamada == 0){
		chamada = 1;
		tecla = procuraTecla();
		if ((tecla == '1')){
			//send_data(0b00110001);
		}
		if ((tecla == '2')){
			//send_data(0b00110010);
		}
		if ((tecla == '3')){
			//send_data(0b00110011);
		}
		if ((tecla == '4')){
			//send_data(0b00110100);
		}
		if ((tecla == '5')){
			//send_data(0b00110101);
		}
		if ((tecla == '6')){
			//send_data(0b00110110);
		}
		if ((tecla == '7')){
			//send_data(0b00110111);
		}
		if ((tecla == '8')){
			//send_data(0b00111000);
		}
		if ((tecla == '9')){
			//send_data(0b00111001);
		}
		/*if ((tecla == 'A')){
			send_data(0b01000001);
			if(alarme_ativo){
				intruso_detectado = 1;
			}
		}
		if ((tecla == 'B')){
			send_data(0b01000010);
			if(alarme_ativo){
				intruso_detectado = 1;
			}
		}
		if ((tecla == 'C')){
			send_data(0b01000011);
			intruso_detectado = 1;
			if(alarme_ativo){
				intruso_detectado = 1;
			}
		}
		if ((tecla == 'D')){
			send_data(0b01000100);
			if(alarme_ativo){
				intruso_detectado = 1;
			}
		}
		*/
		
		if(alarme_ativo){
			if ((tecla == 'A') || (tecla == 'B') || (tecla == 'C') || (tecla == 'D')){
				limpa_reseta_cursor();
				send_string("INTRUSO");
				intruso_detectado = 1;
			}/*else if ((tecla == '1') || (tecla == '2') || (tecla == '3') || (tecla == '4') || (tecla == '5') || (tecla == '6') || (tecla == '7') || (tecla == '8') || (tecla == '9')){
				if(nr_digitados < 5){
					lendo_senha(tecla);
				}
				if(nr_digitados == 5){
					if (resultado_validacao() == 1){
						alarme_ativo = 0; //o timer liga o alarme dps
						intruso_detectado = 0;
					}
				}
			}*/
		}else if ((tecla == '1') || (tecla == '2') || (tecla == '3') || (tecla == '4') || (tecla == '5') || (tecla == '6') || (tecla == '7') || (tecla == '8') || (tecla == '9') || (tecla == '0')){
		if(nr_digitados < 5){
			lendo_senha(tecla);
		}
		if(nr_digitados == 5){
			if (resultado_validacao() == 1){
				alarme_ativo = 0; //o timer liga o alarme dps
				intruso_detectado = 0;
			}
		}
	}
		delay_1s();
	}else{
		chamada = 0;
		PORTK = 0b00001111; //reseta a porta pra proxima leitura
	}
}

ISR(TIMER1_OVF_vect){
	if(nr_ciclos_timer1 == 1){
		
		//ativa_alarme
		limpa_reseta_cursor();
		send_string("ALARME ATIVO");
		alarme_ativo = 1;
		//desliga timer
		TCCR1B = 0x00;
		//limpa flag
		TIFR1 = 1;
		
		nr_ciclos_timer1 = 0;
		}else{
		nr_ciclos_timer1++;
		
	}
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