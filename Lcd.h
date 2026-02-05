#ifndef LCD_H
#define LCD_H

#include <stdint.h>

// Endereço do LCD
#define LCD_ADDR 0x27

// Bits de controle do LCD
#define BT BIT3
#define EN BIT2
#define RW BIT1
#define RS BIT0

#define INSTR 0
#define CHAR  1

// Variáveis para controle de linha e coluna
extern uint8_t coluna;
extern uint8_t linha;

// Inicia o lCD
void i2cInit(void);

// Envia Byte para o endereço
uint8_t i2cWriteByte(uint8_t addr, uint8_t byte);

// Escrita no LCD
void lcdWriteNibble(uint8_t nibble, uint8_t isChar);
void lcdWriteByte(uint8_t byte, uint8_t isChar);
void lcdInit(void);
void lcdSetCursor(uint8_t coluna, uint8_t lin);
void lcdPrint(char c);
void lcdPrintString(const char *str);

#endif
