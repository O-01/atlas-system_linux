#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "signals.h"

/* Our functions */
static void print_hello(int interrupt_sig);
static void set_print_hello(void);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    void (*handler)(int);
    int i;

    handler = current_handler_signal();
    printf("Address of the current handler: %#lx\n", (unsigned long int)handler);

    /* Set 'print_hello()` as the handler for SIGINT */
    set_print_hello();

    handler = current_handler_signal();
    printf("Address of the 'print_hello' function: %#lx\n", (unsigned long int)&print_hello);
    printf("Address of the current handler: %#lx\n", (unsigned long int)handler);

    for (i = 0; ; i++)
    {
        printf("[%d] Wait for it ...\n", i);
        sleep(1);
    }
    return (EXIT_SUCCESS);
}

/**
 * set_print_hello - sets a handler for SIGINT
 * Return: 0 upon success, -1 upon error
*/
static void set_print_hello(void) { signal(SIGINT, print_hello); }

/**
 * print_hello - defines behavior upon receiving SIGINT
 * @interrupt_sig: incoming signal, expected to be SIGINT
*/
static void print_hello(int interrupt_sig)
{
	if (interrupt_sig == SIGINT)
		write(1, "Hello :)\n", 9);
}
