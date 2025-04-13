// (c) Ralph Doncaster 2020
// ArduinoShrink

//#include <Arduino.h>
#include <avr/io.h>
#include "as_common.h"

#define CPU_MHZ (F_CPU / 1000000)

// micros_raw return data
typedef union {
    uint32_t i32;
    struct {
        uint8_t t0cnt; uint8_t t0fract; uint16_t t0millis;
    };
} micros_raw_data;

extern uint32_t micros_raw();
uint32_t micros()
{
    // see micros.S for micros_raw implementation
    register micros_raw_data u asm("r22");
    asm ("%~call %x1" : "=r" (u) : "i"(micros_raw) : "r30", "r31" );
    uint8_t t0 = u.t0cnt;
    // millis fraction/250, so mf * 4 = micoseconds
    uint8_t mf = 256 - u.t0fract;
    uint16_t m16 = u.t0millis;

    //return (m16 * 1000) + (t0 * (uint8_t)(T0_PRESCALE / CPU_MHZ));
    // shift + add is less code vs * 1000, asm would be a bit smaller
    // mf << 2 is the same as mf * 4
    return ( ((m16 << 7) - ((m16 << 1) + m16)) << 3 ) + 
        (t0 * (uint8_t)(T0_PRESCALE / CPU_MHZ)) +
        (mf << 2);
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

