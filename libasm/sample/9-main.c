#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "libasm.h"

#define S1  "Holberton School"
#define S2  "HOLBERTON SCHOOL"
#define S3  "Holberton SchooL"
#define S4  "holberton socool"
#define S5  "Holberton"
#define S6  ""

/**
 * main - Program entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    printf("std: %d - asm: %d\n", strncasecmp(S1, S3, 1), asm_strncasecmp(S1, S3, 1));
    printf("std: %d - asm: %d\n", strncasecmp(S1, S2, 11), asm_strncasecmp(S1, S2, 11));
    printf("std: %d - asm: %d\n", strncasecmp(S1, S3, 11), asm_strncasecmp(S1, S3, 11));
    printf("std: %d - asm: %d\n", strncasecmp(S1, S2, 16), asm_strncasecmp(S1, S2, 16));
    printf("std: %d - asm: %d\n", strncasecmp(S1, S3, 16), asm_strncasecmp(S1, S3, 16));
    printf("std: %d - asm: %d\n", strncasecmp(S1, S4, 11), asm_strncasecmp(S1, S4, 11));
    printf("std: %d - asm: %d\n", strncasecmp(S1, S4, 16), asm_strncasecmp(S1, S4, 16));
    printf("std: %d - asm: %d\n", strncasecmp(S5, S6, strlen("Holberton")), asm_strncasecmp(S5, S6, strlen("Holberton")));
    printf("std: %d - asm: %d\n", strncasecmp(S6, S5, strlen("Holberton")), asm_strncasecmp(S6, S5, strlen("Holberton")));
    assert(strncasecmp(S1, S3, 1) == asm_strncasecmp(S1, S3, 1));
    assert(strncasecmp(S1, S2, 11) == asm_strncasecmp(S1, S2, 11));
    assert(strncasecmp(S1, S3, 11) == asm_strncasecmp(S1, S3, 11));
    assert(strncasecmp(S1, S2, 16) == asm_strncasecmp(S1, S2, 16));
    assert(strncasecmp(S1, S3, 16) == asm_strncasecmp(S1, S3, 16));
    assert(strncasecmp(S1, S4, 11) == asm_strncasecmp(S1, S4, 11));
    assert(strncasecmp(S1, S4, 16) == asm_strncasecmp(S1, S4, 16));

    printf("All good!\n");
    return (EXIT_SUCCESS);
}
