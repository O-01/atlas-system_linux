#include "hreadelf.h"

/**
 * is_elf - verifies if input file is ELF based on magic number
 * @fd_map: mmap of input file
 * Return: 1 if magic number found (ELF), 0 otherwise (not ELF)
*/

int is_elf(char *fd_map) { return (!memcmp(fd_map, ELFMAG, 4) ? 1 : 0); }

/**
 * error_manager - prints error and/or closes file descriptor, as applicable,
 *                 then exits
 * @fd: file descriptor, to be closed if supplied
 * @cause: name of operation that caused error
 * @err: errno supplied for exit status
*/

void error_manager(int fd, char *cause, int err)
{
	if (fd != -1)
		close(fd);
	if (cause)
		perror(cause);
	exit(err);
}
