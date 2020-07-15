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

extern "C" uint32_t micros_raw();
inline uint32_t micros()
{
    /*
    register uint32_t micros_ asm("r22");
    asm ("call micros_raw" : "=r" (micros_) );
    return micros_;
    */
    // asm volatile ("call micros_raw");
    return micros_raw() * 4;
}

extern "C" uint32_t millis_raw();
inline uint32_t millis()
{
    uint32_t m = millis_raw();
    return m + ((m * 1573) >> 16);
}
