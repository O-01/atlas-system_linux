BITS 64

	global asm_strpbrk

	section .text

	; char *asm_strpbrk(const char *s, const char *accept);
	; @s: rdi
	; @accept: rsi
	; Description: Finds address of first byte in s that matches any byte in
	;              accept
	; Return: Address of first byte in s that matches any byte in accept

asm_strpbrk:
	push rbp
	mov rbp, rsp
	push rbx
	push rcx
	push rdx
	mov rcx, 0

_while:
	movzx ebx, byte [rdi]
	cmp bl, 0
	jne _verify_accept
	jmp _not_found

_continue:
	inc rdi
	mov rcx, 0
	jmp _while

_verify_accept:
	movzx edx, byte [rsi + rcx]
	cmp dl, 0
	je _continue
	cmp bl, dl
	je _found
	inc rcx
	jmp _verify_accept

_found:
	mov rax, rdi
	jmp _done

_not_found:
	mov rax, 0
	jmp _done

_done:
	pop rdx
	pop rcx
	pop rbx
	mov rsp, rbp
	pop rbp
	ret
