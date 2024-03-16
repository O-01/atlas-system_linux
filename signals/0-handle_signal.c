#include "signals.h"

static void interrupt_handler(int sig);

/**
 * handle_signal - sets a handler for SIGINT
 * Return: 0 upon success, -1 upon error
*/
int handle_signal(void)
{
	return (signal(SIGINT, interrupt_handler) == SIG_ERR ? -1 : 0);
}

/**
 * interrupt_handler - defines behavior upon receiving SIGINT
 * @interrupt_sig: incoming signal, expected to be SIGINT
*/
static void interrupt_handler(__attribute__((unused)) int interrupt_sig)
{
	/* s/n/printf is no-no - unsafe because non-reentrant */
	write(1, "Gotcha! [2]\n", 12);
}
