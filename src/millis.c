// (c) Ralph Doncaster 2020
// ArduinoShrink

#include <Arduino.h>

struct {
    volatile uint32_t t0_ovfl;
    uint8_t ovfl_pad;
} t0_millis;

// portability macros for mega8
#ifndef TCCR0B
#define TCCR0B TCCR0
#define TIMSK0 TIMSK
#endif

__attribute(( section(".init8"), used, naked))
void init_millis()
{
    TCCR0B = _BV(CS01) | _BV(CS00);     // div64 prescaler
    TIMSK0 = _BV(TOIE0);                // enable overflow interrupt
}

