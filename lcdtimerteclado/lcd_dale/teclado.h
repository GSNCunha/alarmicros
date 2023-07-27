/*
 * teclado.h
 *
 * Created: 20/07/2023 20:36:04
 *  Author: vitor
 */ 


#ifndef TECLADO_H_
#define TECLADO_H_

char teclado[4][4];
char tecla;
uint8_t binColuna;
char possivelTecla;

char ligaLinhas(int bitColuna, int indColuna);
char procuraLinhas(uint8_t coluna);
char procuraTecla();
char procuraTecla1x();

#endif /* TECLADO_H_ */