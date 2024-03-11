BITS 64

	global asm_strcasecmp

	section .text

	; int asm_strcasecmp(const char *s1, const char *s2);
	; @s1: rdi
	; @s2: rsi
	; Description: Compares bytes of 2 strings & returns value upon reaching
	;              inequal byte or end of either string, case-insensitive
	; Return:
	;     0: reached end of both strings (equal length), all bytes match
	;     Otherwise: s1 byte subtracted by s2 byte where inequal (not 0)

asm_strcasecmp:
	push rbp
	mov rbp, rsp
	push rbx

_while:
	movzx eax, byte [rdi]
	movzx ebx, byte [rsi]
	jmp _compare

_continue:
	inc rdi
	inc rsi
	jmp _while

_compare:
	cmp al, 0
	je _done
	cmp bl, 0
	je _done
	cmp al, bl
	je _continue

_possible_nonmatch:
	cmp al, 65
	jge _case
	cmp bl, 65
	jge _case
	jmp _done

_case:
	cmp al, 90
	jle _convert_al
	cmp bl, 90
	jle _convert_bl
	jmp _done

_convert_al:
	add al, 32
	jmp _compare

_convert_bl:
	add bl, 32
	jmp _compare

_done:
	sub rax, rbx
	pop rbx
	mov rsp, rbp
	pop rbp
	ret
