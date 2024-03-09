BITS 64

	global asm_putc

	section .text

	; size_t asm_putc(int c);
	; @c: rdi
	; Description: Prints character c to stdout
	; Return: 1, number of characters printed

asm_putc:
	push rbp
	mov rbp, rsp
	push rdi        ; rdi (char c) to stack
	mov rax, 1      ; syscall number is 1 = sys_write
	mov rdi, 1      ; rdi to syscall = fd 1 = stdout_fileno
	mov rsi, rsp    ; rsi to syscall = c from stack as input buffer
	mov rdx, 1      ; rdx to syscall = 1
	syscall         ; 1<write>(rdi<1>, rsi<c>, rdx<1>)
	mov rsp, rbp
	pop rbp
	ret
