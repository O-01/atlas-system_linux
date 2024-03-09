BITS 64

	global asm_memcpy

	section .text

	; void *asm_memcpy(void *dest, const void *src, size_t n);
	; @dest: rdi
	; @src: rsi
	; @n: rdx
	; Description: Copies n bytes of src to dest
	; Return: Pointer to dest

asm_memcpy:
	push rbp
	mov rbp, rsp
	push rcx
	cmp rdx, 0
	jle _done
	dec rdx

_while:
	mov cl, byte [rsi + rdx]
	mov [rdi + rdx], cl
	dec rdx
	cmp rdx, 0
	jl _done
	jmp _while

_done:
	mov rax, rdi
	pop rcx
	mov rsp, rbp
	pop rbp
	ret
