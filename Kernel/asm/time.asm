GLOBAL readValue

section .text

readValue:
	mov al, dil
	out 70h, al
	in al, 71h
	ret