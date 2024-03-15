#include "signals.h"

/**
 * current_handler_signal - retrieves the current handler of the signal SIGINT
 * Return: pointer to the current handler function of SIGINT, NULL on failure
*/
void (*current_handler_signal(void))(int)
{
	__sighandler_t current_handler = signal(SIGINT, NULL);

	return (current_handler == SIG_ERR
			? NULL
			: signal(SIGINT, current_handler));
}
