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

void ativarSerialInterrupt();
void desativarSerialInterrupt();
void inicializarSerial();
uint8_t byteDisponivelEnvio();
uint8_t byteRecebidoSerial();
void enviarByteSerial(uint8_t ch);
uint8_t receberByteSerial(void);
void enviarStringSerial(const char *str);
int pediHorario;


#endif /* SERIAL_H_ */