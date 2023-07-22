/*
 * deteccao_intruso.h
 *
 * Created: 22/07/2023 16:47:49
 *  Author: vitor
 */ 


#ifndef DETECCAO_INTRUSO_H_
#define DETECCAO_INTRUSO_H_

char alarme_ativo;
char intruso_detectado;
char chamada;
long tempo_TA; //tempo de delay com o led piscando

void delay_piscaled();
void ativa_buzzer();


#endif /* DETECCAO_INTRUSO_H_ */