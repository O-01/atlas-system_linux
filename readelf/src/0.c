#include "hreadelf.h"

static void endianer(char *fd_map);

prog_dt prog;

/**
 * main - entry to program
 * @argc: count of arguments to program upon launch
 * @argv: vector of arguments to program upon launch
 * Return: 0 upon success
*/

int main(int argc, char **argv)
{
	char *pages = NULL;
	struct stat file_dt;

	prog.name = argv[0];
	if (argc == 2)
	{
		prog.fd = open(argv[1], O_RDONLY);
		if (prog.fd == -1)
		{
			fprintf(stderr, "%s: Error: '%s': No such file\n",
				prog.name, argv[1]);
			error_manager("open", 1);
		}
		if (fstat(prog.fd, &file_dt) == -1)
			error_manager("stat", 1);
		else if (!S_ISREG(file_dt.st_mode))
		{
			fprintf(stderr, "%s: Error: '%s': is not an ordinary file\n",
				prog.name, argv[1]);
			error_manager(NULL, 1);
		}
		pages = mmap(
			NULL, file_dt.st_size, PROT_READ, MAP_PRIVATE, prog.fd, 0
		);
		if (pages == MAP_FAILED)
			error_manager("mmap", 0);
		endianer(pages);
		if (!file_header(pages))
			munmap(pages, file_dt.st_size), error_manager(NULL, 1);
	}
	else
		fprintf(stderr, "%s: Usage: %s ELF-file\n", prog.name, prog.name);
	return (0);
}

/**
 * endianer - sets endianness in program data struct
 * @fd_map: mmap of input ELF file
*/

static void endianer(char *fd_map)
{
	if (fd_map[EI_DATA] == ELFDATA2LSB)
		prog.endianness = 1;
	else if (fd_map[EI_DATA] == ELFDATA2MSB)
		prog.endianness = 2;
}
