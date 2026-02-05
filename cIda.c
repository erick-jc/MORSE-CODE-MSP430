#include <msp430.h> 
#include <stdint.h>
#include <string.h>
#include "cida.h"
#include "buzzer.h"
#include "lcd.h"

volatile uint8_t ultimo_estado = 1;    // botao inicia solto
volatile unsigned int tpressionado = 0;
volatile unsigned int tsolto = 0;

volatile char morse_buffer[6];  // buffer de 5 letras + '\0'
volatile uint8_t nletras = 0;
volatile uint8_t letra_pronta = 0;  // flag para letra pronta

char morseToChar(const char *code)
{
    if (strcmp(code, ".-") == 0) return 'A';
    else if (strcmp(code, "-...") == 0) return 'B';
    else if (strcmp(code, "-.-.") == 0) return 'C';
    else if (strcmp(code, "-..") == 0) return 'D';
    else if (strcmp(code, ".") == 0) return 'E';
    else if (strcmp(code, "..-.") == 0) return 'F';
    else if (strcmp(code, "--.") == 0) return 'G';
    else if (strcmp(code, "....") == 0) return 'H';
    else if (strcmp(code, "..") == 0) return 'I';
    else if (strcmp(code, ".---") == 0) return 'J';
    else if (strcmp(code, "-.-") == 0) return 'K';
    else if (strcmp(code, ".-..") == 0) return 'L';
    else if (strcmp(code, "--") == 0) return 'M';
    else if (strcmp(code, "-.") == 0) return 'N';
    else if (strcmp(code, "---") == 0) return 'O';
    else if (strcmp(code, ".--.") == 0) return 'P';
    else if (strcmp(code, "--.-") == 0) return 'Q';
    else if (strcmp(code, ".-.") == 0) return 'R';
    else if (strcmp(code, "...") == 0) return 'S';
    else if (strcmp(code, "-") == 0) return 'T';
    else if (strcmp(code, "..-") == 0) return 'U';
    else if (strcmp(code, "...-") == 0) return 'V';
    else if (strcmp(code, ".--") == 0) return 'W';
    else if (strcmp(code, "-..-") == 0) return 'X';
    else if (strcmp(code, "-.--") == 0) return 'Y';
    else if (strcmp(code, "--..") == 0) return 'Z';
    else if (strcmp(code, "-----") == 0) return '0';
    else if (strcmp(code, ".----") == 0) return '1';
    else if (strcmp(code, "..---") == 0) return '2';
    else if (strcmp(code, "...--") == 0) return '3';
    else if (strcmp(code, "....-") == 0) return '4';
    else if (strcmp(code, ".....") == 0) return '5';
    else if (strcmp(code, "-....") == 0) return '6';
    else if (strcmp(code, "--...") == 0) return '7';
    else if (strcmp(code, "---..") == 0) return '8';
    else if (strcmp(code, "----.") == 0) return '9';
    return '?';
}

void morseAdd(char simb)
{
    if (nletras < 5)
    {
        morse_buffer[nletras++] = simb;
        morse_buffer[nletras] = '\0';
    }
    else
    {
        letra_pronta = 1;
    }
}

void processaLetra()
{
    if (nletras == 0) return;
    char letra = morseToChar((char*)morse_buffer);
    lcdPrint(letra);
    nletras = 0;
    morse_buffer[0] = '\0';
}

void initBotaoTimer()
{
    P1DIR &= ~BIT3;
    P1REN |= BIT3;
    P1OUT |= BIT3;

    TA2CTL = TASSEL__SMCLK | MC__UP | TACLR;
    TA2CCR0 = 10000 - 1;
    TA2CCTL0 = CCIE;
}

#pragma vector = TIMER2_A0_VECTOR
__interrupt void TimerBotao()
{
    static uint8_t ultimo_estado = 1;
    static uint8_t espaco = 1;
    static uint8_t sinal = 0;            // 0 = . e 1 = -
    uint8_t estado;

    if (P1IN & BIT3)
    {
        estado = 1;  // BT solto
    }
    else
    {
        estado = 0;  // BT apertado
    }


    if (estado == 0 && ultimo_estado == 1)  //Buzzer para ponto
    {
        tpressionado = 0;
        tsolto = 0;
        espaco = 0;

        sinal = 0;
        Buzzer(5000);
    }
    else if (estado == 0 && ultimo_estado == 0)
    {
        tpressionado += 10;
        tsolto = 0;
        
        if (tpressionado >= 700 && sinal == 0)   //Buzzer para traço
        {
            sinal = 1;        
            Buzzer(2000);        
        }
    }
    else if (estado == 1 && ultimo_estado == 0)
    {
        if (tpressionado > 30 && tpressionado < 700)  //Periodo para ponto
        {
            morseAdd('.');
        }
        else if (tpressionado >= 700)    //Periodo para traço
        {
            morseAdd('-');
        }
        BuzzerOFF();
        tpressionado = 0;
        tsolto = 0;
    }
    else if (estado == 1 && ultimo_estado == 1)
    {
        tsolto += 10;

        if (tsolto >= 7000 && espaco == 0)  //Tempo para  espaço
        {
            lcdPrint(' ');
            espaco = 1;
        }

        if (tsolto >= 1000 && nletras > 0)
        {
            letra_pronta = 1;
            tsolto = 0;
        }
    }

    ultimo_estado = estado;
}
