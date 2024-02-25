#include "hreadelf.h"

/**
 * main - entry to program
 * @argc: count of arguments to program upon launch
 * @argv: vector of arguments to program upon launch
 * Return: 0 upon success
*/

int main(int argc, char **argv)
{
	int fd = 0;
	char *pages = NULL;
	struct stat file_dt;

	if (argc == 2)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd == -1)
			error_manager(fd, "open", errno);
		if (fstat(fd, &file_dt) == -1)
			error_manager(fd, "stat", errno);
		pages = mmap(NULL, file_dt.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
		if (pages == MAP_FAILED)
			error_manager(fd, "mmap", errno);
		if (!file_header(pages))
			munmap(pages, file_dt.st_size), error_manager(fd, NULL, 0);
	}
	else
		fprintf(stderr, "Usage: %s elf_filename\n", argv[0]);
	return (0);
}
