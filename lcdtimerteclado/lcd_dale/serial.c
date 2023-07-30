/*
 * Comunica  o serial usando o USART0.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <ctype.h>
#include "lcdio.h"
#include "timers.h"
#include "teclado.h"
#include "validacao_senhas.h"
#include "deteccao_intruso.h"
#include "subRotinaAdm.h"



#define BAUD_RATE 19200UL
#define F_CPU 16000000

// Tempo, em ms, que uma comunica  o Serial pode bloquear
// a execu  o do programa sem gerar um timeout.
#define SERIAL_TIMEOUT 3000

int pediHorario = 0;


void serialAtivarInterrupt() {//usamos
	// Ativa o interrupt de recebimento completo.
	UCSR0B |= (1 << RXCIE0);
}

void serialDesativarInterrupt() {//usamos
	// Desativa o interrupt de recebimento completo.
	UCSR0B &= ~(1 << RXCIE0);
}

void serialInicializar()//usamos
{	
	// Configura o Baud Rate.
	UBRR0 = (F_CPU)/(16 * BAUD_RATE) - 1;
	
	// Ativa o recebidor e transmissor.
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
	
	// Altera formato do frame para 8 bits de dados (padr o: 5).
	// Outras configura  es s o inalteradas:
	// USART ass ncrono normal, um bit de in cio, um de parada,
	// sem bit de paridade.
	UCSR0C = (0b011 << UCSZ00);
	
	// Ativa o interrupt de recebimento completo.
	serialAtivarInterrupt();
}

uint8_t serialByteDisponivelEnvio() {//usamos
	// Checa se o buffer de transmiss o estar livre para envio.
	//send_data((UCSR0A & (1 << UDRE0))+0x30);
	return UCSR0A & (1 << UDRE0);
}

uint8_t serialByteRecebido() {//usamos
	// Checa se dados foram recebidos.
	return UCSR0A & (1 << RXC0);
}

void serialEnviarByte(uint8_t ch) {//usamos
	// Carrega o caractere a ser mandado quando o envio estive disponivel.
	while (!serialByteDisponivelEnvio());
	UDR0 = ch;
}

uint8_t serialReceberByte(void) { //usamos
	// Espera dados serem recebidos e os retorna.
	while (!serialByteRecebido());
	return UDR0;
}

void serialEnviarString(const char *str) {//usamos
	 // vamos usar esta
	// Envia uma string terminada por \0 e retorna a
	// quantidade de bytes enviados.
	uint8_t i = 0;
	while (str[i] != '\0') {
		serialEnviarByte(str[i]);
		//send_data(str[i]);
		i++;
	}
//	return i;
}

/*
 * Fun  es para ler e redefinir respostas do m dulo externo
 *  s mensagens da urna.
 */

/*RESPOSTA_CORRESPONDENCIA serialVerificarCorrespondencia() {
	// Verifica se j  houve uma mensagem de correspond ncia e se sim, qual.
	// Realiza verifica  o de timeout de comunica  o.
	static uint8_t  resetFeito = 0;
	if (!resetFeito) {
		serialVerificarTimeout(1);
		resetFeito = 1;
	}
	else {
		serialVerificarTimeout(0);
	}
	
	// Verifica se a mensagem foi recebida.
	RESPOSTA_CORRESPONDENCIA resposta = serialRespostas.correspondencia;
	if (resposta != CORRESPONDENCIA_AGUARDANDO) {
		serialRespostas.correspondencia = CORRESPONDENCIA_AGUARDANDO;
		resetFeito = 0;
	}
	return resposta;
}*/

/*uint8_t serialVerificarMensagem(volatile uint8_t *mensagem) {
	// Verifica se uma mensagem qualquer foi recebida.
	// Realiza verifica  o de timeout de comunica  o.
	static uint8_t  resetFeito = 0;
	if (!resetFeito) {
		serialVerificarTimeout(1);
		resetFeito = 1;
	}
	else {
		serialVerificarTimeout(0);
	}
	
	// Verifica se a mensagem foi recebida.
	if (*mensagem) {
		*mensagem = 0;
		resetFeito = 0;
		return 1;
	}
	return 0;
}*/

/*uint8_t serialVerificarNovoEleitor() {
	return serialVerificarMensagem(&(serialRespostas.novoEleitor));
}

uint8_t serialVerificarInicioVotacao() {
	return serialVerificarMensagem(&(serialRespostas.inicioVotacao));
}

uint8_t serialVerificarConclusaoVotacao() {
	return serialVerificarMensagem(&(serialRespostas.conclusaoVotacao));
}

uint8_t serialVerificarTimeoutVotacao() {
	return serialVerificarMensagem(&(serialRespostas.timeoutVotacao));
}

uint8_t serialVerificarVotoPresidente() {
	return serialVerificarMensagem(&(serialRespostas.votoPresidente));
}

uint8_t serialVerificarVotoGovernador() {
	return serialVerificarMensagem(&(serialRespostas.votoGovernador));
}

uint8_t serialVerificarVotoSenador() {
	return serialVerificarMensagem(&(serialRespostas.votoSenador));
}

uint8_t serialVerificarVotoDeputadoFederal() {
	return serialVerificarMensagem(&(serialRespostas.votoDeputadoFederal));
}*/

/*uint8_t serialVerificarRelatorioVotacao() {
	// N o faz verifica  o de timeout para relat rio de vota  o
	// pois este   comumente demorado e varia com o comprimento do
	// relat rio.
	if (serialRespostas.relatorioVotacao) {
		serialRespostas.relatorioVotacao = 0;
		return 1;
	}
	return 0;
}*/