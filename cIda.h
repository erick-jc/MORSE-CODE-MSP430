#ifndef CIDA_H
#define CIDA_H

#include <msp430.h>
#include <stdint.h>
#include <string.h>
#include "buzzer.h"
#include "lcd.h"

//Variaveis para controle do Tempo do botão, Armazenamento do Morse, e Formação de Letra
extern volatile uint8_t ultimo_estado;
extern volatile unsigned int tpressionado;
extern volatile unsigned int tsolto;

extern volatile char morse_buffer[6];
extern volatile uint8_t nletras;
extern volatile uint8_t letra_pronta;

//Compara duas strings (codigo digitado com a tabela morse) e retorna a letra 
char morseToChar(const char *code);

//Adiciona "." e "-" no buffer morse e determina o fim da string
void morseAdd(char simb);

//Traduz simbolos para algarismos
void processaLetra();

//Botão que será utilizado
void initBotaoTimer();

#endif 
