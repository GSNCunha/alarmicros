#include "lcdio.h"
#include <avr/io.h>
#include <avr/interrupt.h>

	//Utiliza PORTA A como saída para o lcd.
	//Formato:
	// A : [ 7 6 5 4 3 2 1 0]
	// 7 a 4:  envia informação(dados ou comandos)
	// 3 a 0: 0
	
	// RS: PC7 (0: comando,  1: dados)
	// Enable: PC6 
	
	void config_delay_timer0_CTC(char tempo){
		
		//Modo CTC
		TCCR0A = 0x02;
		
		//numero de clocks desejado
		OCR0A = tempo;
		
		//flag de interrupção
		TIFR0 = (1<<1); 
	}

	void delay_CTC()
	{
		//Começa em 0
		TCNT0 = 0;
		
		//limpa flag
		TIFR0 = (1<<1);
		
		 //liga o timer
		 TCCR0B = 0x01;
		
		while((TIFR0 & (1<<1)) == 0);
		
		//desliga timer
		TCCR0B = 0x00;
	}
	
	void config_porta_avr(){
		
		DDRA = 255; //Porta B irá funcionar como saída do AVR/entrada do Hitachi
		DDRC = 0b11100000;//PC0 e PC1 também serão saída do AVR/entrada do Hitachi
		
	}

	void trigger_EN()
	{
		config_delay_timer0_CTC(255);
		
		//coloca HIGH
		PORTC |= (1<<6);
		delay_CTC();
		delay_CTC();
		delay_CTC();
		
		//coloca LOW
		PORTC &= ~(1<<6);
		delay_CTC();
		delay_CTC();
		delay_CTC();
	}

	void send_data(char _data_)
	{
		config_delay_timer0_CTC(255);
		
		//RS = dados
		PORTC |= (1<<7);
		
		PORTA = (_data_ & 0xF0);
		trigger_EN();
		
		delay_CTC();
		delay_CTC();
		delay_CTC();
		

		PORTA = ((_data_<<4) & 0xF0);
		trigger_EN();

		delay_CTC();
		delay_CTC();
		delay_CTC();

	}

	void send_command(char command)
	{
		config_delay_timer0_CTC(255);
		
		//RS = comando
		PORTC &= ~(1<<7);

		PORTA = (command & 0xF0);
		trigger_EN();
		
		delay_CTC();
		delay_CTC();
		delay_CTC();

		PORTA = ((command<<4) & 0xF0);
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
		delay_CTC();
		delay_CTC();
		delay_CTC();
	}
	
	void config_lcd_padrao(){
		
		//Primeiro comando tem que ser em 8 bits
		
		//FUNCTION SET:
		//Interface 4 bits
		//2 linhas
		//Caracteres 5x10
		PORTC &= ~(1<<5);
		PORTC &= ~(1<<7);
		PORTA = 0x28;
		trigger_EN();
		delay_CTC();
		delay_CTC();
		delay_CTC();
		
		
		//ENTRY MODE SET:
		//Incrementa posição do cursor
		send_command(0x06);
		
		
		//CURSOR/DISPLAY SHIFT:
		//Cursor move
		//Para direita
		send_command(0x14);
		
		
		//DISPLAY CONTROL:
		//Display ligado
		//Cursor ligado
		//Cursor não pisca
		send_command(0x0E);
		
	}
	
	void proxima_linha(){
		send_command(0xC0);
		delay_CTC();
		delay_CTC();
		delay_CTC();
		}