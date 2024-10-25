global release
global acquire


acquire:
    mov al, 0
.retry:
    xchg [rdi], al
    test al, al
    jz .retry
    ret

release:
    mov byte [rdi], 1
    ret