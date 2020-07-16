// (c) Ralph Doncaster 2020
// ArduinoShrink

#include <Arduino.h>

struct {
    volatile uint32_t t0_ovfl;
    uint8_t ovfl_pad;
} t0_millis;

extern uint32_t micros_raw();
uint32_t micros()
{
    register uint32_t u asm("r22");
    asm ("%~call %x1" : "=r" (u) : "i"(micros_raw) : "r30", "r31" );
    return u * 4;
}

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

