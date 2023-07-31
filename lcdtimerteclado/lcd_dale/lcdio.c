#include "lcdio.h"
#include "timers.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "subRotinaAdm.h"

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
		delay_1s();
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
		
		
		///TELAS///
		
	void telaSelecionarMotivoAlarme()
	{
		limpa_reseta_cursor();
		send_string("* - ALARME FALSO");
		proxima_linha();
		send_string("# - MAIS OPCOES");
	}
	
	void telaOpcaoIntrusoOuInvadido()
	{
		limpa_reseta_cursor();
		send_string("* Intruso local");
		proxima_linha();
		send_string("# Invadido");
	}
	
	void telaMotivoAlarme()
	{
		limpa_reseta_cursor();
		send_string("MOTIVO DO ALARME");
	}
	
	void printa_horarioreal(int diaAtual, int horaAtual, int minAtual){
		limpa_reseta_cursor();
		send_string("data: ");
		escreveNumero(diaAtual);
		send_string(":");
		escreveNumero(horaAtual);
		send_string(":");
		escreveNumero(minAtual);
	}

	void telaSenhasOuMais()
	{
		limpa_reseta_cursor();
		send_string("* SENHAS");
		proxima_linha();
		send_string("# MAIS OPCOES");
	}
	
	void telaEstadosOuMais()
	{
		limpa_reseta_cursor();
		send_string("* ESTADO USUARIO");
		proxima_linha();
		send_string("# MAIS OPCOES");
		
	}
	void telaHorasOuMais()
	{
		limpa_reseta_cursor();
		send_string("* PEDIR HORAS");
		proxima_linha();
		send_string("# MAIS OPCOES");
	}

	void telaModNotOuSair()
	{
		limpa_reseta_cursor();
		send_string("* ");
		if(!modoNoturnoStatus)
		{
			send_string("HAB");
		}else
		{
			send_string("DES");
		}
		send_string(" MODO NOT");
		proxima_linha();
		send_string("# SAIR");
	}

	void telaSenhaInvalida()
	{
		limpa_reseta_cursor();
		send_string("SENHA INVALIDA");
	}

	void telaTrocaSenhas()
	{
		limpa_reseta_cursor();
		send_string("1/2 - USUARIO1/2");
		proxima_linha();
		send_string("* - ADM");
	}

	void telaDeStatusUsuario()
	{
		limpa_reseta_cursor();
		send_string("1 - ");
		if(!usuario1Status)
		{
			send_string("HAB");//TELA PARA HABILITAR USUARO 1
		}else
		{
			send_string("DES");//TELA PARA HABILITAR USUARO 1
		}
		send_string(" USUARIO1");
		proxima_linha();
		send_string("2 - ");
		if(!usuario2Status)
		{
			send_string("HAB");//TELA PARA HABILITAR USUARO 2
		}else
		{
			send_string("DES");//TELA PARA HABILITAR USUARO 2
		}
		send_string(" USUARIO2  ");
	}

	void telaResetSenhas()
	{
		limpa_reseta_cursor();
		send_string("* RESET SENHAS");
		proxima_linha();
		send_string("# TROCAR SENHAS");
		
	}

	void telaResetarSenhas(){
		limpa_reseta_cursor();
		send_string("RESETAR SENHAS?");
		proxima_linha();
		send_string("* - SIM  # - NAO");
	}

	void telaSenhasDivergentes(){
		limpa_reseta_cursor();
		send_string("     SENHAS     ");
		proxima_linha();
		send_string("  DIVERGENTES   ");
	}

	void telaNovaSenha(){
		limpa_reseta_cursor();
		send_string("NOVA SENHA:");
		proxima_linha();
	}

	void telaConfirmaSenha(){
		limpa_reseta_cursor();
		send_string("REPITA A SENHA:");
		proxima_linha();
	}
	
	void telaModoNoturnoOn(){
		limpa_reseta_cursor();
		send_string("MODO NOTURNO ON");
		proxima_linha();
		send_string("senha:");
	}
	
	void telaModoNoturnoOff(){
		limpa_reseta_cursor();
		send_string("MODO NOTURNO OFF");
		proxima_linha();
		send_string("senha:");
	}
	
	void telaInicio(){
		limpa_reseta_cursor();
		send_string("---ALARMICROS---");
		proxima_linha();
		send_string("SENHA:");
	}
	
	void telaSensorAtivado(){
		limpa_reseta_cursor();
		send_string("INTRUSO");
		proxima_linha();
		send_string("SENHA:");
	}
	
	void telaAlarmeOff(){
		limpa_reseta_cursor();
		send_string("ALARME OFF");
	}
	
	void telaATivandoOSistema(){
		limpa_reseta_cursor();
		send_string("ATIVANDO SISTEMA");
	}
	
	void telaSenhaIncorreta(){
		limpa_reseta_cursor();
		send_string("SENHA INCORRETA");
		proxima_linha();
		send_string("SENHA:");
	}
	
	void telaSenhaCorreta(){
		limpa_reseta_cursor();
		send_string("SENHA CORRETA");
	}
	
	void telaBemVindoAdm(){
		proxima_linha();
		send_string("BEM-VINDO ADM");
	}
	
	void telaAlarmeAtivo(){
		limpa_reseta_cursor();
		send_string("ALARME ATIVO");
		proxima_linha();
		send_string("SENHA:");
	}