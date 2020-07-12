// (c) Ralph Doncaster 2020
// ArduinoShrink

#include <Arduino.h>

struct {
    uint8_t fract_count;
    volatile uint32_t millis;
    uint8_t ovfl_pad;
} t0_millis;

ISR(TIMER0_OVF_vect, ISR_NAKED)
{
    asm (
    "push r16                   \t\n"
    "in r16, __SREG__           \t\n"
    "push r16                   \t\n"
    "push ZL                    \t\n"
    "ldi ZL, lo8(t0_millis)     \t\n"
    "ldd r22, Z+0               \t\n"
    "reti                       \t\n"
    );
}

__attribute((naked))
uint32_t millis()
{
    asm (
    "ldd r22, Z+0               \t\n"
    "ldd r23, Z+1               \t\n"
    "ldd r24, Z+2               \t\n"
    "ldd r25, Z+3               \t\n"
    "ldd r0, Z+0                \t\n"
    "cp r0, r22                 \t\n"   // has millis changed?
    "brne millis                \t\n"
    "ret                        \t\n"
    :: "z"(&(t0_millis.millis)) 
    );
}

// portability macros for mega8
#ifndef TCCR0B
#define TCCR0B TCCR0
#define TIMSK0 TIMSK
#endif
__attribute(( section(".init8"), used))
void init_millis()
{
    TCCR0B = _BV(CS01);                 // div8 prescaler
    TIMSK0 = _BV(TOIE0);                // enable overflow interrupt
}

