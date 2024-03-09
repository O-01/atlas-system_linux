#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "libasm.h"

#define S1  "Holberton School"
#define S2  "HOLBERTON SCHOOL"
#define S3  "Holberton SchooL"
#define S4  "holberton socool"

/**
 * main - Program entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    printf("std: %d - asm: %d\n", strcasecmp(S1, S1), asm_strcasecmp(S1, S1));
    printf("std: %d - asm: %d\n", strcasecmp(S1, S2), asm_strcasecmp(S1, S2));
    printf("std: %d - asm: %d\n", strcasecmp(S1, S3), asm_strcasecmp(S1, S3));
    printf("std: %d - asm: %d\n", strcasecmp(S1, S4), asm_strcasecmp(S1, S4));
    assert(strcasecmp(S1, S1) == asm_strcasecmp(S1, S1));
    assert(strcasecmp(S1, S2) == asm_strcasecmp(S1, S2));
    assert(strcasecmp(S1, S3) == asm_strcasecmp(S1, S3));
    assert(strcasecmp(S1, S4) == asm_strcasecmp(S1, S4));

    printf("All good!\n");
    return (EXIT_SUCCESS);
}
