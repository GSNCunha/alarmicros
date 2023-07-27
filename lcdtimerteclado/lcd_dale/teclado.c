/*
 * GccApplication2.c
 *
 * Created: 7/18/2023 11:21:35 PM
 * Author : Gabri
 */ 
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

char possivelTecla1;
char possivelTecla2;


char ligaLinhas(int bitColuna, int indColuna)//coluna = numero correspondente da coluna
{
	
	PORTK = 0b01111111;
	//delay_1ms();

	if ((PINK & (1 << bitColuna)) == 0){
	return teclado[0][indColuna];}

	PORTK = 0b10111111;
	//delay_1ms();

	if ((PINK & (1 << bitColuna)) == 0){
		
	return teclado[1][indColuna];}

	PORTK = 0b11011111;
	//delay_1ms();

	if ((PINK & (1 << bitColuna)) == 0){
	return teclado[2][indColuna];}

	PORTK = 0b11101111;
	//delay_1ms();
	
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

char procuraTecla1x()
{
	PORTK = 0b00001111;//pull up do input ativado
	//delay_1ms();
	
	while(PINK == 0b00001111);//0000 1011
	
	binColuna = PINK;
	/*possivelSenha = procuraLinhas(binColuna);
	delay_1ms();
	if(possivelSenha == procuraLinhas(binColuna)){
		return possivelSenha;
	}*/
	return procuraLinhas(binColuna);
}


char procuraTecla(){
	possivelTecla1 = procuraTecla1x();
	delay_200ms();
	possivelTecla2 = procuraTecla1x();
	if (possivelTecla1 == possivelTecla2){
		return possivelTecla1;		
	}else{
		return '\0';
	}
	};