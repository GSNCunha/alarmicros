#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcdio.h"
#include "timers.h"
int main(void)
{
	config_porta_avr();
	config_lcd_padrao();
	limpa_reseta_cursor();
	delay_1s();
	send_data(0x30);
	send_command(0xC0);
	send_data(0x41);
	send_data(0x21);
    while (1) 
    {
		
	}
}

