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
		printf("Usage: %s <signum>\n", argv[0]);
		return (EXIT_FAILURE);
	}
	printf("%s: %s\n", argv[1], strsignal(atoi(argv[1])));
	return (0);
}
