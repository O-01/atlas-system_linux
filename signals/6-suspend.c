#include "signals.h"

static void interrupt_handler(int sig);

/**
 * main - entry point of program
 * Return: EXIT_SUCCESS
*/
int main(void)
{
	signal(SIGINT, interrupt_handler);
	pause();
	printf("Signal received\n");
	return (EXIT_SUCCESS);
}

/**
 * interrupt_handler - handler for SIGINT
 * @sig: incoming signal, expected to be SIGINT
*/
static void interrupt_handler(int sig) { printf("Caught %d\n", sig); }
