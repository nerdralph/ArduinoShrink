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
; 8 bits are used instead of 7 for better precision at non 2^n F_CPU
.equ FRACT_MAX, (1000 >> 2) 
GLABEL TIMER0_OVF_vect
    push r16
    in r16, SREG
    push r16
    push ZL
    ldi ZL, lo8(t0_fract)
    push ZH
    ldi ZH, hi8(t0_fract)
    ld r16, Z
    subi r16, lo8(-(FRACT_INC))         ; add millis fraction
    cpi r16, FRACT_MAX                  ; C set when fract < FRACT_MAX
    st Z+, r16
_inc_millis:
    ld r16, Z
    sbci r16, lo8(-(MILLIS_INC +1))     ; r16 += MILLIS_INC + NOT Carry
    st Z+, r16
    brcc _inc_millis
    pop ZH
    pop ZL
    pop r16
    out SREG, r16
    pop r16
reti

