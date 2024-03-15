#include "signals.h"

static void interrupt_handler(int sig);

/**
 * handle_signal - sets a handler for SIGINT
 * Return: 0 upon success, -1 upon error
*/
int handle_signal(void)
{
	__sighandler_t output = signal(SIGINT, interrupt_handler);

	if (output == SIG_ERR)
		return (-1);
	return (0);
}

/**
 * interrupt_handler - defines behavior upon receiving SIGINT
 * @interrupt_sig: incoming signal, expected to be SIGINT
*/
static void interrupt_handler(int interrupt_sig)
{
	char interrupt_sig_char = interrupt_sig + '0'; /* should be 2 */
	/* definition/initialization would be within if-statement in C99+ */
	if (interrupt_sig == SIGINT)
	{
		/* s/n/printf is no-no - unsafe because non-reentrant */
		write(1, "Gotcha! [", 9);
		write(1, &interrupt_sig_char, 1);
		write(1, "]\n", 2);
	}
}
