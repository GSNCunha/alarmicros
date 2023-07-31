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
char esperaAlarme = 1;

void modoNoturno(){
	if ((modoNoturnoStatus == 1) && (hora >= 0) && (hora < 6)){//se hora esta dentro dos limites do modo noturno, o alarme é ativado automaticamente, modonoturnostatus é alterado na subRotinaAdm
		if((hora == 0) && (min == 0)){
			alarme_ativo = 1;
			telaModoNoturnoOn();
		}
	}
	if ((modoNoturnoStatus == 1) && (hora >= 6) && (hora < 24)){//caso esteja fora do horário para o modo noturno, o alarme do modo noturno é desligado
		if((hora == 6) && (min == 0)){
			alarme_ativo = 0;
			telaModoNoturnoOff();
		}
	}
}

int main(void)
{
	alarme_ativo = 0;
	admLogin = 0;
	//------------- organizacao das interrupçoes do teclado:
	DDRB = 0xFF;
	PORTB = 0;
	DDRK = 0b11110000; //  (1) PORTK(0-3) output / (0) PORTK(4-7) input
	PORTK = 0b00001111; //pull up do input ativado
	PCICR |= 0b00000100; //ativa interrupção dos pinos PCINT16 ao PCINT23 = K0 ao K7
	PCMSK2 = 0x0F; //ínterrupção nos pinos K0 até K7;
	sei(); //set enable interrupts -- seta 1 no bit I do status register
	//-------------------------------
	inicializarSerial();
	ativarSerialInterrupt();
	
	config_timer_ok();
	config_timer1();
	config_porta_avr();
	config_lcd_padrao();
	telaInicio();
    while (1){
			if (alarme_ativo && intruso_detectado){//caso um sensor seja ativado e o alarme esteja ativo
				sensorAtivo = tecla;
				horarioSensor[0] = dia;//guarda o horário que a o sensor foi ativado
				horarioSensor[1] = hora;
				horarioSensor[2] = min;
				delay_piscaled();//pisca os leds durante 25 segundos antes de ativar o alarm
				if(alarme_ativo && intruso_detectado)//caso nenhum usuário tenha desligado o alarme durante o tempo de 25seg
				{
					esperaAlarme=0; // tempo sem som de alarme acabou
					
					if(sensorAtivo == 'A'){//enviamos para a central o aviso de que sensor foi ativado 
						enviarStringSerial("AS");
						enviarByteSerial(0b1);
					}
					if(sensorAtivo == 'B'){
						enviarStringSerial("AS");
						enviarByteSerial(0b10);
					} 
					if(sensorAtivo =='C'){
						enviarStringSerial("AS");
						enviarByteSerial(0b100);
					}
					if(sensorAtivo =='D'){
						enviarStringSerial("AS");
						enviarByteSerial(0b1000);
					}
				}
				if(alarme_ativo && intruso_detectado ){
					desativarSerialInterrupt();//desligamos o interrupt para não afetar o funcionamento do som do alarme
					ativa_buzzer();
					if(strcmp(senha,senhas.intruso))//caso a senha seja do intruso, não mandamos  mais motivos para a central
					{
						enviaMotivo();
					}else
					{
						telaInicio();
					}
					UCSR0B &= ~(1 << RXEN0);
					UCSR0B |= (1 << RXEN0);
					ativarSerialInterrupt();
					PORTK = 0b00001111;
				}
				esperaAlarme = 1;
			}
			
			if (admLogin == 1){//caso o estado da variavel admLogin for 1, entramos na subrotina do administrador, essa variaável é mutada em validacao_senhas
				subRotinaAdm();
				PCMSK2 = 0x0F;
				PORTK = 0b00001111;
				admLogin = 0;
				telaInicio();
			}
		}	
		 //fechamento do while
}
	
ISR(PCINT2_vect){ //pinos K0 até K4
	PORTB = 0; 
	if(chamada == 0){//caso a senha seja para desligar o alarme
		chamada = 1;
		tecla = procuraTecla();
		if(alarme_ativo){
			if ((tecla == 'A') || (tecla == 'B') || (tecla == 'C') || (tecla == 'D')){//nesses digitos, o significado é que um sensor foi ativado 
				telaSensorAtivado();
				intruso_detectado = 1;
			}
			if ((tecla == '1') || (tecla == '2') || (tecla == '3') || (tecla == '4') || (tecla == '5') || (tecla == '6') || (tecla == '7') || (tecla == '8') || (tecla == '9') || (tecla == '0')){
				if(nr_digitados < 5){//digitos numericos somam para resultar em uma senha
					lendo_senha(tecla);
				}
				if(nr_digitados == 5){//caso a senha tenha sido digitada por inteiro, vamos conferir seu resultado
					if (resultado_validacao() == 1){
						alarme_ativo = 0;
						intruso_detectado = 0;
						telaAlarmeOff();
						delay_1s();
						if(esperaAlarme)//só preciso ir para a tela inicial aqui se eu ainda não tiver ligado meu buzzer 
							telaInicio();
						}
				}
			}
		}else if ((tecla == '1') || (tecla == '2') || (tecla == '3') || (tecla == '4') || (tecla == '5') || (tecla == '6') || (tecla == '7') || (tecla == '8') || (tecla == '9') || (tecla == '0')){
		if(nr_digitados < 5){//caso a senha seja para desligar o alarme// digitos numericos somam para resultar em uma senha
			lendo_senha(tecla);
		}
		if(nr_digitados == 5){//caso a senha tenha sido digitada por inteiro, vamos conferir seu resultado
			char result_val = resultado_validacao();
			if (result_val == 1 && strcmp(validacao,"central")){// o vigilante apenas avisa que chegou, nao dispara um alarme 
				intruso_detectado = 0;
				TCCR1B = 0x05; //timer 1 pra dar o delay ate ativar o alarme //ativa o alarme na interrupção do timer 1
				TIMSK1 = 1;
				telaATivandoOSistema();
			}else if(!strcmp(validacao,"central"))
			{
				telaInicio();
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
	if(nr_ciclos_timer1 == 5){ //mudar para 19 para o tempo de espera ficar em 1:20min, o numero esta menor para facilitar testes
		//ativa_alarme
		telaAlarmeAtivo();
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
		conversaoMinHr();//convertemos a soma de minutos em horas e dias
	}else{
		nr_ciclos_relogio++;
	}
}

ISR(USART0_RX_vect)//recepção serial 
{
	uint8_t c;
	while ((c = receberByteSerial()) == 'S');//a variável c sai do while com o segundo digito da mensagem mandada pelo professor 
	if (c == 'H') {//caso 'H', estamos recebendo horário
		// Sincroniza  o de hor rio.
		c = receberByteSerial(); // Dia.
		uint8_t c2 = receberByteSerial(); // Hora.
		uint8_t c3 = receberByteSerial(); // Minuto.
		configRelogio();
		//send_data(c2);
		dia = c;
		hora = c2;
		min = c3;
		
		// Resposta de confirma  o.
		if(!pediHorario){
			enviarStringSerial("AH");//menagem confirmação recebimento
		}
		pediHorario = 0;
		
	}
	else if (c == 'S' || c == 'M') {//caso de confirmação da central que as mensgens do alarme foram recebidas 
		// confirmacao de mensagem recebida
	}
}

ISR(TIMER4_OVF_vect){
	if (nr_ciclos_ok == 3){
		nr_ciclos_ok=0;
		enviarStringSerial("AO"); //envia OK periodicamente 
		}else{
		nr_ciclos_ok++;
	}
}