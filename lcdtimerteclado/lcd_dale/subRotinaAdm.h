/*
 * subRotinaAdm.h
 *
 * Created: 7/22/2023 4:51:57 PM
 *  Author: Gabri
 */ 


#ifndef SUBROTINAADM_H_
#define SUBROTINAADM_H_


char instrucaoDigitada;
char usuario1Status;
char usuario2Status;
char modoNoturnoStatus;
char sair;
char novaSenha[6];
int pediHorario;

void printa_horarioreal(int diaAtual, int horaAtual, int minAtual);
void subRotinaAdm();
void escreveNumero(uint8_t valor);
void modoNoturno();




#endif /* SUBROTINAADM_H_ */