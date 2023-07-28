#include <avr/io.h>
#include <avr/interrupt.h>
#include <ctype.h>
#include <string.h>
#include "lcdio.h"
#include "timers.h"
#include "teclado.h"
#include "validacao_senhas.h"
#include "deteccao_intruso.h"
#include "subRotinaAdm.h"
#include "serial.h"
#include "timerRelogio.h"

char alarme_ativo;
char admLogin;
char sensorAtivo;
int horarioSensor[3];

void modoNoturno(){
	if ((modoNoturnoStatus == 1) && (hora >= 0) && (hora < 6)){
		if((hora == 0) && (min == 0)){
			alarme_ativo = 1;
			limpa_reseta_cursor();
			send_string("MODO NOTURNO ON");
			proxima_linha();
			send_string("senha:");
		}
	}
	if ((modoNoturnoStatus == 1) && (hora >= 6) && (hora < 24)){
		if((hora == 6) && (min == 0)){
			alarme_ativo = 0;
			limpa_reseta_cursor();
			send_string("MODO NOTURNO OFF");
			proxima_linha();
			send_string("senha:");
		}
	}
}

void telaLcd()
{
	limpa_reseta_cursor();
	send_string("* - ALARME FALSO");
	proxima_linha();
	send_string("# - MAIS OPCOES");
}
void telaLcd2()
{
	limpa_reseta_cursor();
	send_string("* Intruso local");
	proxima_linha();
	send_string("# Invadido");
}
void telaLcd3()
{
	limpa_reseta_cursor();
	send_string("MOTIVO DO ALARME");
}


void enviaMotivo(){
	send_string("CAUSA: SINAL ");
	send_string(&sensorAtivo);
	proxima_linha();
	printa_horarioreal(horarioSensor[0], horarioSensor[1], horarioSensor[2]);
	delay_1s();
	delay_1s();
	delay_1s();
	telaLcd3();
	delay_1s();
	delay_1s();
	instrucaoDigitada = '\0';
	PCMSK2 = 0x01;
		telaLcd();
		while(1)
		{
			instrucaoDigitada = procuraTecla();
			if(instrucaoDigitada == '*')
			{
				serialEnviarString("AMA");
				break;
			}
			if(instrucaoDigitada == '#')
			{
				instrucaoDigitada = '\0';
				telaLcd2();
				while(1)
				{
					instrucaoDigitada = procuraTecla();
					if(instrucaoDigitada == '*')
					{
						serialEnviarString("AMI");
						break;
					}
					if(instrucaoDigitada == '#')
					{
						serialEnviarString("AMC");
						break;
					}
				}
			break;
		}
	}
	limpa_reseta_cursor();
	send_string("---ALARMICROS---");
	proxima_linha();
	send_string("SENHA:");
	PCMSK2 = 0x0F;
}

int main(void)
{
	alarme_ativo = 0;
	admLogin = 0;
	//------------- organizacao das interrup�oes do teclado:
	DDRB = 0xFF;
	PORTB = 0;
	DDRK = 0b11110000; //  (1) PORTK(0-3) output / (0) PORTK(4-7) input
	PORTK = 0b00001111; //pull up do input ativado
	PCICR |= 0b00000100; //ativa interrup��o dos pinos PCINT16 ao PCINT23 = K0 ao K7
	PCMSK2 = 0x0F; //�nterrup��o nos pinos K0 at� K7;
	sei(); //set enable interrupts -- seta 1 no bit I do status register
	//-------------------------------
	serialInicializar();
	serialAtivarInterrupt();
	
	config_timer_ok();
	config_timer1();
	config_porta_avr();
	config_lcd_padrao();
	limpa_reseta_cursor();
	send_string("---ALARMICROS---");
	proxima_linha();
	send_string("SENHA:");
    while (1){
			if (alarme_ativo && intruso_detectado){
				sensorAtivo = tecla;
				horarioSensor[0] = dia;
				horarioSensor[1] = hora;
				horarioSensor[2] = min;
				delay_piscaled();
				if(alarme_ativo && intruso_detectado)
				{
					if(sensorAtivo == 'A'){
						serialEnviarString("AS");
						serialEnviarByte(0b1);
					}
					if(sensorAtivo == 'B'){
						serialEnviarString("AS");
						serialEnviarByte(0b10);
					} 
					if(sensorAtivo =='C'){
						serialEnviarString("AS");
						serialEnviarByte(0b100);
					}
					if(sensorAtivo =='D'){
						serialEnviarString("AS");
						serialEnviarByte(0b1000);
					}
				}
				if(alarme_ativo && intruso_detectado ){
					serialDesativarInterrupt();
					ativa_buzzer();
					enviaMotivo();
					serialAtivarInterrupt();
					PORTK = 0b00001111;
				}
			}
			
			if (admLogin == 1){
				subRotinaAdm();
				PCMSK2 = 0x0F;
				PORTK = 0b00001111;
				admLogin = 0;
				limpa_reseta_cursor();
				send_string("---ALARMICROS---");
				proxima_linha();
				send_string("SENHA:");
			}
		}	
		 //fechamento do while
}
	
ISR(PCINT2_vect){ //pinos K0 at� K4
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
				}
				if(nr_digitados == 5){
					if (resultado_validacao() == 1){
						alarme_ativo = 0;
						intruso_detectado = 0;
						limpa_reseta_cursor();
						send_string("ALARME OFF");
						proxima_linha();
						send_string("SENHA: ");
						delay_1s();
						}
				}
			}
		}else if ((tecla == '1') || (tecla == '2') || (tecla == '3') || (tecla == '4') || (tecla == '5') || (tecla == '6') || (tecla == '7') || (tecla == '8') || (tecla == '9') || (tecla == '0')){
		if(nr_digitados < 5){
			lendo_senha(tecla);
		}
		if(nr_digitados == 5){
			char result_val = resultado_validacao();
			if (result_val == 1){
				intruso_detectado = 0;
				TCCR1B = 0x05; //timer 1 pra dar o delay ate ativar o alarme //ativa o alarme na interrup��o do timer 1
				TIMSK1 = 1;
				limpa_reseta_cursor();
				send_string("ATIVANDO SISTEMA");
			}
			if (result_val == 2){
				admLogin = 1;
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
	if(nr_ciclos_timer1 == 4){ //mudar p 19 dps
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
	if (nr_ciclos_relogio == 13){//14-1
		nr_ciclos_relogio=0;
		min++;
		conversaoMinHr();
	}else{
		nr_ciclos_relogio++;
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
		configRelogio();
		//send_data(c2);
		dia = c;
		hora = c2;
		min = c3;
		
		
		//relogioSincronizar(c, c2, c3);
		
		// Resposta de confirma  o.
		if(!pediHorario){
			serialEnviarString("AH");
		}
		pediHorario = 0;
		
	}
	else if (c == 'S' || c == 'M') {
		// confirmacao de mensagem recebida
	}
}

ISR(TIMER4_OVF_vect){
	if (nr_ciclos_ok == 3){//14-1
		nr_ciclos_ok=0;
		serialEnviarString("AO"); //envia OK
		}else{
		nr_ciclos_ok++;
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