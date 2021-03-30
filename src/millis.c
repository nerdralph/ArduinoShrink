// (c) Ralph Doncaster 2020
// ArduinoShrink

//#include <Arduino.h>
#include <avr/io.h>
#include "as_common.h"

extern uint32_t micros_raw();
uint32_t micros()
{
    register uint32_t u asm("r22");
    //asm ("%~call %x1" : "=r" (u) : "i"(micros_raw) : "r30", "r31" );
    //return u * 4;
    asm (
        "%~call %x1                 \n"
        "ldi r30, lo8(T0_SCALE + 2) \n"
        "rcall lsl4_r22_r30         \n"
        : "=r" (u) : "i"(micros_raw) : "r30", "r31"
    );
    return u;
}

extern uint32_t millis_impl();
uint32_t millis()
{
    return millis_impl();
}

#define CPU_MHZ (F_CPU / 1000000)
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

    // scaling factor for t0 vs 16Mhz
    uint8_t t0_scale = log2(16000000/F_CPU);
    asm ( ".equ T0_SCALE, %0\n" :: "M" (t0_scale) );
    TIMSK0 = _BV(TOIE0);                // enable T0 overflow interrupt
}

