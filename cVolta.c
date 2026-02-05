#include <msp430.h> 
#include <stdint.h>
#include "cvolta.h"
#include "buzzer.h"
#include "lcd.h"

const char * letraParaMorse(char c)
{
    switch (c)
    {
        case 'A': case 'a': return ".-";
        case 'B': case 'b': return "-...";
        case 'C': case 'c': return "-.-.";
        case 'D': case 'd': return "-..";
        case 'E': case 'e': return ".";
        case 'F': case 'f': return "..-.";
        case 'G': case 'g': return "--.";
        case 'H': case 'h': return "....";
        case 'I': case 'i': return "..";
        case 'J': case 'j': return ".---";
        case 'K': case 'k': return "-.-";
        case 'L': case 'l': return ".-..";
        case 'M': case 'm': return "--";
        case 'N': case 'n': return "-.";
        case 'O': case 'o': return "---";
        case 'P': case 'p': return ".--.";
        case 'Q': case 'q': return "--.-";
        case 'R': case 'r': return ".-.";
        case 'S': case 's': return "...";
        case 'T': case 't': return "-";
        case 'U': case 'u': return "..-";
        case 'V': case 'v': return "...-";
        case 'W': case 'w': return ".--";
        case 'X': case 'x': return "-..-";
        case 'Y': case 'y': return "-.--";
        case 'Z': case 'z': return "--..";

        case '0': return "-----";
        case '1': return ".----";
        case '2': return "..---";
        case '3': return "...--";
        case '4': return "....-";
        case '5': return ".....";
        case '6': return "-....";
        case '7': return "--...";
        case '8': return "---..";
        case '9': return "----.";

        default: return ""; // não reconhecido
    }
}

void piscarPonto()
{
    P1OUT |= BIT0;
    Buzzer(5000);
    __delay_cycles(500000); // LED ponto
    P1OUT &= ~BIT0;
    BuzzerOFF();
    __delay_cycles(500000); // pausa entre sinais
}

void piscarTraco()
{
    P1OUT |= BIT0;
    Buzzer(2000);
    __delay_cycles(1500000); // LED traço
    P1OUT &= ~BIT0;
    BuzzerOFF();
    __delay_cycles(500000);  // pausa entre sinais
}

void piscarMorse(const char *code)
{
    int i;
    for (i = 0; code[i] != '\0'; i++)
    {
        if (code[i] == '.')
        {
            piscarPonto();
        }
        else if (code[i] == '-')
        {
            piscarTraco();
        }
    }
}
