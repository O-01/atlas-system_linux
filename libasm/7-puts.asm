BITS 64

	global asm_puts

	section .text

	; size_t asm_puts(const char *str);
	; @str: rdi
	; Description: Prints string to stdout
	; Return: Number of characters printed

extern asm_strlen

asm_puts:
	push rbp
	mov rbp, rsp
	push rsi           ; save rsi
	push rdx           ; save rdx
	call asm_strlen    ; shares rdi input buffer
	mov rdx, rax       ; rdx to syscall = len of rdi as number of char to write
	mov rax, 1         ; syscall number is 1 = sys_write
	mov rsi, rdi       ; rsi to syscall = str as input buffer
	mov rdi, 1         ; rdi to syscall = fd 1 = stdout_fileno
	syscall            ; 1<write>(rdi<1>, rsi<str>, rdx<strlen(str)>)
	pop rdx
	pop rsi
	mov rsp, rbp
	pop rbp
	ret
