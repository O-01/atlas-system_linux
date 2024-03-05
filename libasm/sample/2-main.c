#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../libasm.h"

#define S1  "Holberton School"
#define S2  ""
#define S3  "Holberton Socool"
#define S4  "Holberton"

/**
 * main - Program entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    printf("std: %d, asm: %d\n", strncmp(S1, S1, 15), asm_strncmp(S1, S1, 15));
    printf("std: %d, asm: %d\n", strncmp(S1, S3, 1), asm_strncmp(S1, S3, 1));
    printf("std: %d, asm: %d\n", strncmp(S1, S3, 11), asm_strncmp(S1, S3, 11));
    printf("std: %d, asm: %d\n", strncmp(S1, S3, 15), asm_strncmp(S1, S3, 15));
    printf("std: %d, asm: %d\n", strncmp(S1, S4, strlen(S1)), asm_strncmp(S1, S4, strlen(S1)));
    printf("std: %d, asm: %d\n", strncmp(S1, S4, strlen(S4)), asm_strncmp(S1, S4, strlen(S4)));
    printf("std: %d, asm: %d\n", strncmp(S4, S1, strlen(S1)), asm_strncmp(S4, S1, strlen(S1)));
    printf("std: %d, asm: %d\n", strncmp(S4, S1, strlen(S4)), asm_strncmp(S4, S1, strlen(S4)));
    assert(strncmp(S1, S3, 1) == asm_strncmp(S1, S3, 1));
    assert(strncmp(S1, S3, 11) == asm_strncmp(S1, S3, 11));
    assert(strncmp(S1, S3, 15) == asm_strncmp(S1, S3, 15));
    assert(strncmp(S1, S4, strlen(S1)) == asm_strncmp(S1, S4, strlen(S1)));
    assert(strncmp(S1, S4, strlen(S4)) == asm_strncmp(S1, S4, strlen(S4)));
    assert(strncmp(S4, S1, strlen(S1)) == asm_strncmp(S4, S1, strlen(S1)));
    assert(strncmp(S4, S1, strlen(S4)) == asm_strncmp(S4, S1, strlen(S4)));

    printf("All good!\n");
    return (EXIT_SUCCESS);
}
