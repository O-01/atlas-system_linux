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

asm_strncmp_while:
	cmp rdx, 0
	je asm_strncmp_equal
	movzx eax, byte [rdi]
	movzx ebx, byte [rsi]
	inc rdi
	inc rsi
	cmp al, 0
	je asm_strncmp_inequal_or_null
	cmp al, bl
	jnz asm_strncmp_inequal_or_null
	dec rdx
	jmp asm_strncmp_while

asm_strncmp_inequal_or_null:
	cmp al, bl
	je asm_strncmp_equal
	sub al, bl
	js asm_strncmp_less
	movzx eax, al
	jmp asm_strncmp_done

asm_strncmp_less:
	movsx eax, al
	jmp asm_strncmp_done

asm_strncmp_equal:
	mov rax, 0
	jmp asm_strncmp_done

asm_strncmp_done:
	pop rbx
	mov rsp, rbp
	pop rbp
	ret
