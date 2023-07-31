#include <avr/io.h>
#include <avr/interrupt.h>
#include "timers.h"
#include "teclado.h"
char teclado [4][4] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};
char tecla;

uint8_t binColuna;

char possivelTecla;


char ligaLinhas(int bitColuna, int indColuna)//coluna = numero correspondente da coluna
{
	
	PORTK = 0b01111111;//ligamos cada linha individualmete para testar se é a linha pressionada 

	if ((PINK & (1 << bitColuna)) == 0){
	return teclado[0][indColuna];}

	PORTK = 0b10111111;

	if ((PINK & (1 << bitColuna)) == 0){
		
	return teclado[1][indColuna];}

	PORTK = 0b11011111;

	if ((PINK & (1 << bitColuna)) == 0){
	return teclado[2][indColuna];}

	PORTK = 0b11101111;
	
	if ((PINK & (1 << bitColuna)) == 0){
	return teclado[3][indColuna];}
	
	return '\0';
}

char procuraLinhas(uint8_t coluna)
{
	if(coluna == 0b00000111)//coluna 1
	{
		return ligaLinhas(3, 0);
	}
	if(coluna == 0b00001011)//coluna 2
	{
		return ligaLinhas(2, 1);
	}
	if(coluna == 0b00001101)//coluna 3
	{
		return ligaLinhas(1, 2);
	}
	if(coluna == 0b00001110)//coluna 4
	{
		return ligaLinhas(0, 3);
	}
	return '\0';
}

char procuraTecla()
{
	PORTK = 0b00001111;//pull up do input ativado
	
	while(PINK == 0b00001111);//espera qualquer tecla ser pressionada
	
	binColuna = PINK;//guarda estado que ja nos revela a coluna do botão pressionado
	possivelTecla = procuraLinhas(binColuna);
	delay_200ms();
	PORTK = 0b00001111; //reseta a porta pra proxima leitura
	binColuna = PINK;
	if(possivelTecla == procuraLinhas(binColuna)){
		return possivelTecla;
	}else{
		return '\0';
	}
}
