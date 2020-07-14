; (c) Ralph Doncaster 2020
; ArduinoShrink

; #include <avr/io.h>

;struct {
;    uint32_t t0_ovfl;
;    uint8_t ovfl_pad;
;} t0_millis;

; ensure timer ISR is brought in by linker
.global t0_ovfl_inc

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
