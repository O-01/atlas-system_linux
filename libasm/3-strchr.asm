BITS 64

	global asm_strchr

	section .text

	; char *asm_strchr(const char *s, int c);
	; @s: rdi
	; @c: rsi
	; Description: Finds first occurrence of char in input str
	; Return: Pointer to character, 0/NULL otherwise

asm_strchr:
	push rbp
	mov rbp, rsp

_while:
	movzx eax, byte [rdi]
	cmp al, sil
	je _found
	cmp al, 0
	je _not_found
	inc rdi
	jmp _while

_found:
	mov rax, rdi
	jmp _done

_not_found:
	mov rax, 0
	jmp _done

_done:
	mov rsp, rbp
	pop rbp
	ret
