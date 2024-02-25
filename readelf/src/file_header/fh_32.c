#include "hreadelf.h"

static void type_32(Elf32_Ehdr *map_32);
static void machine_32(Elf32_Ehdr *map_32);

/**
 * fh_details_32 - prints file header information for 32-bit ELF file
 * @map_32: mmap of input 32-bit ELF file cast to Elf32_Ehdr info struct
*/

void fh_details_32(Elf32_Ehdr *map_32)
{
	printf("  %-35s", "Type:"), type_32(map_32);
	printf("  %-35s", "Machine:"), machine_32(map_32);
	printf("  %-35s%#x\n",
		"Version:", map_32->e_version);
	printf("  %-35s0x%x\n",
		"Entry point address:", map_32->e_entry);
	printf("  %-35s%d (bytes into file)\n",
		"Start of program headers:", map_32->e_phoff);
	printf("  %-35s%d (bytes into file)\n",
		"Start of section headers:", map_32->e_shoff);
	printf("  %-35s0x%x\n",
		"Flags:", map_32->e_flags);
	printf("  %-35s%d (bytes)\n",
		"Size of this header:", map_32->e_ehsize);
	printf("  %-35s%d (bytes)\n",
		"Size of program headers:", map_32->e_phentsize);
	printf("  %-35s%d\n",
		"Number of program headers:", map_32->e_phnum);
	printf("  %-35s%d (bytes)\n",
		"Size of section headers:", map_32->e_shentsize);
	printf("  %-35s%d\n",
		"Number of section headers:", map_32->e_shnum);
	printf("  %-35s%d\n",
		"Section header string table index:", map_32->e_shstrndx);
}

/**
 * type_32 - prints file type of 32-bit ELF file
 * @map_32: mmap of input 32-bit ELF file cast to Elf32_Ehdr info struct
*/

static void type_32(Elf32_Ehdr *map_32)
{
	switch (map_32->e_type)
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
			printf("DYN (Shared object file)");
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
 * machine_32 - prints designated architecture of input 32-bit ELF file
 * @map_32: mmap of input 32-bit ELF file cast to Elf32_Ehdr info struct
*/

static void machine_32(Elf32_Ehdr *map_32)
{
	switch (map_32->e_machine)
	{
		case EM_386:
			printf("Intel 80386");
		break;
		/* possibly add more later */
		default:
			break;
	}
	putchar('\n');
}
