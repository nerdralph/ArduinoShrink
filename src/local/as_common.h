// ArduinoShrink refactored code here

#pragma once

// avr-libc math.h does not declare gcc log2 builtin
double log2(double);

// portability macros for mega8
#ifndef TCCR0B
#define TCCR0B TCCR0
#define TIMSK0 TIMSK
#endif

