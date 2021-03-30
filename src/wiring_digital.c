// (c) Ralph Doncaster 2020
// thanks to Bill W for inspiration
// github.com/WestfW/Duino-hacks/blob/master/fastdigitalIO/fastdigitalIO.h 
// 20200709 v0.1.0 release

#define ARDUINO_MAIN
#include <Arduino.h>

typedef volatile uint8_t* ioreg_p;

inline ioreg_p pin_to_port(uint8_t pin)
{
    return (ioreg_p) port_to_output_PGM[digital_pin_to_port_PGM[pin]];
}

inline ioreg_p pin_to_ddr(uint8_t pin)
{
    // DDR address is 1 less than PORT address
    return pin_to_port(pin) - 1;
}

inline ioreg_p pin_to_pinreg(uint8_t pin)
{
    // PIN address is 2 less than PORT address
    return pin_to_port(pin) - 2;
}

inline uint8_t pin_to_bit(uint8_t pin)
{
    return digital_pin_to_bit_mask_PGM[pin];
}

void pinMode(uint8_t pin, uint8_t mode)
{
    if (mode == OUTPUT)
        *pin_to_ddr(pin) |= (pin_to_bit(pin));
    else {
        *pin_to_ddr(pin) &= ~(pin_to_bit(pin));
        if (mode == INPUT_PULLUP) *pin_to_port(pin) |= (pin_to_bit(pin));
    }
}

void digitalWrite(uint8_t pin, uint8_t val)
{
    if (val == LOW)
        *pin_to_port(pin) &= ~(pin_to_bit(pin));
    else                                // HIGH
        *pin_to_port(pin) |= (pin_to_bit(pin));
}

int digitalRead(uint8_t pin)
{
    return (*pin_to_pinreg(pin) & pin_to_bit(pin)) ? HIGH : LOW;
}
