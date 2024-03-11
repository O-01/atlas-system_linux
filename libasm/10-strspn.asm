BITS 64

	global asm_strspn

	section .text

	; size_t asm_strspn(const char *s, const char *accept);
	; @s: rdi
	; @accept: rsi
	; Description: Calculates number of bytes in initial segment from s
	;              consisting only of bytes from accept
	; Return: Number of bytes in initial segment from s consisting only of
	;         bytes from accept

asm_strspn:
	push rbp
	mov rbp, rsp
	push rbx
	push rcx
	push rdx
	mov rax, 0
	mov rcx, 0

_while:
	movzx ebx, byte [rdi]
	cmp bl, 0
	jne _verify_accept
	jmp _done

_continue:
	inc rdi
	inc rax
	mov rcx, 0
	jmp _while

_verify_accept:
	movzx edx, byte [rsi + rcx]
	cmp dl, 0
	je _done
	cmp bl, dl
	je _continue
	inc rcx
	jmp _verify_accept

_done:
	pop rdx
	pop rcx
	pop rbx
	mov rsp, rbp
	pop rbp
	ret
