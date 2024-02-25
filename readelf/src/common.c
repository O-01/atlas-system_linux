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
 * @cause: name of operation that caused error
 * @err: errno supplied for exit status
*/

void error_manager(__attribute__ ((unused)) char *cause, int err)
{
	if (prog.fd && prog.fd != -1)
		close(prog.fd);
	/* if (cause) */
		/* perror(cause); */
	exit(err);
}

/**
 * data_ - prints endianness of ELF file
 * @fd_map: mmap of input ELF file
*/

void data_(char *fd_map)
{
	switch (fd_map[EI_DATA])
	{
		case ELFDATA2LSB:
			printf("2's complement, little endian");
			prog.endianness = ELFDATA2LSB;
		break;
		case ELFDATA2MSB:
			printf("2's complement, big endian");
			prog.endianness = ELFDATA2MSB;
		break;
		case ELFDATANONE:
			fprintf(stderr, "%s: Error: Invalid class\n", prog.name);
			error_manager(NULL, 1);
		break;
		default:
			break;
	}
	putchar('\n');
}
