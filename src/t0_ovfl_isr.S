; (c) Ralph Doncaster 2020
; ArduinoShrink

#define __SFR_OFFSET 0
#include <avr/io.h>
#include "macros.inc"

; millis_pad keeps t0_millis overflow from corrupting memory
;struct {
;    uint8_t t0_fract;
;    uint32_t t0_millis;
;    uint8_t millis_pad;
;} t0_millis;
.lcomm t0_fract, 1
.global t0_millis
.lcomm t0_millis, 5                     ; t0_millis + pad

; ensure BSS gets cleared
.global __do_clear_bss

; to help with linking
GLABEL t0_ovfl_isr

; this is an optimized asm version of the Arduino core timer overflow
; github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/wiring.c
; uses 8 bits of t0_fract instead of 7 for more precision at non 2^n F_CPU
.equ FRACT_MAX, (1000 >> 2) 
GLABEL TIMER0_OVF_vect
    sei                                 ; allow other interrupts
    push r16
    in r16, SREG
    push r16
    push ZL
    ldi ZL, lo8(t0_fract)
    push ZH
    ldi ZH, hi8(t0_fract)
    ld r16, Z
    subi r16, lo8(FRACT_INC)            ; subtract millis fraction
    brcc _no_underflow
    subi r16, lo8(-FRACT_MAX)           ; add FRACT_MAX, clears Carry
    rjmp 1f
_no_underflow:
    sec                                 ; invert Carry
1:
    st Z+, r16
    ; add one more to millis LSB when fractional counter underflows
    ld r16, Z
    sbci r16, lo8(-(MILLIS_INC + 1))     ; r16 += MILLIS_INC + NOT Carry
    rjmp _save
_add1:                                  ; handle most significant bytes
    ld r16, Z
    subi r16, -1                        ; add 1
_save:
    st Z+, r16
    brcc _add1
    pop ZH
    pop ZL
    pop r16
    out SREG, r16
    pop r16
reti

