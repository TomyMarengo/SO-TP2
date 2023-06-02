GLOBAL cli
GLOBAL sti
GLOBAL hlt
GLOBAL haltcpu

GLOBAL picMasterMask
GLOBAL picSlaveMask

GLOBAL irq00Handler
GLOBAL irq01Handler
GLOBAL irq02Handler	
GLOBAL irq03Handler	
GLOBAL irq04Handler
GLOBAL irq05Handler
GLOBAL exception0Handler
GLOBAL exception6Handler
GLOBAL exception0DHandler
GLOBAL exception0EHandler

GLOBAL awakeScheduler
GLOBAL int81

GLOBAL syscallHandler

EXTERN irqDispatcher
EXTERN syscallDispatcher
EXTERN exceptionDispatcher
EXTERN switchProcess

SECTION .text

%macro pushState 0
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	push rax
%endmacro

%macro popState 0
	pop rax
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 
	call irqDispatcher

	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro

%macro exceptionHandler 1
	mov [regdata], rax
	mov [regdata+8], rbx
	mov [regdata+16], rcx
	mov [regdata+24], rdx
	mov [regdata+32], rsi
	mov [regdata+40], rdi
	mov [regdata+48], rbp
	mov [regdata+64], r8
	mov [regdata+72], r9
	mov [regdata+80], r10
	mov [regdata+88], r11
	mov [regdata+96], r12
	mov [regdata+104], r13
	mov [regdata+112], r14
	mov [regdata+120], r15

	mov rax, [rsp+24]
	mov [regdata+56], rax
	mov rax, [rsp]
	mov [regdata+128], rax
	mov rax, [rsp+16]
	mov [regdata+136], rax

	mov rdi, %1
	mov rsi, regdata
	call exceptionDispatcher
	jmp haltcpu
%endmacro

haltcpu:
	cli
	hlt
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h, al
    pop rbp
    retn

picSlaveMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	0A1h, al
    pop rbp
    retn

irq00Handler:
	pushState

	mov rdi, 0
	call irqDispatcher
	
	mov rdi, rsp
	call switchProcess
	mov rsp, rax

	mov al, 20h
	out 20h, al
	
	popState
	iretq

; Keyboard
irq01Handler:
	irqHandlerMaster 1

; Zero Division Exception
exception0Handler:
	exceptionHandler 00h

; Invalid Operand Exception
exception6Handler:
	exceptionHandler 06h

; General Protection Exception
exception0DHandler:
	exceptionHandler 0Dh

; Page Fault Exception
exception0EHandler:
	exceptionHandler 0Eh

syscallHandler:
	mov rcx, r10
	mov r9, rax
	call syscallDispatcher
	iretq

; To avoid ticking unnecesarily (e.g. when a process yields the CPU)
awakeScheduler:
	pushState
	
	mov rdi, rsp
	call switchProcess
	mov rsp, rax

	popState
	iretq

int81:
	int 81h
	ret

hlt:
	sti
	hlt
	ret

cli:
	cli
	ret

sti:
	sti
	ret

SECTION .bss
regdata	resq	18
