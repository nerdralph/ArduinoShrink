; (c) Ralph Doncaster 2020
; ArduinoShrink

; #include <avr/io.h>

;struct {
;    uint32_t t0_ovfl;
;    uint8_t ovfl_pad;
;} t0_millis;

; to force t0_isr to get linked in
.global lsl4_r22_r20

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
    ; millis = t0_ovfl * 1.024 , so multiply by fractional addition
    movw r18, r22
    movw r20, r24
    rcall shift_add                     ; adds t0_ovfl / 128
    rcall shift_add                     ; adds t0_ovfl / 64
    ; rcall 1f
    ; todo: add t0_ovfl / 2048
1:  add r18, r24                        ; adds t0_ovfl / 16384
    adc r19, r25
    adc r20, r1
    movw r22, r18
    movw r24, r20
ret

shift_add:
    ldi r30, 1
    rcall lsl4_r22_r30
    add r18, r23
    adc r19, r24
    adc r20, r25
    adc r21, r1
ret
