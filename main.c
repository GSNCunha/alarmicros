/*
 * leitura de teclas.c
 *
 * Created: 7/16/2023 4:08:48 PM
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

char teclado [4][4] = {
	"1", "2", "3", "A",
	"4", "5", "6", "B",
	"7", "8", "9", "C",
	"*", "0", "#", "D"
};
char tecla;

uint8_t binColuna;

char possivelTecla;

char ligaLinhas(char coluna)//coluna = numero correspondente da coluna
{
	PORTK = 0x7;//01111111

	if ((PORTK & (1 << 0)) == 0)
		return teclado[0][coluna];

	PORTK = 0xBF;//10111111

	if ((PORTK & (1 << 1)) == 0)
		return teclado[1][coluna];

	PORTK = 0xD7;//11011111

	if ((PORTK & (1 << 2)) == 0)
		return teclado[2][coluna];

	PORTK = 0xEF;//11101111
	
	if ((PORTK & (1 << 3)) == 0)
		return teclado[3][coluna];
}


char procuraLinhas(char coluna)
{	
	if(coluna == 0b00000111)//coluna 1
	{
		return ligaLinhas(0);
	}
	if(coluna == 0b00001011)//coluna 2
	{
		return ligaLinhas(1);
	}
	if(coluna == 0b00001101)//coluna 3
	{
		return ligaLinhas(2);
	}
	if(coluna == 0b00001110)//coluna 4
	{
		return ligaLinhas(3);
	}
}

char procuraTecla()
{
			
	//atraso1ms();
			
	PORTK = 0b00001111;//pull up do input ativado
			
	//atraso1ms();
	
	while(PINK = 0b00001111);//0000 1011
	
	binColuna = PINK;
	
	possivelTecla = procuraLinhas(binColuna);
	
	//atraso2ms();
	
	if(possivelTecla == procuraLinhas(binColuna))
		return  possivelTecla;
}


ISR(PCINT2_vect){ //pinos K0 até K7
		PCIFR = (1<<2); //reseta flag da interrupção 
		tecla = procuraTecla();
}

int main(void)
{
	sei(); //set enable interrupts -- seta 1 no bit I do status register
	PCICR = 0x4; //ativa interrupção dos pinos PCINT16 ao PCINT23 = K0 ao K7
	PCMSK2 = 0x1; //interrupção só no pino K0
	DDRK = 0b11110000; //  (1) PORTK(0-3) output / (0) PORTK(4-7) input 
	
    while (1) 
    {
		tecla = procuraTecla();
    }
}