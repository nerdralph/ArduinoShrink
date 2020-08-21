// (c) Ralph Doncaster 2020
// ArduinoShrink

#include <Arduino.h>
#include <util/delay.h>

// lightweight custom function call abi
extern void delay_impl();
void delay(uint32_t msec)
{
    register uint32_t ms asm ("r24") = msec;
    asm ( ".equ T0CNT_PER_MS, %0\n" 
        ".global T0CNT_PER_MS\n"
        // T0 prescaler is 64
        :: "i" (F_CPU / 1000 / 64) );
    asm volatile (
        "rcall %x1\n" 
        : "+r"(ms)
        : "i"(delay_impl)
        : "r18", "r19", "r20", "r21"
    );
}

// avr-libc math.h does not declare gcc log2 builtin
double log2(double);

void init()
{
    asm("sei");

    // setup ADC with 250-500kHz clock
    uint8_t prescaler = log2(F_CPU/500000);
    ADCSRA = prescaler | (1 << ADEN);
}

