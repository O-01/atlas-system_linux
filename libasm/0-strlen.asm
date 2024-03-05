BITS 64

	global asm_strlen

	section .text

	; size_t asm_strlen(const char *str);
	; @str: rdi
	; Description: Returns length of a string
	; Return: length of input string

asm_strlen:
	push rbp
	mov rbp, rsp
	mov rax, 0

asm_strlen_loopin_time:
	cmp byte [rdi + rax], 0
	je asm_strlen_done
	inc rax
	jmp asm_strlen_loopin_time

asm_strlen_done:
	mov rsp, rbp
	pop rbp
	ret
