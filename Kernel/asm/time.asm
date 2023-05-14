GLOBAL rtc_getCurrentHours
GLOBAL rtc_getCurrentMinutes
GLOBAL rtc_getCurrentSeconds
GLOBAL rtc_getCurrentDay
GLOBAL rtc_getCurrentMonth
GLOBAL rtc_getCurrentYear

section .text
rtc_getCurrentHours:
    cli
    mov al, 0x0B
    out 70h, al
    in al, 71h
    or al, 4
    out 71h, al

    mov al, 0x04
    out 70h, al
    in al, 71h
    sti
    ret

rtc_getCurrentMinutes:
    cli
    mov al, 0x0B
    out 70h, al
    in al, 71h
    or al, 4
    out 71h, al

    mov al, 0x02
    out 70h, al
    in al, 71h
    sti

    ret

rtc_getCurrentSeconds:
    cli
    mov al, 0x0B
    out 70h, al
    in al, 71h
    or al, 4
    out 71h, al

    mov al, 0x00
    out 70h, al
    in al, 71h
    sti

    ret

rtc_getCurrentDay:
    cli
    mov al, 0x0B
    out 70h, al
    in al, 71h
    or al, 4
    out 71h, al

    mov al, 0x07
    out 70h, al
    in al, 71h
    sti

    ret

rtc_getCurrentMonth:
    cli
    mov al, 0x0B
    out 70h, al
    in al, 71h
    or al, 4
    out 71h, al

    mov al, 0x08
    out 70h, al
    in al, 71h
    sti

    ret

rtc_getCurrentYear:
    cli
    mov al, 0x0B
    out 70h, al
    in al, 71h
    or al, 4
    out 71h, al

    mov al, 0x09
    out 70h, al
    in al, 71h
    sti

    ret
