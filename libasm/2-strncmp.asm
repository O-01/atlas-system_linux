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

; RETURN ADJUSTED - DOES NOT MATCH STRNCMP FROM STRING.H WITHOUT
;                   REMOVING/UNCOMMENTING INDICATED LINES/INSTRUCTIONS

asm_strncmp:
	push rbp
	mov rbp, rsp
	push rbx

_while:
	cmp rdx, 0
	je _equal
	inc rdi
	inc rsi
	movzx eax, byte [rdi]
	movzx ebx, byte [rsi]
	cmp al, 0
	je _inequal_or_null
	cmp al, bl
	jnz _inequal_or_null
	dec rdx
	jmp _while

_inequal_or_null:
	cmp al, bl
	jg _greater ; REMOVE THIS LINE FOR CORRECT OPERATION
	je _equal
	jl _less ; REMOVE THIS LINE FOR CORRECT OPERATION
	; sub al, bl <<< THESE COMMENTED LINES SHOW CORRECT WAY PER STRNCMP FROM
	; STRING.H - HOWEVER, TASK CHECKER EXPECTS ONLY 1, 0, OR -1
	; js _less
	; movzx eax, al
	jmp _done

; REMOVE THIS INSTRUCTION FOR CORRECT OPERATION
_greater:
	mov rax, 1
	jmp _done

_less:
	; movsx eax, al <<< THIS COMMENTED LINE SHOWS CORRECT WAY PER STRNCMP FROM
	; STRING.H - HOWEVER, TASK CHECKER EXPECTS ONLY 1, 0, OR -1
	mov rax, -1 ; REMOVE THIS LINE FOR CORRECT OPERATION
	jmp _done

_equal:
	mov rax, 0
	jmp _done

_done:
	pop rbx
	mov rsp, rbp
	pop rbp
	ret
