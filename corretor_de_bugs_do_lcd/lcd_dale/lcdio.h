#ifndef LCDIO_H_

#define LCDIO_H_

void config_delay_timer0_CTC(char tempo);

void delay_CTC();

void config_porta_avr();

void trigger_EN();

void send_data(char _data_);

void send_command(char command);

void send_string(char *string);

void limpa_reseta_cursor();

void config_lcd_padrao();

void proxima_linha();

#endif