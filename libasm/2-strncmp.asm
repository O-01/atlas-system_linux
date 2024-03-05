BITS 64

	global asm_strncmp

	section .text

	; int asm_strncmp(const char *s1, const char *s2, size_t n);
	; @s1: rdi
	; @s2: rsi
	; @n: rdx
	; Description: Compares up to n bytes of 2 strings & returns value upon
	;              reaching inequal byte or end of either string
	; Return:
	;    0: reached end of both strings (equal length), all bytes match
	;    Otherwise: s1 byte subtracted by s2 byte at point where inequal

asm_strncmp:
	push rbp
	mov rbp, rsp
	push rbx

_while:
	movzx eax, byte [rdi]
	movzx ebx, byte [rsi]
	cmp al, 0
	je _inequal_or_null
	cmp al, bl
	jnz _inequal_or_null
	inc rdi
	inc rsi
	dec rdx
	jz _equal
	jmp _while

_inequal_or_null:
	cmp al, bl
	je _equal
	sub al, bl
	js _less
	movzx eax, al
	jmp _done

_less:
	movsx eax, al
	jmp _done

_equal:
	mov rax, 0
	jmp _done

_done:
	pop rbx
	mov rsp, rbp
	pop rbp
	ret
