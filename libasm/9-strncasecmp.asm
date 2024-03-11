BITS 64

	global asm_strncasecmp

	section .text

	; int asm_strncasecmp(const char *s1, const char *s2, size_t n);
	; @s1: rdi
	; @s2: rsi
	; @n: rdx
	; Description: Compares up to n bytes of 2 strings & returns value upon
	;              reaching inequal byte or end of either string,
	;              case-insensitive
	; Return:
	;     0: reached n or end of both strings (equal length), all bytes match
	;     Otherwise: s1 byte subtracted by s2 byte where inequal (not 0)

	; OUTPUT ADJUSTED PER PROJECT REQUIREMENTS - NOT NORMAL OUTPUT

asm_strncasecmp:
	push rbp
	mov rbp, rsp
	push rbx

_while:
	movzx eax, byte [rdi]
	movzx ebx, byte [rsi]
	cmp al, 0
	jne _al_case
	cmp bl, 0
	jne _bl_case
	jmp _done

_continue:
	inc rdi
	inc rsi
	dec rdx
	jz _done
	jmp _while

_al_case:
	cmp al, 65
	jl _bl_case
	cmp al, 90
	jg _bl_case
	add ax, 32

_bl_case:
	cmp bl, 65
	jl _compare
	cmp bl, 90
	jg _compare
	add bx, 32

_compare:
	cmp al, bl
	je _continue
	jmp _done

_done:
	sub rax, rbx
	pop rbx
	mov rsp, rbp
	pop rbp
	ret
