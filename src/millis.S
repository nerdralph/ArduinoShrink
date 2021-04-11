; (c) Ralph Doncaster 2020
; ArduinoShrink

#include "macros.inc"

; to force t0_isr to get linked in
.global t0_ovfl_isr

; atomically read t0_millis, return in r25:r22 as per avr-gcc ABI
GLABEL millis_impl
    ldi ZL, lo8(t0_millis)
    ldi ZH, hi8(t0_millis)
;    X_ldiw 30, t0_millis                ; Z = t0_millis
  _reload:
    ld r22, Z
    ldd r23, Z+1
    ldd r24, Z+2
    ldd r25, Z+3
    ld r0, Z
    cp r0, r22                          ; has millis changed?
    brne _reload
ret

