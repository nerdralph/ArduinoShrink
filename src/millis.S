; (c) Ralph Doncaster 2020
; ArduinoShrink

; all used io registers are < 0x3F
;#define __SFR_OFFSET 0
#include <avr/io.h>

;struct {
;    uint32_t t0_ovfl;
;    uint8_t ovfl_pad;
;} t0_millis;

; micros not supported
.global micros
micros:

.global millis
millis:
    ldi ZL, lo8(t0_millis)
    ldi ZH, hi8(t0_millis)
    ld r22, Z
    ldd r23, Z+1
    ldd r24, Z+2
    ldd r25, Z+3
    ld r0, Z
    cp r0, r22                          ; has millis changed?
    brne millis
ret

.global TIMER0_OVF_vect
TIMER0_OVF_vect:
    push r16
    in r16, SREG-0x20
    push r16
    push ZL
    ldi ZL, lo8(t0_millis)
    push ZH
    ldi ZH, hi8(t0_millis)
millis_inc:
    ld r16, Z
    subi r16, -1
    st Z+, r16
    brcc millis_inc
    pop ZH
    pop ZL
    pop r16
    out SREG-0x20, r16
    pop r16
reti

