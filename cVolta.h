#ifndef CVOLTA_H
#define CVOLTA_H

#include <msp430.h>
#include "buzzer.h"
#include "lcd.h"


// Letras retornam para Morse
const char *letraParaMorse(char c);

//LED piscando em Morse
void piscarPonto();
void piscarTraco();
void piscarMorse(const char *code);

#endif

