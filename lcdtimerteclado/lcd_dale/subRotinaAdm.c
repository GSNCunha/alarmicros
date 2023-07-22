#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "teclado.h"
#include "lcdio.h"
#include "validacao_senhas.h"
#include "timers.h"

char instrucaoDigitada = '\0';
char usuario1Status = 1;
char usuario2Status = 1;
char modoNoturnoStatus = 1;
char sair = 1;
char novaSenha[6]= {"11111"};
	
	void tela1()
	{
		limpa_reseta_cursor();
		send_string("* TROCAR SENHAS");
		proxima_linha();
		send_string("# MAIS OPCOES");
	}
	void tela2()
	{
		limpa_reseta_cursor();
		send_string("* ESTADO USUARIO");
		proxima_linha();
		send_string("# MAIS OPCOES");
		
	}
	void tela3()
	{
		limpa_reseta_cursor();
		send_string("* PEDIR HORAS");
		proxima_linha();
		send_string("# MAIS OPCOES");
	}

	void tela4()
	{
		limpa_reseta_cursor();
		send_string("* ");
		if(!modoNoturnoStatus)
		{
			send_string("HAB");
		}else
		{
			send_string("DES");
		}
		send_string(" MODO NOT");
		proxima_linha();
		send_string("# SAIR");
	}

	void telaTrocaSenhas()
	{
		limpa_reseta_cursor();
		send_string("1/2 - USUARIO1/2");
		proxima_linha();
		send_string("* - ADM");
	}

	void telaDeStatusUsuario()
	{
		limpa_reseta_cursor();
		send_string("1 - ");
		if(!usuario1Status)
		{
			send_string("HAB");//TELA PARA HABILITAR USUARO 1
		}else
		{
			send_string("DES");//TELA PARA HABILITAR USUARO 1
		}
		send_string(" USUARIO1");
		proxima_linha();
		send_string("2 - ");
		if(!usuario2Status)
		{
			send_string("HAB");//TELA PARA HABILITAR USUARO 2
		}else
		{
			send_string("DES");//TELA PARA HABILITAR USUARO 2
		}
		send_string(" USUARIO2  ");
	}

void subRotinaAdm()
{
	
	delay_1s();//espera para exibir a mensagem de senha correta
	delay_1s();
	
	tela1();// troca senhas ou mais opcoes
	sair = 0;
	
	while(!sair)
	{
		instrucaoDigitada = procuraTecla();
		
		if(instrucaoDigitada == '*')//opção trocar senha de usuários
		{
			telaTrocaSenhas();
			instrucaoDigitada = '\0';
			while(1)
			{
				instrucaoDigitada = procuraTecla();
				if(instrucaoDigitada == '1' || instrucaoDigitada == '2' || instrucaoDigitada == '*')
				{
					limpa_reseta_cursor();
					send_string("NOVA SENHA:");
					//novaSenha = SUBROTINA TROCA SENHA
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
					tela1();
					instrucaoDigitada = '\0';
					break;//sai desse while entre no while(!sair)
				}
							
			}
		}else if(instrucaoDigitada == '#')//mais opcoes
		{
			tela2();//status usuario e mais opcoes
			instrucaoDigitada = '\0';
			while(1)
			{
				instrucaoDigitada = procuraTecla();
				if( instrucaoDigitada == '*')//opcao mudar status usuários
				{
					telaDeStatusUsuario();
					instrucaoDigitada = '\0';
					while(1)
					{
						instrucaoDigitada = procuraTecla();
						if(instrucaoDigitada == '1')
						{
							usuario1Status = !usuario1Status;
							break; //sair desse while
						}
						if(instrucaoDigitada == '2')
						{
							usuario2Status = !usuario2Status;
							break; //sair desse while
						}
					}
					
					tela1();
					instrucaoDigitada = '\0';
					break;//sai desse while entre no while(!sair)
					
				}else if(instrucaoDigitada == '#')//mais opcoes			
				{
					tela3();//pedir horário ou mais opcoes
					instrucaoDigitada = '\0';
					while(1)
					{
						instrucaoDigitada = procuraTecla();
						if(instrucaoDigitada == '*')
						{
							//subrotina pedir horas
							break;//sair desse while
						}else if(instrucaoDigitada == '#')//mais opcoes
						{ 
							tela4();//habilitar ou desabilitar modo noturno ou mais opcoes
							instrucaoDigitada = '\0';
							while(1)
							{
								instrucaoDigitada = procuraTecla();
								if(instrucaoDigitada == '*')
								{
									modoNoturnoStatus = !modoNoturnoStatus;
									break;
								}
								if(instrucaoDigitada == '#')
								{
									sair = 1;
									break;
								}
							}break;
						}
					}
					tela1();
					instrucaoDigitada = '\0';
					break;//sai desse while entre no while(!sair)
				}			
			}	
		}	
	}
}