#include <msp430.h>
#include "lcd.h"

uint8_t coluna = 0;
uint8_t linha = 0;

void i2cInit()
{
    P3SEL |= BIT0 | BIT1;  // P3.0 = SDA, P3.1 = SCL
    P3REN |= BIT0 | BIT1;
    P3DIR &= ~(BIT0 | BIT1);
    P3OUT |= BIT0 | BIT1;

    UCB0CTL1 |= UCSWRST;
    UCB0CTL0 = UCMST | UCMODE_3 | UCSYNC;
    UCB0CTL1 = UCSSEL__SMCLK;
    UCB0BRW = 100;
    UCB0CTL1 &= ~UCSWRST;
}

uint8_t i2cWriteByte(uint8_t addr, uint8_t byte)
{
    UCB0I2CSA = addr;
    UCB0CTL1 |= UCTR | UCTXSTT;

    while (!(UCB0IFG & UCTXIFG));
    UCB0TXBUF = byte;
    while (UCB0CTL1 & UCTXSTT);

    if (UCB0IFG & UCNACKIFG)
    {
        UCB0CTL1 |= UCTXSTP;
        while (UCB0CTL1 & UCTXSTP);
        return 1;
    }

    while (!(UCB0IFG & UCTXIFG));
    UCB0CTL1 |= UCTXSTP;
    while (UCB0CTL1 & UCTXSTP);
    return 0;
}

void lcdWriteNibble(uint8_t nibble, uint8_t isChar)
{
    nibble <<= 4;
    i2cWriteByte(LCD_ADDR, nibble | BT | isChar);
    i2cWriteByte(LCD_ADDR, nibble | BT | EN | isChar);
    i2cWriteByte(LCD_ADDR, nibble | BT | isChar);
}

void lcdWriteByte(uint8_t byte, uint8_t isChar)
{
    lcdWriteNibble(byte >> 4, isChar);
    lcdWriteNibble(byte & 0x0F, isChar);
    __delay_cycles(3000);
}

void lcdInit()
{
    __delay_cycles(100000);
    lcdWriteNibble(0x03, INSTR); __delay_cycles(20000);
    lcdWriteNibble(0x03, INSTR); __delay_cycles(20000);
    lcdWriteNibble(0x03, INSTR); __delay_cycles(20000);
    lcdWriteNibble(0x02, INSTR); __delay_cycles(20000);

    lcdWriteByte(0x28, INSTR);
    lcdWriteByte(0x06, INSTR);
    lcdWriteByte(0x0F, INSTR);
    lcdWriteByte(0x01, INSTR);
}

void lcdSetCursor(uint8_t col, uint8_t lin)
{
    uint8_t address;

    if (lin == 0)
    {
        address = 0x00;
    }
    else
    {
        address = 0x40;
    }

    lcdWriteByte(0x80 | (address + col), INSTR);
}

void lcdPrint(char c)
{
    lcdSetCursor(coluna, linha);
    lcdWriteByte(c, CHAR);

    coluna++;
    if (coluna >= 16)
    {
        coluna = 0;
        linha++;
        if (linha >= 2)
        {
            linha = 0;
            coluna = 0;
            __delay_cycles(2500000);
            lcdWriteByte(0x01, INSTR);
            __delay_cycles(3000);
        }
    }
}

void lcdPrintString(const char *str)
{
    while (*str)
    {
        lcdPrint(*str++);
    }
}
