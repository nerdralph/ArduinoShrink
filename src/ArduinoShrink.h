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

// move millis to millis.c - LTO can inline
// millis_raw is slow by 2.4% (1000/1024), so multiply by 1.024
// shift and add instead of multiply: 1 6/256 = 1.0234375
// ifdef RAW_MILLIS - else precise millis
extern "C" uint32_t millis_raw();
inline uint32_t millis()
{
    uint32_t m = millis_raw();
    // return m;
    // high-precision millis correction =~ 32 extra bytes
    // 1 + 1573/2^16 = 1.0240
    // return m + ((m * 1573) >> 16);
    return m + m>>6 + m>>7;
}

