; (c) Ralph Doncaster 2020
; ArduinoShrink

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

