#ifndef DETECCAO_INTRUSO_H_
#define DETECCAO_INTRUSO_H_

char alarme_ativo;
char intruso_detectado;
char chamada;
long tempo_TA; //tempo de delay com o led piscando
char sensorAtivo;
int horarioSensor[3];

void delay_piscaled();
void ativa_buzzer();
void desativa_buzzer();
void enviaMotivo();


#endif /* DETECCAO_INTRUSO_H_ */