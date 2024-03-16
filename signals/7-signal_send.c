#include "signals.h"

/**
 * main - entry point of program
 * @argc: argument count upon launch
 * @argv: argument vector containing arguments supplied upon launch as strings
 * Return: 0 upon success, EXIT_FAILURE upon incorrect launch argument supplied
*/
int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Usage: %s <pid>\n", argv[0]);
		return (EXIT_FAILURE);
	}
	if (kill(atoi(argv[1]), SIGINT) == -1)
	{
		perror(argv[0]);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
