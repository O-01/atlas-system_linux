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

_loopin_time:
	cmp byte [rdi + rax], 0
	je _done
	inc rax
	jmp _loopin_time

_done:
	mov rsp, rbp
	pop rbp
	ret
