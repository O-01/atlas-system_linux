#define _XOPEN_SOURCE 700
#include "signals.h"

/**
 * current_handler_sigaction - retrieves the current handler of signal SIGINT
 * Return: pointer to the current handler of SIGINT, NULL upon failure
*/
void (*current_handler_sigaction(void))(int)
{
	sigact_t old_action;

	return (!sigaction(SIGINT, NULL, &old_action)
			? old_action.sa_handler
			: NULL);
}
