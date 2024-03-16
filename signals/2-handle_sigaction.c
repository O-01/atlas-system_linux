#include "signals.h"

static void interrupt_handler(int interrupt_sig);

/**
 * handle_sigaction - sets a handler for the signal SIGINT
 * Return: 0 upon success, -1 upon error
*/
int handle_sigaction(void)
{
	sigact_t new_action;

	new_action.sa_handler = interrupt_handler;
	sigemptyset(&new_action.sa_mask);
	new_action.sa_flags = 0;
	return (sigaction(SIGINT, &new_action, NULL));
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
