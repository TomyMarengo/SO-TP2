GLOBAL resetmain

EXTERN main
EXTERN getStackBase
EXTERN initializeKernelBinary

loader:
	call initializeKernelBinary	; Set up the kernel binary, and get thet stack address
	jmp mn

resetmain:
	call getStackBase
mn:	mov rsp, rax				; Set up the stack with the returned address
	call main
hang:
	cli
	hlt	; halt machine should kernel return
	jmp hang
