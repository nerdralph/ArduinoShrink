// (c) Ralph Doncaster 2020
// ArduinoShrink

#include <Arduino.h>
#include <util/delay.h>

__attribute ((noinline))
void delay(unsigned long ms)
{
    do {
        _delay_us(999);                 // leave 1us for loop overhead
    } while (--ms);
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
