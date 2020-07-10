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

void init()
{
    asm("sei");

    // setup ADC with /32 prescaler
    ADCSRA = (1 << ADPS2) | (1 << ADPS0) | (1 << ADEN);
}

