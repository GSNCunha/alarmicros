#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "teclado.h"
#include "lcdio.h"
#include "validacao_senhas.h"
#include "timers.h"
#include "serial.h"
#include "timerRelogio.h"

char instrucaoDigitada = '\0';
char usuario1Status = 1;
char usuario2Status = 1;
char modoNoturnoStatus = 0;
char senhaConferida = 0;
char sair = 1;
char novaSenha[6]= {"11111"};
char confereNovaSenha[6]= {"11111"};
int pediHorario = 0;
	

void escreveNumero(uint8_t valor){//converte numeros inteiros em seus respectivos c�digos compat�veis com o LCD para printa-los na tela
	if(valor>9)
	{
		int segundoNum;
		int primeiroNum = valor/10;
		send_data(primeiroNum+0b110000);
		segundoNum = valor-primeiroNum*10;
		send_data(segundoNum+0b110000);
	}else{
		send_data(0b110000);
		send_data(valor+0b110000);
	}
}	


void subRotinaAdm()
{
	
	telaSenhasOuMais();// tela troca senhas ou mais opcoes
	sair = 0;
	PCMSK2 = 0x01;//desliga interrup��o dos pinos 
	
	while(!sair)
	{
		instrucaoDigitada = procuraTecla();
		
		if(instrucaoDigitada == '*')//op��o trocar ou resetar senha de usu�rios
		{
			telaResetSenhas();
			instrucaoDigitada = '\0';
			while(1)
			{
				instrucaoDigitada = procuraTecla();
				if(instrucaoDigitada == '*')// resetar as senhas para o padrao de fabrica
				{
					telaResetarSenhas();
					instrucaoDigitada = '\0';
					while(1)
					{
						instrucaoDigitada = procuraTecla();
						if (instrucaoDigitada == '*')
						{
							strcpy(senhas.usuario1, senhas.usuario1Default);
							strcpy(senhas.usuario2, senhas.usuario2Default);
							strcpy(senhas.adm, senhas.admDefault);
							break;
						}
						if (instrucaoDigitada == '#')
						{
							break;
						}
					}
					telaSenhasOuMais();//ap�s a��o, printa novamente a primeira tela para voltar para as op��es de a��es do ADM
					instrucaoDigitada = '\0';
					break;//sai desse while entre no while(!sair)
					
				}else if(instrucaoDigitada == '#')//op��o troca das senhas
				{
					telaTrocaSenhas();
					instrucaoDigitada = '\0';
					while(1)
					{
						instrucaoDigitada = procuraTecla();
						if(instrucaoDigitada == '1' || instrucaoDigitada == '2' || instrucaoDigitada == '*')
						{
							senhaConferida = 0;
							while(!senhaConferida)
							{
								
								telaNovaSenha();
								
								strcpy(novaSenha,subRotinaTrocaSenha());
								
								if(strcmp(novaSenha,"INVAL"))// se senha == INVAL, a senha digitada ja � usada no sistema, ent�o fazemos o usu�rio repetir  a��o
								{
									telaConfirmaSenha();
									
									strcpy(confereNovaSenha,subRotinaTrocaSenha());
									
									if(!strcmp(novaSenha,confereNovaSenha))// se a primeira senha � valida, pedimos para o usu�rio a confirme, se n�o ele tera que repetir tudo novamente.
									{
										senhaConferida = 1;
										}else{
										
										telaSenhasDivergentes();
										delay_1s();
									}
								}else{
									telaSenhaInvalida();
									delay_1s();
								}
								
							}
							
							
							if(instrucaoDigitada == '1')//usuario 1
							{
								strcpy(senhas.usuario1, novaSenha);
							}
							if(instrucaoDigitada == '2')//usuario 2
							{
								strcpy(senhas.usuario2, novaSenha);
							}
							if(instrucaoDigitada == '*')//usuario adm
							{
								strcpy(senhas.adm, novaSenha);
							}
							for (int i = 0; i < 5; i++) {
								senha[i] = '\0';
							}
							nr_digitados = 0;
							break;
						}
					}
					telaSenhasOuMais();//ap�s a��o, printa novamente a primeira tela para voltar para as op��es de a��es do ADM
					instrucaoDigitada = '\0';
					break;//sai desse while entre no while(!sair)
				}
				
			}
		}else if(instrucaoDigitada == '#')//mais opcoes
		{
			telaEstadosOuMais();//status usuario e mais opcoes
			instrucaoDigitada = '\0';
			while(1)
			{
				instrucaoDigitada = procuraTecla();
				if( instrucaoDigitada == '*')//opcao mudar status usu�rios
				{
					telaDeStatusUsuario();
					instrucaoDigitada = '\0';
					while(1)
					{
						instrucaoDigitada = procuraTecla();
						if(instrucaoDigitada == '1')//habilitar/desabilitar usu�rio 1
						{
							usuario1Status = !usuario1Status;
							break; //sair desse while
						}
						if(instrucaoDigitada == '2')//habilitar/desabilitar usu�rio 2
						{
							usuario2Status = !usuario2Status;
							break; //sair desse while
						}
					}
					
					telaSenhasOuMais();//ap�s a��o, printa novamente a primeira tela para voltar para as op��es de a��es do ADM
					instrucaoDigitada = '\0';
					break;//sai desse while entre no while(!sair)
					
				}else if(instrucaoDigitada == '#')//mais opcoes
				{
					telaHorasOuMais();//pedir hor�rio ou mais opcoes
					instrucaoDigitada = '\0';
					while(1)
					{
						instrucaoDigitada = procuraTecla();
						if(instrucaoDigitada == '*')//op��o pedir hor�rio
						{
							pediHorario = 1;
							enviarStringSerial("AH");
							delay_250ms();
							printa_horarioreal(dia, hora, min);//mostra o horario por um curto tempo
							delay_1s();
							delay_1s();
							break;//sair desse while
						}else if(instrucaoDigitada == '#')//mais opcoes
						{
							telaModNotOuSair();//habilitar ou desabilitar modo noturno ou mais opcoes
							instrucaoDigitada = '\0';
							while(1)
							{
								instrucaoDigitada = procuraTecla();
								if(instrucaoDigitada == '*')// muda estado do modo noturno
								{
									modoNoturnoStatus = !modoNoturnoStatus;
									break;
								}
								if(instrucaoDigitada == '#')
								{
									sair = 1;//sai da subrotina para o programa principal
									limpa_reseta_cursor();
									break;
								}
							}break;
						}
					}
					telaSenhasOuMais();//ap�s a��o, printa novamente a primeira tela para voltar para as op��es de a��es do ADM
					instrucaoDigitada = '\0';
					break;//sai desse while entre no while(!sair)
				}
			}
		}
	}
	nr_digitados = 0;
	PCMSK2 = 0x0F; //volta a ter interrup��o nos pinos
}
