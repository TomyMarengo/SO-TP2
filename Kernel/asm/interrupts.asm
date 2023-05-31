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
EXTERN printRegisters
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
	push rsp
	pushState

	mov rdi, %1 
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	pop rsp
	iretq
%endmacro

%macro exceptionHandler 1
	pushState

	call printRegisters

	mov rdi, %1 
	call exceptionDispatcher

	popState
	iretq

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
    mov ax, di  ; ax is a 16 bits mask
    out	0A1h, al
    pop rbp
    retn

; 8254 Timer (Timer Tick)
irq00Handler:
	pushState

	mov rdi, 0
	call irqDispatcher
	
	mov rdi, rsp
	call switchProcess
	mov rsp, rax

	; signal pic EOI (End of Interrupt)
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

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al
	
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
	aux resq 1
