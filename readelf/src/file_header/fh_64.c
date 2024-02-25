#include "hreadelf.h"

static void type_64(Elf64_Ehdr *map_64);
static void machine_64(Elf64_Ehdr *map_64);

/**
 * fh_details_64 - prints file header information for 64-bit ELF file
 * @map_64: mmap of input 64-bit ELF file cast to Elf64_Ehdr info struct
*/

void fh_details_64(Elf64_Ehdr *map_64)
{
	printf("  %-35s", "Type:"), type_64(map_64);
	printf("  %-35s", "Machine:"), machine_64(map_64);
	printf("  %-35s%#x\n",
		"Version:", map_64->e_version);
	printf("  %-35s0x%lx\n",
		"Entry point address:", map_64->e_entry);
	printf("  %-35s%ld (bytes into file)\n",
		"Start of program headers:", map_64->e_phoff);
	printf("  %-35s%ld (bytes into file)\n",
		"Start of section headers:", map_64->e_shoff);
	printf("  %-35s0x%x\n",
		"Flags:", map_64->e_flags);
	printf("  %-35s%d (bytes)\n",
		"Size of this header:", map_64->e_ehsize);
	printf("  %-35s%d (bytes)\n",
		"Size of program headers:", map_64->e_phentsize);
	printf("  %-35s%d\n",
		"Number of program headers:", map_64->e_phnum);
	printf("  %-35s%d (bytes)\n",
		"Size of section headers:", map_64->e_shentsize);
	printf("  %-35s%d\n",
		"Number of section headers:", map_64->e_shnum);
	printf("  %-35s%d\n",
		"Section header string table index:", map_64->e_shstrndx);
}

/**
 * type_64 - prints file type of 64-bit ELF file
 * @map_64: mmap of input 64-bit ELF file cast to Elf64_Ehdr info struct
*/

static void type_64(Elf64_Ehdr *map_64)
{
	switch (map_64->e_type)
	{
		case ET_NONE:
			printf("Unknown type");
		break;
		case ET_REL:
			printf("REL (Relocatable file)");
		break;
		case ET_EXEC:
			printf("EXEC (Executable file)");
		break;
		case ET_DYN:
			/* printf("DYN (Shared object file)"); */
			printf("DYN (Position-Independent Executable file)");
		break;
		case ET_CORE:
			printf("CORE (Core file)");
		break;
		default:
			break;
	}
	putchar('\n');
}

/**
 * machine_64 - prints designated architecture of input 64-bit ELF file
 * @map_64: mmap of input 64-bit ELF file cast to Elf64_Ehdr info struct
*/

static void machine_64(Elf64_Ehdr *map_64)
{
	switch (map_64->e_machine)
	{
		case EM_X86_64:
			printf("Advanced Micro Devices X86-64");
		break;
		/* possibly add more later */
		default:
			break;
	}
	putchar('\n');
}
