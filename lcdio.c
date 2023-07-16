#include "lcdio.h"
#include <avr/io.h>
#include <avr/interrupt.h>

	//Utiliza PORTA B como saída para o lcd.
	//Formato:
	//[ 7 6 5 4 3 2 1 0]
	// 7 a 4:  envia informação(dados ou comandos)
	// 3: RS(0 para comandos, 1 para dados)
	// 2: zero sempre
	// 1: ENABLE
	// 0: zero sempre

	void config_delay_timer0_CTC(char tempo){
		//numero de clocks desejado
		OCR0A = tempo;
		
		//flag de interrupção
		TIFR0 |= (1<<1); 
	}

	void delay_CTC()
	{
		
		TCNT0 = 0;
		
		 //liga o timer
		 TCCR0A = 0x00;
		 TCCR0B = 0x01;
		
		while((TIFR0 & (1<<0)) == 0);
		
		//desliga timer
		TCCR0B = 0x00;
		
		//limpa flag
		TIFR0 |= (1<<1); 
	}
	
	void config_porta_avr(){
		
		DDRB = 255; //Porta B irá funcionar como saída do AVR/entrada do Hitachi
		
	}

	void trigger_EN()
	{
		//coloca HIGH
		PORTB |= (1<<1);
		
		//coloca LOW
		PORTB &= ~(1<<1);
	}

	void send_data(char _data_)
	{
		config_delay_timer0_CTC(255);
		
		
		//Seta RS = 1(serão comunicados dados)
		PORTB |= (1<<3);

		PORTB = (_data_ & 0xF0);
		PORTB |= (1<<3);
		trigger_EN();

		PORTB = ((_data_<<4) & 0xF0);
		PORTB |= (1<<3);
		trigger_EN();

		delay_CTC();
		delay_CTC();
		delay_CTC();

	}

	void send_command(char command)
	{
		config_delay_timer0_CTC(255);
		
		//Seta RS = 0(serão comunicados comandos)
		PORTB &= ~(1<<3);

		PORTB = (command & 0xF0);
		trigger_EN();

		PORTB = ((command<<4) & 0xF0);
		trigger_EN();

		delay_CTC();
		delay_CTC();
		delay_CTC();

	}

	void send_string(char *string){
		
		while(*string != 0){
			send_data(*string);
			string++;
		}
	}
	
	
	void limpa_reseta_cursor(){
		//Limpa display e reseta posição do cursor
		send_command(0x01);
	}
	
	void config_lcd_padrao(){
		
		//Coloca em modo 4 bits
		send_command(0x20);
		
		//Interface 4 bits
		//2 linhas
		//Caracteres 5x10
		send_command(0x28);
		
		//Display ligado
		//Cursor desligado
		//Cursor não pisca
		send_command(0x0C);
		
		//Incrementa posição do cursor
		//Cursor move
		send_command(0x06);
	}
	
	void proxima_linha(){
		
		//tem que ver direito isso dai
		
	}