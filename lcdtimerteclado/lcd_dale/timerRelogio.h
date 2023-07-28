/*
 * timerRelogio.h
 *
 * Created: 28/07/2023 00:32:01
 *  Author: vitor
 */
#ifndef TIMERRELOGIO_H_
#define TIMERRELOGIO_H_

uint8_t dia;
uint8_t hora;
uint8_t min;
int nr_ciclos_relogio;

void configRelogio();
void conversaoMinHr();

#endif /* TIMERRELOGIO_H_ */