# x86 Assembly - libASM
#### Concepts
*For this project, we expect you to look at this concept:*
- [[EYNTK] x86-64 Assembly](https://intranet.atlasschool.com/concepts/924)
#
## Resources
#### Read or watch:
- [ASM concept page](https://intranet.atlasschool.com/concepts/924)
- [x86 Assembly Language](https://en.wikipedia.org/wiki/X86_assembly_language)
- [x86 Registers](https://en.wikipedia.org/wiki/X86#x86_registers)
- [x86 Architecture](https://en.wikibooks.org/wiki/X86_Assembly/X86_Architecture)
- [Intel instructions](https://intranet.atlasschool.com/rltoken/aercrZHyE9KAl4TzoABPpA)
## Learning Objectives
At the end of this project, you are expected to be able to [explain to anyone](https://fs.blog/feynman-learning-technique/), **without the help of Google**:
### General
- The differences between `Intel` and `AT&T` syntaxes
- The different x86 GPR
- The different flag registers
- What is a stack frame
- How to setup a stack frame
- How to call a function
- How to make a system call
- How to pass parameters to a function
- How to retrieve parameters
## Requirements
### General
- Allowed editors: `vi`, `vim`, `emacs`
- All your files will be compiled on Ubuntu 20.04 LTS
- Your `ASM` programs and functions will be compiled with `NASM version 2.14.02` using the flags `-f elf64`
- All your files should end with a new line
- A `README.md` file, at the root of the folder of the project, is mandatory
- The prototypes of all your functions should be included in your header file called `libasm.h`
- Don’t forget to push your header files
- All your header files should be include guarded
### Allowed Functions and System Calls
- Unless specified otherwise, you are **NOT allowed** to do any call or make any system call. It means you’re not allowed to use either the `call` nor the `syscall` instructions.
#
## Tasks
#### 0. strlen
Write a copycat of the function `strlen`(3), in x86-64 Assembly
- Prototype when used in C: `size_t asm_strlen(const char *str);`
```
alex@~/libasm$ cat 0-main.c 
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "libasm.h"

#define S1  "Holberton School"
#define S2  ""
#define S3  "\0"

/**
 * main - Program entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    assert(strlen(S1) == asm_strlen(S1));
    assert(strlen(S2) == asm_strlen(S2));
    assert(strlen(S3) == asm_strlen(S3));

    printf("All good!\n");
    return (EXIT_SUCCESS);
}
alex@~/libasm$ gcc -Wall -Wextra -Werror -pedantic -g3 -c -o 0-main.o 0-main.c
alex@~/libasm$ nasm -f elf64 -o 0-strlen.o 0-strlen.asm
alex@~/libasm$ gcc -o 0-strlen 0-main.o 0-strlen.o
alex@~/libasm$ ./0-strlen 
All good!
alex@~/libasm$ 
```
#
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `libasm`
- File: `0-strlen.asm`
#
#### 1. strcmp
Write a copycat of the function `strcmp`(3), in x86-64 Assembly
- Prototype when used in C: `int asm_strcmp(const char *s1, const char *s2);`
```
alex@~/libasm$ cat 1-main.c 
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "libasm.h"

#define S1  "Holberton School"
#define S2  ""
#define S3  "Holberton Socool"

/**
 * main - Program entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    assert(strcmp(S1, S1) == asm_strcmp(S1, S1));
    assert(strcmp(S1, S2) == asm_strcmp(S1, S2));
    assert(strcmp(S1, S3) == asm_strcmp(S1, S3));

    printf("All good!\n");
    return (EXIT_SUCCESS);
}
alex@~/libasm$ gcc -Wall -Wextra -Werror -pedantic -g3 -c -o 1-main.o 1-main.c
alex@~/libasm$ nasm -f elf64 -o 1-strcmp.o 1-strcmp.asm
alex@~/libasm$ gcc -o 1-strcmp 1-main.o 1-strcmp.o
alex@~/libasm$ ./1-strcmp 
All good!
alex@~/libasm$ 
```
#
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `libasm`
- File: `1-strcmp.asm`
#
#### 2. strncmp
Write a copycat of the function `strncmp`(3), in x86-64 Assembly
- Prototype when used in C: `int asm_strncmp(const char *s1, const char *s2, size_t n);`
```
alex@~/libasm$ cat 2-main.c 
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "libasm.h"

#define S1  "Holberton School"
#define S2  ""
#define S3  "Holberton Socool"

/**
 * main - Program entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    assert(strncmp(S1, S3, 1) == asm_strncmp(S1, S3, 1));
    assert(strncmp(S1, S3, 11) == asm_strncmp(S1, S3, 11));
    assert(strncmp(S1, S3, 15) == asm_strncmp(S1, S3, 15));

    printf("All good!\n");
    return (EXIT_SUCCESS);
}
alex@~/libasm$ gcc -Wall -Wextra -Werror -pedantic -g3 -c -o 2-main.o 2-main.c
alex@~/libasm$ nasm -f elf64 -o 2-strncmp.o 2-strncmp.asm
alex@~/libasm$ gcc -o 2-strncmp 2-main.o 2-strncmp.o
alex@~/libasm$ ./2-strncmp 
All good!
alex@~/libasm$ 
```
#
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `libasm`
- File: `2-strncmp.asm`
#
#### 3. strchr
Write a copycat of the function `strchr`(3), in x86-64 Assembly
- Prototype when used in C: `char *asm_strchr(const char *s, int c);`
```
alex@~/libasm$ cat 3-main.c 
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "libasm.h"

#define S1  "Holberton School"
#define C1  'n'
#define C2  'S'
#define C3  's'

/**
 * main - Program entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    assert(strchr(S1, C1) == asm_strchr(S1, C1));
    assert(strchr(S1, C2) == asm_strchr(S1, C2));
    assert(strchr(S1, C3) == asm_strchr(S1, C3));

    printf("All good!\n");
    return (EXIT_SUCCESS);
}
alex@~/libasm$ gcc -Wall -Wextra -Werror -pedantic -g3 -c -o 3-main.o 3-main.c
alex@~/libasm$ nasm -f elf64 -o 3-strchr.o 3-strchr.asm
alex@~/libasm$ gcc -o 3-strchr 3-main.o 3-strchr.o
alex@~/libasm$ ./3-strchr 
All good!
alex@~/libasm$ 
```
#
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `libasm`
- File: `3-strchr.asm`
#
#### 4. strstr
Write a copycat of the function `strstr`(3), in x86-64 Assembly
- Prototype when used in C: `char *asm_strstr(const char *haystack, const char *needle);`
```
alex@~/libasm$ cat 4-main.c 
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "libasm.h"

#define S1  "Holberton School"
#define S2  "School"
#define S3  "Socool"

/**
 * main - Program entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    assert(strstr(S1, S2) == asm_strstr(S1, S2));
    assert(strstr(S1, S3) == asm_strstr(S1, S3));
    assert(strstr(S1, S1) == asm_strstr(S1, S1));

    printf("All good!\n");
    return (EXIT_SUCCESS);
}
alex@~/libasm$ gcc -Wall -Wextra -Werror -pedantic -g3 -c -o 4-main.o 4-main.c
alex@~/libasm$ nasm -f elf64 -o 4-strstr.o 4-strstr.asm
alex@~/libasm$ gcc -o 4-strstr 4-main.o 4-strstr.o
alex@~/libasm$ ./4-strstr 
All good!
alex@~/libasm$ 
```
#
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `libasm`
- File: `4-strstr.asm`
#
#### 5. memcpy
Write a copycat of the function `memcpy`(3), in x86-64 Assembly
- Prototype when used in C: `void *asm_memcpy(void *dest, const void *src, size_t n);`
```
alex@~/libasm$ cat 5-main.c 
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "libasm.h"

/**
 * main - Program entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    int i;
    char *src = strdup("Holberton");

    for (i = 0; i <= 9; i++)
    {
        char *dest = strdup("......... School");

        assert(asm_memcpy(dest, src, i) == dest);
        printf("%s\n", dest);
        free(dest);
    }
    free(src);
    printf("All good!\n");
    return (EXIT_SUCCESS);
}
alex@~/libasm$ gcc -Wall -Wextra -Werror -pedantic -g3 -c -o 5-main.o 5-main.c
alex@~/libasm$ nasm -f elf64 -o 5-memcpy.o 5-memcpy.asm
alex@~/libasm$ gcc -o 5-memcpy 5-main.o 5-memcpy.o
alex@~/libasm$ ./5-memcpy 
......... School
H........ School
Ho....... School
Hol...... School
Holb..... School
Holbe.... School
Holber... School
Holbert.. School
Holberto. School
Holberton School
All good!
alex@~/libasm$ 
```
#
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `libasm`
- File: `5-memcpy.asm`
#
#### 6. putc
Write a function that prints a single character on the standard output, in x86-64 Assembly
- Prototype when used in C: `size_t asm_putc(int c);`
- Where `c` holds the character to be printed
- Your function must return the total number of bytes written on the standard output
- For this task, you are allowed to use the `syscall` instruction only once in your file
```
alex@~/libasm$ cat 6-main.c 
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "libasm.h"

/**
 * main - Program entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    assert(asm_putc('H') == 1);
    assert(asm_putc('b') == 1);
    assert(asm_putc('t') == 1);
    assert(asm_putc('n') == 1);
    assert(asm_putc('\n') == 1);

    printf("All good!\n");
    return (EXIT_SUCCESS);
}
alex@~/libasm$ gcc -Wall -Wextra -Werror -pedantic -g3 -c -o 6-main.o 6-main.c
alex@~/libasm$ nasm -f elf64 -o 6-putc.o 6-putc.asm
alex@~/libasm$ gcc -o 6-putc 6-main.o 6-putc.o
alex@~/libasm$ ./6-putc 
Hbtn
All good!
alex@~/libasm$ 
```
#
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `libasm`
- File: `6-putc.asm`
#
#### 7. puts
Write a function that prints a string of characters on the standard output, in x86-64 Assembly
- Prototype when used in C: `size_t asm_puts(const char *str);`
- Where `str` holds the string to be printed
- Your function must return the total number of bytes written on the standard output
- You are not allowed to use any sort of `jump`
- Your file `0-strlen.asm` will be compiled as well, you are allowed to `call` it once in your file
- For this task, you are allowed to use the `syscall` instruction only once in your file
```
alex@~/libasm$ cat 7-main.c 
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "libasm.h"

/**
 * main - Program entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    assert(asm_puts("H") == 1);
    assert(asm_puts("olberton") == 8);
    assert(asm_puts(" School\n") == 8);

    printf("All good!\n");
    return (EXIT_SUCCESS);
}
alex@~/libasm$ gcc -Wall -Wextra -Werror -pedantic -g3 -c -o 7-main.o 7-main.c
alex@~/libasm$ nasm -f elf64 -o 7-puts.o 7-puts.asm
alex@~/libasm$ nasm -f elf64 -o 0-strlen.o 0-strlen.asm
alex@~/libasm$ gcc -o 7-puts 7-main.o 7-puts.o 0-strlen.o
alex@~/libasm$ ./7-puts 
Holberton School
All good!
alex@~/libasm$ 
```
#
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `libasm`
- File: `7-puts.asm`, `0-strlen.asm`
#
#### 8. strcasecmp
Write a copycat of the function `strcasecmp`(3), in x86-64 Assembly
- Prototype when used in C: `int asm_strcasecmp(const char *s1, const char *s2);`
```
alex@~/libasm$ cat 8-main.c 
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
    assert(strcasecmp(S1, S1) == asm_strcasecmp(S1, S1));
    assert(strcasecmp(S1, S2) == asm_strcasecmp(S1, S2));
    assert(strcasecmp(S1, S3) == asm_strcasecmp(S1, S3));
    assert(strcasecmp(S1, S4) == asm_strcasecmp(S1, S4));

    printf("All good!\n");
    return (EXIT_SUCCESS);
}
alex@~/libasm$ gcc -Wall -Wextra -Werror -pedantic -g3 -c -o 8-main.o 8-main.c
alex@~/libasm$ nasm -f elf64 -o 8-strcasecmp.o 8-strcasecmp.asm
alex@~/libasm$ gcc -o 8-strcasecmp 8-main.o 8-strcasecmp.o
alex@~/libasm$ ./8-strcasecmp 
All good!
alex@~/libasm$ 
```
#
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `libasm`
- File: `8-strcasecmp.asm`
#
#### 9. strncasecmp
Write a copycat of the function `strncasecmp`(3), in x86-64 Assembly
- Prototype when used in C: `int asm_strncasecmp(const char *s1, const char *s2, size_t n);`
```
alex@~/libasm$ cat 9-main.c 
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
alex@~/libasm$ gcc -Wall -Wextra -Werror -pedantic -g3 -c -o 9-main.o 9-main.c
alex@~/libasm$ nasm -f elf64 -o 9-strncasecmp.o 9-strncasecmp.asm
alex@~/libasm$ gcc -o 9-strncasecmp 9-main.o 9-strncasecmp.o
alex@~/libasm$ ./9-strncasecmp 
All good!
alex@~/libasm$ 
```
#
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `libasm`
- File: `9-strncasecmp.asm`
#
#### 10. strspn
Write a copycat of the function `strspn`(3), in x86-64 Assembly
- Prototype when used in C: `size_t asm_strspn(const char *s, const char *accept);`
```
alex@~/libasm$ cat 10-main.c 
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "libasm.h"

#define S1  "Holberton"
#define S2  "holberton"
#define S3  "HOLBERTON"

#define A1  "abcdefghijklmnopqrstuvwxyz"
#define A2  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

/**
 * main - Program entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    assert(strspn(S2, A1) == asm_strspn(S2, A1));
    assert(strspn(S2, A2) == asm_strspn(S2, A2));
    assert(strspn(S3, A1) == asm_strspn(S3, A1));
    assert(strspn(S3, A2) == asm_strspn(S3, A2));
    assert(strspn(S1, A1) == asm_strspn(S1, A1));
    assert(strspn(S1, A2) == asm_strspn(S1, A2));
    assert(strspn(S1, A1 A2) == asm_strspn(S1, A1 A2));

    printf("All good!\n");
    return (EXIT_SUCCESS);
}
alex@~/libasm$ gcc -Wall -Wextra -Werror -pedantic -g3 -c -o 10-main.o 10-main.c
alex@~/libasm$ nasm -f elf64 -o 10-strspn.o 10-strspn.asm
alex@~/libasm$ gcc -o 10-strspn 10-main.o 10-strspn.o
alex@~/libasm$ ./10-strspn 
All good!
alex@~/libasm$ 
```
#
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `libasm`
- File: `10-strspn.asm`
#
#### 11. strcspn
Write a copycat of the function `strcspn`(3), in x86-64 Assembly
- Prototype when used in C: `size_t asm_strcspn(const char *s, const char *reject);`
```
alex@~/libasm$ cat 11-main.c 
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "libasm.h"

#define S1  "Holberton"
#define S2  "holberton"
#define S3  "HOLBERTON"

#define A1  "abcdefghijklmnopqrstuvwxyz"
#define A2  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

/**
 * main - Program entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    assert(strcspn(S2, A1) == asm_strcspn(S2, A1));
    assert(strcspn(S2, A2) == asm_strcspn(S2, A2));
    assert(strcspn(S3, A1) == asm_strcspn(S3, A1));
    assert(strcspn(S3, A2) == asm_strcspn(S3, A2));
    assert(strcspn(S1, A1) == asm_strcspn(S1, A1));
    assert(strcspn(S1, A2) == asm_strcspn(S1, A2));
    assert(strcspn(S1, A1 A2) == asm_strcspn(S1, A1 A2));

    printf("All good!\n");
    return (EXIT_SUCCESS);
}
alex@~/libasm$ gcc -Wall -Wextra -Werror -pedantic -g3 -c -o 11-main.o 11-main.c
alex@~/libasm$ nasm -f elf64 -o 11-strcspn.o 11-strcspn.asm
alex@~/libasm$ gcc -o 11-strcspn 11-main.o 11-strcspn.o
alex@~/libasm$ ./11-strcspn 
All good!
alex@~/libasm$ 
```
#
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `libasm`
- File: `11-strcspn.asm`
#
#### 12. strpbrk
Write a copycat of the function `strpbrk`(3), in x86-64 Assembly
- Prototype when used in C: `char *asm_strpbrk(const char *s, const char *accept);`
```
alex@~/libasm$ cat 12-main.c 
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "libasm.h"

#define S1  "Holberton"
#define S2  "holberton"
#define S3  "HOLBERTON"

#define A1  "abcdefghijklmnopqrstuvwxyz"
#define A2  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define A3  "()[]{}<>n"

/**
 * main - Program entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    assert(strpbrk(S2, A1) == asm_strpbrk(S2, A1));
    assert(strpbrk(S2, A2) == asm_strpbrk(S2, A2));
    assert(strpbrk(S3, A1) == asm_strpbrk(S3, A1));
    assert(strpbrk(S3, A2) == asm_strpbrk(S3, A2));
    assert(strpbrk(S1, A3) == asm_strpbrk(S1, A3));

    printf("All good!\n");
    return (EXIT_SUCCESS);
}
alex@~/libasm$ gcc -Wall -Wextra -Werror -pedantic -g3 -c -o 12-main.o 12-main.c
alex@~/libasm$ nasm -f elf64 -o 12-strpbrk.o 12-strpbrk.asm
alex@~/libasm$ gcc -o 12-strpbrk 12-main.o 12-strpbrk.o
alex@~/libasm$ ./12-strpbrk 
All good!
alex@~/libasm$ 
```
#
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `libasm`
- File: `12-strpbrk.asm`
#
## ADVANCED TASKS
#### 13. puti
Write a function that prints a signed integer on the standard output, in x86-64 Assembly
- Prototype when used in C: `size_t asm_puti(int n);`
- Where `n` holds the integer to be printed
- Your function must return the total number of bytes written on the standard output
- Your file `6-putc.asm` will be compiled as well
- You are allowed to use the instruction `call` up to three times in your file
```
alex@~/libasm$ cat 100-main.c 
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "libasm.h"

/**
 * main - Program entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    assert(asm_puti(98) == 2);
    printf("\n");
    assert(asm_puti(-1) == 2);
    printf("\n");
    assert(asm_puti(-1024) == 5);
    printf("\n");
    assert(asm_puti(INT_MAX) == 10);
    printf("\n");
    assert(asm_puti(INT_MIN) == 11);
    printf("\n");
    assert(asm_puti(0) == 1);
    printf("\n");
    return (EXIT_SUCCESS);
}
alex@~/libasm$ gcc -Wall -Wextra -Werror -pedantic -g3 -c -o 100-main.o 100-main.c
alex@~/libasm$ nasm -f elf64 -o 100-puti.o 100-puti.asm
alex@~/libasm$ nasm -f elf64 -o 6-putc.o 6-putc.asm
alex@~/libasm$ gcc -o 100-puti 100-main.o 100-puti.o 6-putc.o
alex@~/libasm$ ./100-puti 
98
-1
-1024
2147483647
-2147483648
0
alex@~/libasm$ 
```
#
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `libasm`
- File: `100-puti.asm`, `6-putc.asm`
#
#### 14. puti_base
Write a function that prints a signed integer in a given base on the standard output, in x86-64 Assembly
- Prototype when used in C: `size_t asm_puti_base(int n, const char *base);`
- Where `n` holds the integer to be printed
- And `base` points to a string representing the base used to print `n`
- Your function must return the total number of bytes written on the standard output
- Your files `6-putc.asm` and `0-strlen.asm` will be compiled as well
- You are allowed to use the instruction `call` up to four times in your file
```
alex@~/libasm$ cat 101-main.c 
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "libasm.h"

/**
 * main - Program entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    asm_puti_base(98, "0123456789");
    printf("\n");
    asm_puti_base(-1, "0123456789");
    printf("\n");
    asm_puti_base(129342, "0123456789abcdef");
    printf("\n");
    asm_puti_base(781532, "01");
    printf("\n");
    asm_puti_base(INT_MAX, "abcdefghijklmnopqrstwxyz");
    printf("\n");
    asm_puti_base(INT_MIN, "0123456789ABCDEF");
    printf("\n");
    asm_puti_base(0, "01234567");
    printf("\n");
    return (EXIT_SUCCESS);
}
alex@~/libasm$ gcc -Wall -Wextra -Werror -pedantic -g3 -c -o 101-main.o 101-main.c
alex@~/libasm$ nasm -f elf64 -o 101-puti_base.o 101-puti_base.asm
alex@~/libasm$ nasm -f elf64 -o 6-putc.o 6-putc.asm
alex@~/libasm$ nasm -f elf64 -o 0-strlen.o 0-strlen.asm
alex@~/libasm$ gcc -o 101-puti_base 101-main.o 101-puti_base.o 6-putc.o 0-strlen.o
alex@~/libasm$ ./101-puti_base 
98
-1
1f93e
10111110110011011100
lfqqofh
-80000000
0
alex@~/libasm$ 
```
#
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `libasm`
- File: `101-puti_base.asm`, `6-putc.asm`, `0-strlen.asm`
#
