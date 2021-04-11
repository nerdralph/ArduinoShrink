// (c) Ralph Doncaster 2020
// thanks to Bill W for inspiration
// github.com/WestfW/Duino-hacks/blob/master/fastdigitalIO/fastdigitalIO.h 
// 20200709 v0.1.0 release
// 20200410 alias _to_xxx_PGM[] arrays as __flash for runtime pin manipulation

#include <stdint.h>
#include <avr/io.h>

// todo: use constants.h from PicoCore
enum {LOW, HIGH};                       // for digitalWrite()
enum {INPUT, OUTPUT, INPUT_PULLUP};     // for pinMode()

extern __flash const uint16_t port_to_output_PGM[];
extern __flash const uint8_t digital_pin_to_port_PGM[];
extern __flash const uint8_t digital_pin_to_bit_mask_PGM[];

typedef volatile uint8_t* ioreg_p;

#define ALWAYS_INLINE __attribute__((always_inline)) inline

ALWAYS_INLINE ioreg_p pin_to_port(uint8_t pin)
{
    // use pointer instead of array operators due to avr-gcc bug:
    // https://www.avrfreaks.net/forum/avr-gcc-have-i-told-you-lately-i-hate-you
    //return (ioreg_p) port_to_output_PGM[digital_pin_to_port_PGM[pin]];
    return (ioreg_p) *(port_to_output_PGM + *(digital_pin_to_port_PGM +pin));
}

ALWAYS_INLINE ioreg_p pin_to_ddr(uint8_t pin)
{
    // DDR address is 1 less than PORT address
    return pin_to_port(pin) - 1;
}

ALWAYS_INLINE ioreg_p pin_to_pinreg(uint8_t pin)
{
    // PIN address is 2 less than PORT address
    return pin_to_port(pin) - 2;
}

ALWAYS_INLINE uint8_t pin_to_bit(uint8_t pin)
{
    //return digital_pin_to_bit_mask_PGM[pin];
    return *(digital_pin_to_bit_mask_PGM + pin);
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
