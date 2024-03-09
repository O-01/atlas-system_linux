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
	push rdx
	movzx ebx, byte [rsi]
	cmp bl, 0
	je _null_rsi
	jmp _while

_keep_going:
	movzx ebx, byte [rsi]

_while:
	movzx eax, byte [rdi]
	cmp al, 0
	je _inequal_or_null
	cmp al, bl
	je _candidate
	inc rdi
	jmp _while

_candidate:
	mov rcx, rsi
	mov rbx, rdi
	jmp _while_candidate

_while_candidate:
	movzx eax, byte [rdi]
	movzx edx, byte [rcx]
	cmp dl, 0
	je _found
	cmp al, 0
	je _inequal_or_null
	cmp dl, al
	jne _keep_going
	inc rcx
	inc rdi
	jmp _while_candidate

_found:
	mov rax, rbx
	jmp _done

_inequal_or_null:
	xor rax, rax
	jmp _done

_null_rsi:
	mov rax, rdi

_done:
	pop rdx
	pop rcx
	pop rbx
	mov rsp, rbp
	pop rbp
	ret