// (c) Ralph Doncaster 2020
// ArduinoShrink

extern "C" void delay_impl();
// lightweight custom function call abi
__attribute((always_inline))
inline void delay(uint32_t msec)
{
    register __uint24 ms asm ("r24") = msec;
    asm volatile (
        "rcall %x1\n" 
        : "+r"(ms)
        : "i"(delay_impl)
        : "r19", "r20", "r21"           // clobbers
    );
}

