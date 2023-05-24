GLOBAL createProcessStack

start:
    call rdx
    mov rax, 0x42
    int 80h

createProcessStack:
    push rbp
    mov rbp, rsp
    
    mov rsp, rdx ; rsp 
    push 0x0     ; SS
    push rdx     ; RSP
    push 0x202   ; RFLAGS
    push 0x8     ; CS
    push start
    
    push 0x01    ; RBX
    push 0x02    ; RCX
    push rcx     ; RDX
    push rdx     ; RBP
	push rdi     ; RDI
	push rsi     ; RSI
	push 0x08    ; R8   
	push 0x09    ; r9
	push 0x0A    ; r10
	push 0x0B    ; r11
	push 0x0C    ; r12
	push 0x0D    ; r13
	push 0x0E    ; r14
	push 0x0F    ; r15    
	push 0x10    ; rax

    mov rax, rsp

    mov rsp, rbp
    pop rbp
    ret 