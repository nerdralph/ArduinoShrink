// (c) Ralph Doncaster 2020
// ArduinoShrink

#include <Arduino.h>
#include <util/delay.h>

/*
__attribute ((noinline))
void delay(unsigned long ms)
{
    do {
        _delay_us(999);                 // leave 1us for loop overhead
    } while (--ms);
}
*/

void delay(unsigned long ms)
{
    // lower 16-bits of micros is enough considering wraparound
    uint16_t start = micros();

    while (ms > 0) {
        yield();
        while (ms > 0 && ((uint16_t)micros() - start) >= 1000) {
            ms --;
            start += 1000;
        }
    }
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
