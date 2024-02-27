#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/**
 * main - entry to program
 * @argc: count of arguments to program upon launch
 * @argv: vector of arguments to program upon launch
 * Return: 0 upon success
*/
int main(__attribute__((unused)) int argc, char **argv)
{
	char *command[] = {"readelf", "-W", "-S", "", NULL};

	command[3] = argv[1];
	if (execvp(command[0], command) == -1)
		return (1);
	return (0);
}
