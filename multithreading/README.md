# Multithreading
### Concepts
For this project, we expect you to look at this concept:
- [EYNTK - Multithreading](https://intranet.atlasschool.com/concepts/941)
### Resources
#### Read or watch:
- [pthread.h](https://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread.h.html)
- [Concurrency vs. Parallelism](https://howtodoinjava.com/java/multi-threading/concurrency-vs-parallelism/)
### Learning Objectives
At the end of this project, you are expected to be able to [explain to anyone](https://fs.blog/feynman-learning-technique/), without the help of Google:
- What is a thread
- What are the differences between a thread and a process
- What is the difference between Concurrency and Parallelism
- How to create a thread
- How to properly exit a thread
- How to handle mutual execution
- What is a deadlock
- What is a race condition
### Requirements
#### General
- Allowed editors: `vi`, `vim`, `emacs`
- All your files will be compiled on Ubuntu 14.04 LTS
- Your C programs and functions will be compiled with `gcc 4.8.4` using the flags `-Wall` `-Werror` `-Wextra` and `-pedantic`
- All your files should end with a new line
- A `README.md` file, at the root of the folder of the project, is mandatory
- Your code should use the `Betty` style. It will be checked using [betty-style.pl](https://github.com/hs-hq/Betty/blob/master/betty-style.pl) and [betty-doc.pl](https://github.com/hs-hq/Betty/blob/master/betty-doc.pl)
- You are not allowed to have more than 5 functions per file
- The prototypes of all your functions should be included in your header file called `multithreading.h`
- Don’t forget to push your header files
- All your header files should be include guarded
- You are allowed to use `global` variables
- You are allowed to use `static` functions and variables
#### Betty Compliance
- All the C source files in your directory and subdirectories must be [Betty-compliant](https://github.com/hs-hq/betty)
### Allowed Functions and System Calls
- Unless specified otherwise, you are allowed to use the C standard library
### Data structures
Here are the data structures used throughout this project, make sure to include them in your header file `multithreading.h`.
```
#include <stddef.h>
#include <stdint.h>

/**
 * struct pixel_s - RGB pixel
 *
 * @r: Red component
 * @g: Green component
 * @b: Blue component
 */
typedef struct pixel_s
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} pixel_t;

/**
 * struct img_s - Image
 *
 * @w:      Image width
 * @h:      Image height
 * @pixels: Array of pixels
 */
typedef struct img_s
{
    size_t w;
    size_t h;
    pixel_t *pixels;
} img_t;

/**
 * struct kernel_s - Convolution kernel
 *
 * @size:   Size of the matrix (both width and height)
 * @matrix: Kernel matrix
 */
typedef struct kernel_s
{
    size_t size;
    float **matrix;
} kernel_t;

/**
 * struct blur_portion_s - Information needed to blur a portion of an image
 *
 * @img:      Source image
 * @img_blur: Destination image
 * @x:        X position of the portion in the image
 * @y:        Y position of the portion in the image
 * @w:        Width of the portion
 * @h:        Height of the portion
 * @kernel:   Convolution kernel to use
 */
typedef struct blur_portion_s
{
    img_t const *img;
    img_t *img_blur;
    size_t x;
    size_t y;
    size_t w;
    size_t h;
    kernel_t const *kernel;
} blur_portion_t;
```
### Manual QA Review
#### It is your responsibility to request a review for your work on the Task #3 from a peer before the project’s deadline. If no peers have been reviewed, you should request a review from a TA or staff member.
#
#### 0. Thread entry point
Write a function that will serve as the entry point to a new thread.
- Prototype: `void *thread_entry(void *arg);`, where:
    - `arg` holds the address of a string that must be printed and followed by a new line.
- Make sure to respect the order in which strings are printed out in the following example:
```
alex@~/multithreading$ cat 0-main.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "multithreading.h"

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS
 */
int main(void)
{
    pthread_t tid;

    pthread_create(&tid, NULL, &thread_entry, "Holberton School");

    sleep(1);
    printf("Created thread ID -> %lu\n", tid);

    thread_entry("C is fun");
    printf("Should not be printed\n");
    return (EXIT_SUCCESS);
}
alex@~/multithreading$ gcc -Wall -Wextra -Werror -pedantic -g3 0-main.c 0-thread_entry.c -o 0-thread_entry -pthread
alex@~/multithreading$ ./0-thread_entry
Holberton School
Created thread ID -> 139851772159744
C is fun
alex@~/multithreading$
```
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `multithreading`
- File: `0-thread_entry.c`
#
#### 1. Thread Logger
Write a function that uses the `printf` family to print out a given formatted string.
- Prototype: `int tprintf(char const *format, ...);`
- The output must be preceded by the calling thread ID, as the following output shows.
```
alex@~/multithreading$ cat 1-main.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "multithreading.h"

/**
 * _thread_entry - Thread entry point. Prints out a string
 *
 * @arg: Unused
 *
 * Return: NULL
 */
void *_thread_entry(__attribute__((unused))void *arg)
{
    tprintf("Hello from thread\n");
    return (NULL);
}

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS
 */
int main(void)
{
    pthread_t tid;

    pthread_create(&tid, NULL, &_thread_entry, NULL);
    pthread_join(tid, NULL);

    tprintf("Hello from main thread\n");

    return (EXIT_SUCCESS);
}
alex@~/multithreading$ gcc -Wall -Wextra -Werror -pedantic -g3 1-main.c 1-tprintf.c -o 1-tprintf -pthread
alex@~/multithreading$ ./1-tprintf
[140095003338496] Hello from thread
[140095011714880] Hello from main thread
alex@~/multithreading$ ./1-tprintf
[139794647717632] Hello from thread
[139794656094016] Hello from main thread
alex@~/multithreading$
```
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `multithreading`
- File: `1-tprintf.c`
#
