#include "hreadelf.h"

static void magic_(char *fd_map);
static void class_(char *fd_map);
static void osabi_(char *fd_map);
static void version_(char *fd_map);

/**
 * file_header - manages print of file header information from input file
 * @fd_map: mmap of input ELF file
 * Return: 1 upon success, 0 if file is not an ELF file
*/

int file_header(char *fd_map)
{
	if (!is_elf(fd_map))
	{
		fprintf(stderr, "%s: Error: Not an ELF file - %s",
			prog.name, "it has the wrong magic bytes at the start\n");
		return (0);
	}
	printf("ELF Header:\n");
	printf("  Magic:   "), magic_(fd_map);
	printf("  %-35s", "Class:"), class_(fd_map);
	printf("  %-35s", "Data:"), data_(fd_map);
	printf("  %-35s", "Version:"), version_(fd_map);
	printf("  %-35s", "OS/ABI:"), osabi_(fd_map);
	printf("  %-35s%d\n", "ABI Version:", fd_map[EI_ABIVERSION]);
	if (fd_map[EI_CLASS] == ELFCLASS32)
		fh_details_32((Elf32_Ehdr *)fd_map);
	else if (fd_map[EI_CLASS] == ELFCLASS64)
		fh_details_64((Elf64_Ehdr *)fd_map);
	return (1);
}

/**
 * magic_ - prints EI_NIDENT bytes from ELF file
 * @fd_map: mmap of input ELF file
*/

static void magic_(char *fd_map)
{
	int iter = 0;

	for (; iter != EI_NIDENT; iter++)
		printf("%02x ", fd_map[iter]);
	putchar('\n');
}

/**
 * class_ - prints file class (32-bit / 64-bit) of input file
 * @fd_map: mmap of input ELF file
*/

static void class_(char *fd_map)
{
	switch (fd_map[EI_CLASS])
	{
		case ELFCLASS32:
			printf("ELF32");
		break;
		case ELFCLASS64:
			printf("ELF64");
		break;
		case ELFCLASSNONE:
			fprintf(stderr, "%s: Error: Invalid data encoding\n", prog.name);
			error_manager(NULL, 1);
		break;
		default:
			break;
	}
	putchar('\n');
}

/**
 * version_ - prints ELF version of input ELF file
 * @fd_map: mmap of input ELF file
*/

static void version_(char *fd_map)
{
	switch (fd_map[EI_VERSION])
	{
		case EV_CURRENT:
			printf("%d (current)", fd_map[EI_VERSION]);
		break;
		case EV_NONE:
			fprintf(stderr, "%s: Error: Invalid ELF version\n", prog.name);
			error_manager(NULL, 1);
		break;
		default:
			printf("Unknown version");
		break;
	}
	putchar('\n');
}

/**
 * osabi_ - prints OS/ABI identification of input ELF file
 * @fd_map: mmap of input ELF file
*/

static void osabi_(char *fd_map)
{
	switch (fd_map[EI_OSABI])
	{
		case ELFOSABI_SYSV:
			printf("UNIX - System V");
		break;
		case ELFOSABI_NETBSD:
			printf("UNIX - NetBSD");
		break;
		case ELFOSABI_GNU:
			printf("UNIX - GNU");
		break;
		case ELFOSABI_SOLARIS:
			printf("UNIX - Solaris");
		break;
		case ELFOSABI_FREEBSD:
			printf("UNIX - FreeBSD");
		break;
		case ELFOSABI_OPENBSD:
			printf("UNIX - OpenBSD");
		break;
		case ELFOSABI_ARM:
			printf("UNIX - ARM");
		break;
		default:
			break;
	}
	putchar('\n');
}
