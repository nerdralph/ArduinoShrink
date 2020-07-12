// (c) Ralph Doncaster 2020
// ArduinoShrink

#include <util/delay.h>

inline void delayMicroseconds(unsigned int us)
{
    _delay_us(us);
}
