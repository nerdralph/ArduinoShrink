; (c) Ralph Doncaster 2020
; ArduinoShrink

; all used io registers are < 0x3F
#define __SFR_OFFSET 0
#include <avr/io.h>

;struct {
;    uint32_t t0_ovfl;
;    uint8_t ovfl_pad;
;} t0_millis;

; ensure timer ISR is brought in by linker
.global t0_ovfl_inc

.global micros
micros:
    ldi ZL, lo8(t0_millis)
    ldi ZH, hi8(t0_millis)
1:
    in r22, TCNT0
    ld r23, Z
    ldd r24, Z+1
    ldd r25, Z+2
    in r0, TCNT0
    sub r0, r22                         ; TCNT0 overflow?
    brne 1b
ret
