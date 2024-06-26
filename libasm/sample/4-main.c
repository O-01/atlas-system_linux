#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "libasm.h"

#define S1  "Holberton School"
#define S2  "School"
#define S3  "Socool"
#define S4 ""

/**
 * main - Program entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    printf("std: %p - asm: %p\n", strstr(S1, S2), asm_strstr(S1, S2));
    printf("std: %p - asm: %p\n", strstr(S1, S3), asm_strstr(S1, S3));
    printf("std: %p - asm: %p\n", strstr(S1, S1), asm_strstr(S1, S1));
    printf("std: %p - asm: %p\n", strstr(S1, S4), asm_strstr(S1, S4));
    printf("std: %p - asm: %p\n", strstr(S4, S4), asm_strstr(S4, S4));
    assert(strstr(S1, S2) == asm_strstr(S1, S2));
    assert(strstr(S1, S3) == asm_strstr(S1, S3));
    assert(strstr(S1, S1) == asm_strstr(S1, S1));
    assert(strstr(S1, S4) == asm_strstr(S1, S4));
    assert(strstr(S4, S4) == asm_strstr(S4, S4));

    printf("All good!\n");
    return (EXIT_SUCCESS);
}
