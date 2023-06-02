GLOBAL spinLock
GLOBAL unlock

spinLock:
  mov rax, 0
  mov al, 1
  xchg al, [rdi]
  cmp al, 0
  jne spinLock
  ret

unlock:
    mov byte [rdi], 0
    ret