#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcdio.h"
#include "timers.h"
int main(void)
{
	DDRB = 0xFF;
	PORTB = 0xFF;
	config_porta_avr();
	config_lcd_padrao();
	limpa_reseta_cursor();
	delay_1s();
	send_data(0x30);
    while (1) 
    {
	}
}

