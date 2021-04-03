; (c) Ralph Doncaster 2020
; ArduinoShrink

; all used io registers are < 0x3F
#define __SFR_OFFSET 0
#include <avr/io.h>
#include "macros.inc"

#define start   r0
#define remain  r20
#define current r21
; todo: implement fractional T0CNT_PER_MS for precision with non 2^n F_CPU
; delay for r26:r24 (u24) ms
GLABEL delay_impl
    in  start, TCNT0
    ldi remain, lo8(T0CNT_PER_MS)
_wait1ms:                                ; check TCNT0 until 1ms has passed
    in  current, TCNT0
    sub current, start
    add start, current                  ; advance start
    sub remain, current
    brcc _wait1ms
    ; sub remain_fract, T0MS_FRACT
    subi remain, lo8(-(T0CNT_PER_MS))   ; add 1ms
    ; decrement ms argument
    sbiw r24, 1
    sbci r26, 0
    brne _wait1ms
    ret

