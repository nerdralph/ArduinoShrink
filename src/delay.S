; (c) Ralph Doncaster 2020
; ArduinoShrink

; all used io registers are < 0x3F
#define __SFR_OFFSET 0
#include <avr/io.h>
#include "macros.inc"

;struct {
;    uint32_t t0_ovfl;
;    uint8_t ovfl_pad;
;} t0_millis;


; to force t0_isr to get linked in
.global t0_ovfl_isr

; read lower 16 bits of extended TCNT0 into r19:r18
r18_t0cnt:
    in r18, TCNT0
    lds r19, t0_millis
    in r0, TCNT0
    sub r0, r18                         ; TCNT0 overflow?
    brcs r18_t0cnt
ret

; delay for r27:r24 (ulong) ms
GLABEL delay_impl
    rcall r18_t0cnt
    movw r20, r18                       ; r20 = start count
    rjmp decms
dloop:
    rcall r18_t0cnt
    sub r18, r20                        ; current - start
    sbc r19, r21
    subi r18, lo8(T0CNT_PER_MS)         ; > 1ms?
    sbci r19, hi8(T0CNT_PER_MS)
    brcc dloop
    subi r20, lo8(-(T0CNT_PER_MS))        ; add 1ms to start
    sbci r21, hi8(-(T0CNT_PER_MS))
decms:                                  ; decrement ms argument
    sbiw r24, 1
    sbci r26, 0
    sbci r27, 0
    brcc dloop
    ret

