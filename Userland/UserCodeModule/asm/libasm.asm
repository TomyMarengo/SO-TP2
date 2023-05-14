GLOBAL runInvalidOpcode
GLOBAL runDivideByZero

section .text

runInvalidOpcode:
	db	0xFF, 0xFF
	ret ; Necessary? Not really... Just here for good measure.

runDivideByZero:
	mov rax, 0
	div rax
	ret

section .bss
