; (c) Ralph Doncaster 2020
; ArduinoShrink

#include <avr/io.h>

;struct {
;    uint32_t t0_ovfl;
;    uint8_t ovfl_pad;
;} t0_millis;

.global TIMER0_OVF_vect
TIMER0_OVF_vect:
    push r16
    in r16, SREG-0x20
    push r16
    push ZL
    ldi ZL, lo8(t0_millis)
    push ZH
    ldi ZH, hi8(t0_millis)
t0_ovfl_inc:
    ld r16, Z
    subi r16, -1
    st Z+, r16
    brcc t0_ovfl_inc
    pop ZH
    pop ZL
    pop r16
    out SREG-0x20, r16
    pop r16
reti

; shift left r22-r25 (ulong), arg in r30
.global lsl4_r22_r30
lsl4_r22_r30:
    lsl r22
    rol r23
    rol r24
    rol r25
    dec r30
    brne lsl4_r22_r30
ret
