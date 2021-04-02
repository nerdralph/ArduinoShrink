// (c) Ralph Doncaster 2020
// ArduinoShrink

//#include <Arduino.h>
#include <avr/io.h>
#include "as_common.h"

#define CPU_MHZ (F_CPU / 1000000)

extern uint32_t micros_raw();
uint32_t micros()
{
    register uint32_t u asm("r22");
    asm ("%~call %x1" : "=r" (u) : "i"(micros_raw) : "r30", "r31" );
    // split into lower 8 & upper 24-bits
    uint8_t t0 = u & 0xFF;
    __uint24 u3 = u >> 8;

    //return (u3 * 1000) + (t0 * (uint8_t)(T0_PRESCALE / CPU_MHZ));
    // shift + add saves 24B vs * 1000, asm would be a bit smaller
    return ( ((u3 << 7) - ((u3 << 1) + u3)) << 3 ) + 
        (t0 * (uint8_t)(T0_PRESCALE / CPU_MHZ));
}

extern uint32_t millis_impl();
uint32_t millis()
{
    register uint32_t m asm("r22");
    asm ("%~call %x1" : "=r" (m) : "i"(millis_impl) : "r30", "r31" );
    return millis_impl();
}

#define MICROS_PER_T0_OVFL ((T0_PRESCALE * 256) / CPU_MHZ)
#define MILLIS_INC (MICROS_PER_T0_OVFL / 1000)
#define FRACT_INC ((MICROS_PER_T0_OVFL % 1000) >> 2)

__attribute(( section(".init8"), used, naked))
void init_millis()
{
    // for millis ISR
    asm ( ".global MILLIS_INC\n");
    asm ( ".equ MILLIS_INC, %0\n" :: "M" (MILLIS_INC) );
    asm ( ".global FRACT_INC\n");
    asm ( ".equ FRACT_INC, %0\n" :: "M" (FRACT_INC) );

    TIMSK0 = _BV(TOIE0);                // enable T0 overflow interrupt
}

