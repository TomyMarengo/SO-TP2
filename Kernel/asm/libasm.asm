GLOBAL cpuVendor
GLOBAL saveRegisters

EXTERN storeRegisters

section .text

cpuVendor:
	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx
	ret

; Save registers for inforeg
saveRegisters:
	push rbp
	mov rbp, rsp

	mov rdi, rsp
	add rdi, 112		; 14*8 = 112
	call storeRegisters

    mov rsp, rbp
	pop rbp
	ret
