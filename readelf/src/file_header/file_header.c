#include "hreadelf.h"

static void magic_(unsigned char *ident);
static void class_(unsigned char clas);
static void data_(unsigned char dat);
static void version_(unsigned char version);
static void osabi_(unsigned char osabi);
static void type_(uint16_t type);
static void machine_(uint16_t machine);

/**
 * fh_details_32 - prints file header information for 32-bit ELF file
 * @map_32: Elf32_Ehdr info struct for 32-bit input file
*/
void fh_details_32(Elf32_Ehdr *map_32)
{
	printf(FH_MAGIC), magic_(map_32->e_ident);
	printf("  %-35s", FH_CLASS), class_(map_32->e_ident[EI_CLASS]);
	printf("  %-35s", FH_DATA), data_(map_32->e_ident[EI_DATA]);
	printf("  %-35s", FH_VERSION), version_(map_32->e_ident[EI_VERSION]);
	printf("  %-35s", FH_OSABI), osabi_(map_32->e_ident[EI_OSABI]);
	printf("  %-35s%d\n", FH_ABIVERSION, map_32->e_ident[EI_ABIVERSION]);
	printf("  %-35s", FH_TYPE), type_(map_32->e_type);
	printf("  %-35s", FH_MACHINE), machine_(map_32->e_machine);
	printf("  %-35s%#x\n", FH_EVERSION, map_32->e_version);
	printf("  %-35s0x%x\n", FH_ENTRY, map_32->e_entry);
	printf("  %-35s%d (bytes into file)\n", FH_PHOFF, map_32->e_phoff);
	printf("  %-35s%d (bytes into file)\n", FH_SHOFF, map_32->e_shoff);
	printf("  %-35s0x%x\n", FH_FLAGS, map_32->e_flags);
	printf("  %-35s%d (bytes)\n", FH_EHSIZE, map_32->e_ehsize);
	printf("  %-35s%d (bytes)\n", FH_PHENTSIZ, map_32->e_phentsize);
	printf("  %-35s%d\n", FH_PHNUM, map_32->e_phnum);
	printf("  %-35s%d (bytes)\n", FH_SHENTSIZ, map_32->e_shentsize);
	printf("  %-35s%d\n", FH_SHNUM, map_32->e_shnum);
	printf("  %-35s%d\n", FH_SHSTRNDX, map_32->e_shstrndx);
}

/**
 * fh_details_64 - prints file header information for 64-bit ELF file
 * @map_64: Elf64_Ehdr info struct for 64-bit input file
*/
void fh_details_64(Elf64_Ehdr *map_64)
{
	printf(FH_MAGIC), magic_(map_64->e_ident);
	printf("  %-35s", FH_CLASS), class_(map_64->e_ident[EI_CLASS]);
	printf("  %-35s", FH_DATA), data_(map_64->e_ident[EI_DATA]);
	printf("  %-35s", FH_VERSION), version_(map_64->e_ident[EI_VERSION]);
	printf("  %-35s", FH_OSABI), osabi_(map_64->e_ident[EI_OSABI]);
	printf("  %-35s%d\n", FH_ABIVERSION, map_64->e_ident[EI_ABIVERSION]);
	printf("  %-35s", FH_TYPE), type_(map_64->e_type);
	printf("  %-35s", FH_MACHINE), machine_(map_64->e_machine);
	printf("  %-35s%#x\n", FH_EVERSION, map_64->e_version);
	printf("  %-35s0x%lx\n", FH_ENTRY, map_64->e_entry);
	printf("  %-35s%ld (bytes into file)\n", FH_PHOFF, map_64->e_phoff);
	printf("  %-35s%ld (bytes into file)\n", FH_SHOFF, map_64->e_shoff);
	printf("  %-35s0x%x\n", FH_FLAGS, map_64->e_flags);
	printf("  %-35s%d (bytes)\n", FH_EHSIZE, map_64->e_ehsize);
	printf("  %-35s%d (bytes)\n", FH_PHENTSIZ, map_64->e_phentsize);
	printf("  %-35s%d\n", FH_PHNUM, map_64->e_phnum);
	printf("  %-35s%d (bytes)\n", FH_SHENTSIZ, map_64->e_shentsize);
	printf("  %-35s%d\n", FH_SHNUM, map_64->e_shnum);
	printf("  %-35s%d\n", FH_SHSTRNDX, map_64->e_shstrndx);
}

/**
 * magic_ - prints EI_NIDENT bytes from ELF file
 * @ident: array of identifying bytes from ELF file
*/
static void magic_(unsigned char *ident)
{
	int iter = 0;

	for (; iter != EI_NIDENT; iter++)
		printf("%.2x ", ident[iter]);
	putchar('\n');
}

/**
 * class_ - prints file class (32-bit / 64-bit) of input file
 * @clas: unsigned char representing file class of ELF file
*/
static void class_(unsigned char clas)
{
	switch (clas)
	{
		case ELFCLASS32:
			printf("ELF32");
		break;
		case ELFCLASS64:
			printf("ELF64");
		break;
		case ELFCLASSNONE:
			printf("none");
		break;
		default:
			printf("<unknown: %x>", clas);
		break;
	}
	putchar('\n');
}

/**
 * data_ - prints endianness of ELF file
 * @dat: unsigned char representing endianness of ELF file
*/
static void data_(unsigned char dat)
{
	switch (dat)
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
			printf("none");
		break;
		default:
			printf("<unknown: %x>", dat);
		break;
	}
	putchar('\n');
}

/**
 * version_ - prints ELF version of input ELF file
 * @version: unsigned char representing version of ELF file
*/
static void version_(unsigned char version)
{
	switch (version)
	{
		case EV_CURRENT:
			printf("%d (current)", version);
		break;
		case EV_NONE:
			fprintf(stderr, "%s: Error: Invalid ELF version\n", prog.name);
			error_manager(0, 1);
		break;
		default:
			printf("<unknown: %x>", version);
		break;
	}
	putchar('\n');
}

/**
 * osabi_ - prints OS/ABI identification of input ELF file
 * @osabi: unsigned char representing OS/ABI
*/
static void osabi_(unsigned char osabi)
{
	switch (osabi)
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
			printf("<unknown: %x>", osabi);
		break;
	}
	putchar('\n');
}

/**
 * type_ - prints file type of ELF file
 * @type: 16-bit unsigned integer representing object file type
*/
static void type_(uint16_t type)
{
	switch (type)
	{
		case ET_NONE:
			printf("NONE (None)");
		break;
		case ET_REL:
			printf("REL (Relocatable file)");
		break;
		case ET_EXEC:
			printf("EXEC (Executable file)");
		break;
		case ET_DYN:
			printf("DYN (Shared object file)");
			/* printf("DYN (Position-Independent Executable file)"); */
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
 * machine_ - prints designated architecture of input 64-bit ELF file
 * @machine: 16-bit unsigned integer representing designated architecture
*/
static void machine_(uint16_t machine)
{
	switch (machine)
	{
		case EM_X86_64:
			printf("Advanced Micro Devices X86-64");
		break;
		case EM_386:
			printf("Intel 80386");
		break;
		case EM_SPARC:
			printf("Sparc");
		break;
		default:
			printf("WIP");
		break;
	}
	putchar('\n');
}
