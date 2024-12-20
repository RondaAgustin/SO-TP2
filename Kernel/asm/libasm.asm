GLOBAL cpu_vendor, outb, inb

section .text
	
cpu_vendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid

	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

outb: 
	mov rdx, rdi
	mov rax, rsi
	out dx, al
	ret

inb:
	mov rdx, rdi
	in al, dx
	ret