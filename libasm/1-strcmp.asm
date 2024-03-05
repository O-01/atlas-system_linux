BITS 64

	global asm_strcmp

	section .text

	; int asm_strcmp(const char *s1, const char *s2);
	; @s1: rdi
	; @s2: rsi
	; Description: Compares bytes of 2 strings & returns value upon reaching
	;              inequal byte or end of either string
	; Return:
	;     1: byte in s1 greater than that of s2
	;    -1: byte in s1 less than that of s2
	;     0: reached end of both strings (equal length), all bytes match

asm_strcmp:
	push rbp
	mov rbp, rsp
	push rbx

_while:
	movzx eax, byte [rdi]
	movzx ebx, byte [rsi]
	inc rdi
	inc rsi
	cmp al, 0
	je _inequal_or_null
	cmp al, bl
	jnz _inequal_or_null
	jmp _while

_inequal_or_null:
	cmp al, bl
	jg _greater
	jl _less
	je _equal

_greater:
	mov rax, 1
	jmp _done

_less:
	mov rax, -1
	jmp _done

_equal:
	mov rax, 0
	jmp _done

_done:
	pop rbx
	mov rsp, rbp
	pop rbp
	ret
