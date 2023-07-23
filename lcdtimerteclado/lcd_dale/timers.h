#ifndef TIMERS_H_

#define TIMERS_H_

int nr_ciclos_timer1;
char led2hz;
char buzzer;
long ciclos_buzzer;
char onda;

void config_timer1();
void onda_800hzint();
void onda_400hzint();
void onda_2hzint();
void onda_800hz();
void onda_400hz();
void delay_250ms();
void delay_1s();
void delay_1ms();
void led2hz_int();


#endif