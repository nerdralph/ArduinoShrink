; (c) Ralph Doncaster 2020
; ArduinoShrink

; all used io registers are < 0x3F
#define __SFR_OFFSET 0
#include <avr/io.h>
#include "macros.inc"

#define remain  r19
#define start   r20
#define current r21
; delay for r26:r24 (u24) ms
GLABEL delay_impl
    in  start, TCNT0
    ldi remain, lo8(T0CNT_PER_MS)
wait1ms:                                ; check TCNT0 until 1ms has passed
    in  current, TCNT0
    sub current, start
    add start, current                  ; advance start
    sub remain, current
    brcc wait1ms
    subi remain, lo8(-(T0CNT_PER_MS))   ; add 1ms
decms:                                  ; decrement ms argument
    sbiw r24, 1
    sbci r26, 0
    brne wait1ms
    ret

