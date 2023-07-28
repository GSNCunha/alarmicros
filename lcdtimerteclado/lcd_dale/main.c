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
	serialInicializar();
	serialAtivarInterrupt();
	
	
	config_timer1();
	config_porta_avr();
	config_lcd_padrao();
	limpa_reseta_cursor();
	send_string("---ALARMICROS---");
	proxima_linha();
	send_string("SENHA:");
    while (1){
		if (alarme_ativo && intruso_detectado){
			delay_piscaled();
			ativa_buzzer();
			PORTB = 0;
		}
	}
}
	
ISR(PCINT2_vect){ //pinos K0 até K4
	PORTB = 0; //so p nao dar bip no buzzer
	if(chamada == 0){
		chamada = 1;
		tecla = procuraTecla();
		if(alarme_ativo){
			if ((tecla == 'A') || (tecla == 'B') || (tecla == 'C') || (tecla == 'D')){
				limpa_reseta_cursor();
				send_string("INTRUSO");
				proxima_linha();
				send_string("SENHA:");
				intruso_detectado = 1;
			}
			if ((tecla == '1') || (tecla == '2') || (tecla == '3') || (tecla == '4') || (tecla == '5') || (tecla == '6') || (tecla == '7') || (tecla == '8') || (tecla == '9') || (tecla == '0')){
				if(nr_digitados < 5){
					lendo_senha(tecla);
					//send_data(alarme_ativo);
				}
				if(nr_digitados == 5){
					if (resultado_validacao() == 1){
						alarme_ativo = 0;
						intruso_detectado = 0;
						limpa_reseta_cursor();
						send_string("ALARME OFF");
						delay_1s();
						limpa_reseta_cursor();
						send_string("---ALARMICROS---");
						proxima_linha();
						send_string("SENHA:");
						}
				}
			}
		}else if ((tecla == '1') || (tecla == '2') || (tecla == '3') || (tecla == '4') || (tecla == '5') || (tecla == '6') || (tecla == '7') || (tecla == '8') || (tecla == '9') || (tecla == '0')){
		if(nr_digitados < 5){
			lendo_senha(tecla);
		}
		if(nr_digitados == 5){
			if (resultado_validacao() == 1){
				intruso_detectado = 0;
				TCCR1B = 0x05; //timer 1 pra dar o delay ate ativar o alarme //ativa o alarme na interrupção do timer 1
				TIMSK1 = 1;
				limpa_reseta_cursor();
				send_string("ATIVANDO SISTEMA");
			}
		}
	}
	delay_200ms();
	}else{
		chamada = 0;
		PORTK = 0b00001111; //reseta a porta pra proxima leitura
	}
}

ISR(TIMER1_OVF_vect){
	if(nr_ciclos_timer1 == 19){
		//ativa_alarme
		limpa_reseta_cursor();
		send_string("ALARME ATIVO");
		proxima_linha();
		send_string("SENHA:");
		alarme_ativo = 1;
		//desliga timer
		TCCR1B = 0x00;
		TIMSK1 = 0x00;
		//limpa flag
		TIFR1 = 1;
		nr_ciclos_timer1 = 0;
		}else{
		nr_ciclos_timer1++;
	}
}

ISR(TIMER3_OVF_vect){
	if(nr_ciclos_timer1 == 19){
		//ativa_alarme
		limpa_reseta_cursor();
		send_string("ALARME ATIVO");
		proxima_linha();
		send_string("SENHA:");
		alarme_ativo = 1;
		//desliga timer
		TCCR1B = 0x00;
		TIMSK1 = 0x00;
		//limpa flag
		TIFR1 = 1;
		nr_ciclos_timer1 = 0;
		}else{
		nr_ciclos_timer1++;
	}
}




ISR(USART0_RX_vect)//usamos
{
	// Recebe e interpreta uma mensagem do m dulo externo.
	// Recebe bytes at  encontrar um que n o   o identificador
	// de mensagem do m dulo externo ('M'). Durante opera  o normal, o primeiro
	// byte sempre ser  'M', e o segundo ser  outro caractere.
	// Para mensagens com respostas longas/vari veis, desativa interrupts
	// para que o c digo principal lide com os dados.
	uint8_t c;
	while ((c = serialReceberByte()) == 'S');
	if (c == 'H') {
		// Sincroniza  o de hor rio.
		c = serialReceberByte(); // Dia.
		uint8_t c2 = serialReceberByte(); // Hora.
		uint8_t c3 = serialReceberByte(); // Minuto.
		
		limpa_reseta_cursor();
		send_data(c);
		send_data(c2);
		send_data(c3);
		
		
		//relogioSincronizar(c, c2, c3);
		
		// Resposta de confirma  o.
		serialEnviarString("AH");
	}
	else if (c == 'S' || c == 'M') {
		// confirmacao de mensagem recebida
	}
}





















/*ISR(TIMER3_OVF_vect){
	TCCR3A = 0; //modo normal
	TCCR3B = 0x1; //sem prescaler
	TCNT3 = 45536; //como o processador tem 16Mhz e o timer tem 16bits, precisamos de 20000 contagens p bater meio periodo da onda de 400hz
	TIFR3 = 1;
}*//*
ISR(TIMER4_OVF_vect){
	//timer p 1s
	if(led2hz == 1){
		TCCR4A = 0; //modo normal
		TCCR4B = 0x3; //prescaler de 256
		TCNT4 = 3036; //como o processador tem 16Mhz e o timer tem 16bits, precisamos de 4000000 contagens p bater meio periodo da onda de 2hz. 4000000/64 = 62500
		TIFR4 = 1;
		PORTB ^= (1 << 7);
	}
	if(buzzer == 1){
		if(onda == 0){
			TCCR4A = 0; //modo normal
			TCCR4B = 0x1; //sem prescaler
			TCNT4 = 45536; //como o processador tem 16Mhz e o timer tem 16bits, precisamos de 20000 contagens p bater meio periodo da onda de 400hz
			TIFR4 = 1;
			PORTB ^= (1 << 6);
			ciclos_buzzer++;
			if(ciclos_buzzer == 400){
				ciclos_buzzer = 0;
				onda = 1;
			}
		}
		if(onda == 1){
			TCCR4A = 0; //modo normal
			TCCR4B = 0x1; //sem prescaler
			TCNT4 = 55536; //como o processador tem 16Mhz e o timer tem 16bits, precisamos de 10000 contagens p bater meio periodo da onda de 800hz
			TIFR4 = 1;
			PORTB ^= (1 << 6);
			ciclos_buzzer++;
			if(ciclos_buzzer == 800){
				ciclos_buzzer = 0;
				onda = 0;
			}
		}
		
	}
}*/