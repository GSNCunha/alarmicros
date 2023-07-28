/*
 * serial.h
 *
 * Created: 27/07/2023 22:53:20
 *  Author: vitor
 */ 


#ifndef SERIAL_H_
#define SERIAL_H_


#define BAUD_RATE ;
#define F_CPU ;
#define SERIAL_TIMEOUT ;

void serialAtivarInterrupt();
void serialDesativarInterrupt();
void serialInicializar();
uint8_t serialByteDisponivelEnvio();
uint8_t serialByteRecebido();
void serialEnviarByte(uint8_t ch);
uint8_t serialReceberByte(void);
void serialEnviarString(const char *str);
int pediHorario;


#endif /* SERIAL_H_ */