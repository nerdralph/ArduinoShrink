// (c) Ralph Doncaster 2020
// ArduinoShrink

#if F_CPU != 16000000
#error Only 16Mhz F_CPU supported
#endif

#include <util/delay.h>

inline void delayMicroseconds(unsigned int us)
{
    _delay_us(us);
}

