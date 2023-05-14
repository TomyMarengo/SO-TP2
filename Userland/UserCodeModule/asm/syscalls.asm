GLOBAL sys_read
GLOBAL sys_write
GLOBAL sys_time
GLOBAL sys_millis
GLOBAL sys_clearscreen
GLOBAL sys_writeat
GLOBAL sys_screensize
GLOBAL sys_pollread
GLOBAL sys_drawpoint
GLOBAL sys_drawrect
GLOBAL sys_drawline
GLOBAL sys_date
GLOBAL sys_inforeg
section .text

sys_read:
    mov rax, 0x00
    int 80h
    ret

sys_write:
    mov rax, 0x01
    int 80h
    ret

sys_time:
    mov rax, 0x02
    int 80h
    ret

sys_millis:
    mov rax, 0x03
    int 80h
    ret

sys_clearscreen:
    mov rax, 0x04
    int 80h
    ret

sys_writeat:
    mov rax, 0x05
    mov r10, rcx
    int 80h
    ret

sys_screensize:
    mov rax, 0x06
    int 80h
    ret

sys_pollread:
    mov rax, 0x07
    mov r10, rcx ; fourth integer param in C is in RCX, but we need to pass it through R10.
    int 80h
    ret

sys_drawpoint:
    mov rax, 0x08
    int 80h
    ret

sys_drawrect:
    mov rax, 0x09
    mov r10, rcx
    int 80h
    ret

sys_drawline:
    mov rax, 0x0A
    mov r10, rcx
    int 80h
    ret

sys_date:
    mov rax, 0x0B
    mov r10, rcx
    int 80h
    ret

sys_inforeg:
    mov rax, 0x0C
    mov r10, rcx
    int 80h
    ret