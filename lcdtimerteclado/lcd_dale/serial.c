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


void ativarSerialInterrupt()
{
	UCSR0B |= (1 << RXCIE0);// Ativa o interrupt
}

void desativarSerialInterrupt()
{
	UCSR0B &= ~(1 << RXCIE0);// Desativa o interrupt
}

void inicializarSerial()
{	
	UBRR0 = (F_CPU)/(16 * BAUD_RATE) - 1;//Baud Rate.
	
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);// Ativa recebimento e transmissão.
	
	UCSR0C = (0b011 << UCSZ00);//frame de 8 bits.
	
	ativarSerialInterrupt();
}

uint8_t byteDisponivelEnvio() {
	
	return UCSR0A & (1 << UDRE0);
}

uint8_t byteRecebidoSerial() {
	
	return UCSR0A & (1 << RXC0);
}

void enviarByteSerial(uint8_t byte) {
	
	while (!byteDisponivelEnvio());//espera estar disponível para enviar
	UDR0 = byte;
}

uint8_t receberByteSerial(void) {
	while (!byteRecebidoSerial());//espera bytes estarem disponíveis para ler
	return UDR0;
}

void enviarStringSerial(const char *str) {//envia serie de bytes
	uint8_t i = 0;
	while (str[i] != '\0') {
		enviarByteSerial(str[i]);
		i++;
	}
}