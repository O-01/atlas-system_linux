BITS 64

	global strlen

	section .text

	; size_t asm_strlen(const char *str);
	; Returns length of a string
	; Return: length of input string

asm_strlen: