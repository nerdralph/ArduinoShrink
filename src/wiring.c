// (c) Ralph Doncaster 2020
// ArduinoShrink

#define ARDUINO_MAIN
#include <Arduino.h>
#include <util/delay.h>
#include "as_common.h"

extern void delay_impl();

// lightweight asm function call interface
void delay(uint32_t msec)
{
    register __uint24 ms asm ("r24") = msec;
    asm volatile (
        "%~call %x1\n" 
        : "+r"(ms)
        : "i"(delay_impl)
        : "r20", "r21"           // clobbers
    );
}

// delays a specified number of microseconds - modified from picoCore
// works for clock frequencies of 4Mhz and up
void delayMicroseconds(uint16_t us)
{
    // if us is a compile-time constant result is accurate to 1 cycle
    if (__builtin_constant_p(us)) {
        _delay_us(us);
        return;
    }

    // code below runs when us is not known at compile time
    // uses inline asm to guarantee exact cycle counts
    const float fMHz = (F_CPU/1000000.0);
    delay1us:
        // delay 1us per loop, less 4 cycles for overhead
        _delay_us(1.0 - (4.0 / fMHz));
        asm volatile ("sbiw %[us], 1" : [us]"+d"(us));
    asm goto( "brne %l[delay1us]" :::: delay1us);
}

void init(void)
{
    // T0CNT_PER_MS needed for delay_impl
    asm ( ".equ T0CNT_PER_MS, %0\n" 
        ".global T0CNT_PER_MS\n"
        :: "i" (F_CPU / 1000 / T0_PRESCALE) );

    // setup ADC with 250-500kHz clock
    uint8_t prescaler = log2(F_CPU/500000);
    ADCSRA = prescaler | (1 << ADEN);

    TCCR0B = _BV(CS01) | _BV(CS00);     // enable T0, div64 prescale

    asm("sei");                         // enable interrupts
}

// replace main() from Arduino core
int main(void)
{
    init();
    setup();
    while(1) loop();
}

