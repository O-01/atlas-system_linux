#include "signals.h"

/* Our functions */
static int set_print_hello(void);
static void print_hello(int interrupt_sig);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    void (*handler)(int);
    int i;

    handler = current_handler_sigaction();
    printf("Address of the current handler: %#lx\n", (unsigned long int)handler);

    /* Set 'print_hello()` as the handler for SIGINT */
    set_print_hello();

    handler = current_handler_sigaction();
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
 * set_print_hello - sets a handler for the signal SIGINT
 * Return: 0 upon success, -1 upon error
*/
static int set_print_hello(void)
{
	sigact_t new_action;

	new_action.sa_handler = print_hello;
	sigemptyset(&new_action.sa_mask);
	new_action.sa_flags = 0;
	return (sigaction(SIGINT, &new_action, NULL));
}

/**
 * print_hello - defines behavior upon receiving SIGINT
 * @interrupt_sig: incoming signal, expected to be SIGINT
*/
static void print_hello(int interrupt_sig)
{
	if (interrupt_sig == SIGINT)
		write(1, "Hello :)\n", 9);
}
