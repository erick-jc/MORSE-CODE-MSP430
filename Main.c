#include <msp430.h>
#include "cida.h"
#include "cvolta.h"
#include "lcd.h"
#include "buzzer.h"

volatile int modo = 0;  // 0 = Ida, 1 = Volta
volatile int troca = 0;     //troca de modo

void TrocaBT()
{
    P2DIR &= ~BIT1;
    P2REN |= BIT1;
    P2OUT |= BIT1;

    TA1CTL = TASSEL__SMCLK | MC__UP | TACLR;
    TA1CCR0 = 10000 - 1;
    TA1CCTL0 = CCIE;
}

void Trocar()           //Função para trocar de modo
{
    if (troca)
    {
        modo ^= 1; // troca o modo entre 0 e 1
        lcdWriteByte(0x01, INSTR);
        if (modo == 0)
        {
            coluna = 0;
            linha = 0;
            lcdPrintString("Modo:Morse-Letra");
        }
        else
        {
            coluna = 0;
            linha = 0;
            lcdPrintString("Modo:Letra-Morse");
        }
        __delay_cycles(2000000);
        lcdWriteByte(0x01, INSTR);
        coluna = 0;
        linha = 0;

        troca = 0;
    }
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void TrocaModo()
{
    static uint8_t estado_anterior = 1;
    uint8_t estado = (P2IN & BIT1) ? 1 : 0;

    if (estado == 0 && estado_anterior == 1)
    {
        troca = 1;  // Solicita troca de modo
    }

    estado_anterior = estado;
}

int main()
{
    WDTCTL = WDTPW | WDTHOLD;

    P1DIR |= BIT0;     // LED
    P1OUT &= ~BIT0;  

    i2cInit();
    lcdInit();
    initBotaoTimer();
    TrocaBT();
    __enable_interrupt();

    coluna = 0;
    linha = 0;

    lcdPrintString("Letra para Morse");
    __delay_cycles(2000000);
    lcdWriteByte(0x01, INSTR);
    coluna = 0;
    linha = 0;


    while (1)
    {
        Trocar();
        if (modo == 0)
        {
            // Modo: Morse para texto
            if (letra_pronta)
            {
                processaLetra();
                letra_pronta = 0;
            }
        }
        else
        {
            // Modo: Texto para Morse
            const char *msg = "Edil gostoso passa nois ai pfvr nunca te pedi nada";
            int i = 0;
            while (msg[i] != '\0')
            {
                Trocar();
                if (modo != 1)
                {
                    break;  // Sai se modo for trocado
                }
                char c = msg[i];
                lcdPrint(c);
                const char *morse = letraParaMorse(c);
                piscarMorse(morse);
                __delay_cycles(1200000);        ///Tempo entre letras
                i++;
            }
            while (modo == 1)
            {
                Trocar();  // Verifica se o botão foi pressionado e muda o modo
            }
        }
    }
}
