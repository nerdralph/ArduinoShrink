; (c) Ralph Doncaster 2020
; ArduinoShrink

; all used io registers are < 0x3F
#define __SFR_OFFSET 0
#include <avr/io.h>

; to force t0_isr to get linked in
.global t0_ovfl_isr

.global micros_raw
micros_raw:
    ldi ZL, lo8(t0_millis)
    ldi ZH, hi8(t0_millis)
  _reload:
    in r22, TCNT0
    ld r23, Z
    ldd r24, Z+1
    ldd r25, Z+2
    in r0, TCNT0
    sub r0, r22                         ; TCNT0 overflow?
    brcs _reload
ret

