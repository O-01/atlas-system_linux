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
	FILE *pipe_stream;
	size_t line_len = 0;
	char *line = NULL;
	char command[128];

	sprintf(command, "/usr/bin/readelf -W -l %s", argv[1]);
	pipe_stream = popen(command, "r");
	if (!pipe_stream)
		return (1);
	while (getline(&line, &line_len, pipe_stream) != -1)
		printf("%s", line);
	pclose(pipe_stream);
	return (0);
}
