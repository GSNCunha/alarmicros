#ifndef TECLADO_H_
#define TECLADO_H_

char teclado[4][4];
char tecla;
uint8_t binColuna;
char possivelTecla;

char ligaLinhas(int bitColuna, int indColuna);
char procuraLinhas(uint8_t coluna);
char procuraTecla();

#endif /* TECLADO_H_ */