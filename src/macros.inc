; Ralph Doncaster public domain AVR assembler macros

.macro GLABEL name
     .global \name
     \name:
.endm

; in/out require an io register at address < 0x40
.macro LOAD rd, ioreg
    .if \ioreg > 0x3F
    lds \rd, \ioreg
    .else
    in \rd, \ioreg
    .endif
.endm

.macro STORE ioreg, rr
    .if \ioreg > 0x3F
    sts \ioreg, \rr
    .else
    out \ioreg, \rr
    .endif
.endm

; example: X_ldiw 24, 12345
.macro X_ldiw rd, K
    ldi \rd, lo8(\K)
    ldi (\rd)+1, hi8(\K)
.endm

