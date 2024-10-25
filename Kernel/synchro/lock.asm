global release
global acquire


acquire:
    mov al, 0
.retry:
    xchg [rdx], al
    test al, al
    jz .retry
    ret

release:
    mov dword [rdx], 1
    ret