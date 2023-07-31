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

void telaSelecionarMotivoAlarme();

void telaOpcaoIntrusoOuInvadido();

void telaMotivoAlarme();

void telaSenhasOuMais();

void telaEstadosOuMais();

void telaHorasOuMais();

void telaModNotOuSair();

void telaTrocaSenhas();

void telaDeStatusUsuario();

void telaResetSenhas();

void telaResetarSenhas();

void telaConfirmaSenha();

void telaSenhaInvalida();

void telaSenhasDivergentes();

void telaNovaSenha();

void printa_horarioreal(int diaAtual, int horaAtual, int minAtual);

void telaConfirmaSenha();

void telaModoNoturnoOn();

void telaModoNoturnoOff();

void telaInicio();

void telaSensorAtivado();

void telaAlarmeOff();

void telaATivandoOSistema();

void telaSenhaIncorreta();

void telaSenhaCorreta();

void telaBemVindoAdm();

void telaAlarmeAtivo();

#endif