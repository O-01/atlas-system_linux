BITS 64

	global asm_strstr

	section .text

	; char *asm_strstr(const char *haystack, const char *needle);
	; @haystack: rdi
	; @needle: rsi
	; Description: Locates substring needle within string haystack
	; Return: Address where needle begins within haystack

asm_strstr:
	push rbp
	mov rbp, rsp
	push rbx
	push rcx
	mov rcx, rsi
	movzx ebx, byte [rcx]

_while:
	movzx eax, byte [rdi]
	cmp al, 0
	je _inequal_or_null
	cmp al, bl
	je _while_candidate
	inc rdi
	jmp _while

_while_candidate:
	movzx eax, byte [rdi]
	movzx ebx, byte [rcx]
	cmp al, 0
	je _inequal_or_null
	cmp bl, 0
	je _found
	cmp bl, al
	jne _almost
	inc rcx
	jmp _while_candidate

_almost:
	mov rcx, rsi
	movzx ebx, byte [rcx]
	jmp _while

_found:
	mov rax, rdi
	jmp _done

_inequal_or_null:
	mov rax, 0
	jmp _done

_done:
	pop rcx
	pop rbx
	mov rsp, rbp
	pop rbp
	ret