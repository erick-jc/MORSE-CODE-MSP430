#include <msp430.h>
#include "buzzer.h"

void Buzzer(uint16_t freq)
{
    P1DIR |= BIT2;     // P1.2 como saída
    P1SEL |= BIT2;     // Função secundária: TA0.1

    uint16_t periodo = 1000000 / freq;
    uint16_t intensidade = periodo /30;
    
    TA0CTL = TASSEL__SMCLK | MC__UP | TACLR;
    TA0CCR0 = periodo - 1;   
    TA0CCR1 = intensidade;    
    TA0CCTL1 = OUTMOD_7;   
}

void BuzzerOFF()
{
    TA0CTL = TASSEL__SMCLK | MC__UP | TACLR;
    TA0CCR1 = 0;   
}
