#include "hreadelf.h"

static int read_sh_32(elf_dt *elfen);
static int read_sh_64(elf_dt *elfen);
static char *sh_str_table_32(elf_dt *elfen);
static char *sh_str_table_64(elf_dt *elfen);
static char *type_(unsigned int type);
static char *flags_(unsigned long flags);
static void print_col_titles(void);
static void print_flag_key(void);

/**
 * sh_details_32 - prints section header information for 32-bit ELF file
 * @elfen: ELF file data struct
 * Return: 0 upon success, 1 upon failure
*/
int sh_details_32(elf_dt *elfen)
{
	int iter = 0;
	char *str_table = NULL;

	if (!elfen->fh32.e_shnum)
	{
		printf("\nThere are no sections in this file.\n");
		return (1);
	}
	printf("There are %d section headers", elfen->fh32.e_shnum);
	printf(", starting at offset 0x%x:\n", elfen->fh32.e_shoff);
	if (!read_sh_32(elfen))
		error_manager(0, 1);
	if (prog.endianness == MSB)
		convert_sh_endian_32(elfen->sh32, elfen->fh32.e_shnum);
	str_table = sh_str_table_32(elfen);
	printf("\nSection Headers:\n");
	print_col_titles();
	for (iter = 0; iter != elfen->fh32.e_shnum; iter++)
	{
		printf("  [%2u] %-17s ", iter, str_table + elfen->sh32[iter].sh_name);
		printf("%-15.15s ", type_(elfen->sh32[iter].sh_type));
		printf("%8.8x ", elfen->sh32[iter].sh_addr);
		printf("%6.6x ", elfen->sh32[iter].sh_offset);
		printf("%6.6x ", elfen->sh32[iter].sh_size);
		printf("%2.2x ", elfen->sh32[iter].sh_entsize);
		printf("%3s ", flags_(elfen->sh32[iter].sh_flags));
		printf("%2u ", elfen->sh32[iter].sh_link);
		printf("%3u ", elfen->sh32[iter].sh_info);
		printf("%2u\n", elfen->sh32[iter].sh_addralign);
	}
	print_flag_key();
	free(str_table);
	return (0);
}

/**
 * sh_details_64 - prints section header information for 64-bit ELF file
 * @elfen: ELF file data struct
 * Return: 0 upon success, 1 upon failure
*/
int sh_details_64(elf_dt *elfen)
{
	int iter = 0;
	char *str_table = NULL;

	if (!elfen->fh64.e_shnum)
	{
		printf("\nThere are no sections in this file.\n");
		return (1);
	}
	printf("There are %d section headers", elfen->fh64.e_shnum);
	printf(", starting at offset 0x%lx:\n", elfen->fh64.e_shoff);
	if (!read_sh_64(elfen))
		error_manager(0, 1);
	if (prog.endianness == MSB)
		convert_sh_endian_64(elfen->sh64, elfen->fh64.e_shnum);
	str_table = sh_str_table_64(elfen);
	printf("\nSection Headers:\n");
	print_col_titles();
	for (iter = 0; iter != elfen->fh64.e_shnum; iter++)
	{
		printf("  [%2u] %-17s ", iter, str_table + elfen->sh64[iter].sh_name);
		printf("%-15.15s ", type_(elfen->sh64[iter].sh_type));
		printf("%16.16lx ", elfen->sh64[iter].sh_addr);
		printf("%6.6lx ", elfen->sh64[iter].sh_offset);
		printf("%6.6lx ", elfen->sh64[iter].sh_size);
		printf("%2.2lx ", elfen->sh64[iter].sh_entsize);
		printf("%3s ", flags_(elfen->sh64[iter].sh_flags));
		printf("%2u ", elfen->sh64[iter].sh_link);
		printf("%3u ", elfen->sh64[iter].sh_info);
		printf("%2lu\n", elfen->sh64[iter].sh_addralign);
	}
	print_flag_key();
	free(str_table);
	return (0);
}

/**
 * read_sh_32 - populates section header data for 32-bit ELF file
 * @elfen: ELF file data struct
 * Return: 1 upon success, 0 upon 0 shnum, mem allocation error, or read error
*/
static int read_sh_32(elf_dt *elfen)
{
	size_t shnum = elfen->fh32.e_shnum, rd = 0;
	size_t read_siz = elfen->fh32.e_shentsize * elfen->fh32.e_shnum;
	char *sh_map = NULL;

	if (!shnum)
		return (0);
	sh_map = calloc(shnum, read_siz * shnum);
	if (!sh_map)
		return (0);
	lseek(prog.fd, elfen->fh32.e_shoff, SEEK_SET);
	rd = read(prog.fd, sh_map, read_siz);
	if (rd < read_siz)
		return (0);
	elfen->sh32 = (void *)sh_map;
	return (1);
}

/**
 * read_sh_64 - populates section header data for 64-bit ELF file
 * @elfen: ELF file data struct
 * Return: 1 upon success, 0 upon 0 shnum, mem allocation error, or read error
*/
static int read_sh_64(elf_dt *elfen)
{
	size_t shnum = elfen->fh64.e_shnum, rd = 0;
	char *sh_map = NULL;
	size_t read_siz = elfen->fh64.e_shentsize * elfen->fh64.e_shnum;

	if (!shnum)
		return (0);
	sh_map = calloc(shnum, read_siz * shnum);
	if (!sh_map)
		return (0);
	lseek(prog.fd, elfen->fh64.e_shoff, SEEK_SET);
	rd = read(prog.fd, sh_map, read_siz);
	if (rd < read_siz)
		return (0);
	elfen->sh64 = (void *)sh_map;
	return (1);
}

/**
 * sh_str_table_32 - retrieves section header string table from 32-bit ELF file
 * @elfen: ELF file data struct
 * Return: section header string table upon success
*/
static char *sh_str_table_32(elf_dt *elfen)
{
	char *out = NULL;

	out = calloc(1, elfen->sh32[elfen->fh32.e_shstrndx].sh_size);
	if (!out)
		error_manager(0, 1);
	lseek(prog.fd, elfen->sh32[elfen->fh32.e_shstrndx].sh_offset, SEEK_SET);
	read(prog.fd, out, elfen->sh32[elfen->fh32.e_shstrndx].sh_size);
	return (out);
}

/**
 * sh_str_table_64 - retrieves section header string table from 64-bit ELF file
 * @elfen: ELF file data struct
 * Return: section header string table upon success
*/
static char *sh_str_table_64(elf_dt *elfen)
{
	char *out = NULL;

	out = calloc(1, elfen->sh64[elfen->fh64.e_shstrndx].sh_size);
	if (!out)
		error_manager(0, 1);
	lseek(prog.fd, elfen->sh64[elfen->fh64.e_shstrndx].sh_offset, SEEK_SET);
	read(prog.fd, out, elfen->sh64[elfen->fh64.e_shstrndx].sh_size);
	return (out);
}

/**
 * type_ - retrieves name of section type
 * @type: unsigned 32-bit integer representing section type
 * Return: name of section type
*/
static char *type_(uint32_t type)
{
	static char buf[32];

	switch (type)
	{
		case SHT_NULL:			return ("NULL");
		case SHT_PROGBITS:		return ("PROGBITS");
		case SHT_SYMTAB:		return ("SYMTAB");
		case SHT_STRTAB:		return ("STRTAB");
		case SHT_RELA:			return ("RELA");
		case SHT_HASH:			return ("HASH");
		case SHT_DYNAMIC:		return ("DYNAMIC");
		case SHT_NOTE:			return ("NOTE");
		case SHT_NOBITS:		return ("NOBITS");
		case SHT_REL:			return ("REL");
		case SHT_SHLIB:			return ("SHLIB");
		case SHT_DYNSYM:		return ("DYNSYM");
		case SHT_INIT_ARRAY:	return ("INIT_ARRAY");
		case SHT_FINI_ARRAY:	return ("FINI_ARRAY");
		case SHT_PREINIT_ARRAY:	return ("PREINIT_ARRAY");
		case SHT_GNU_HASH:		return ("GNU_HASH");
		case SHT_GROUP:			return ("GROUP");
		case SHT_SYMTAB_SHNDX:	return ("SYMTAB SECTION INDICIES");
		case SHT_GNU_verdef:	return ("VERDEF");
		case SHT_GNU_verneed:	return ("VERNEED");
		case SHT_GNU_versym:	return ("VERSYM");
		case 0x6ffffff0:		return ("VERSYM");
		case 0x6ffffffc:		return ("VERDEF");
		case 0x7ffffffd:		return ("AUXILIARY");
		case 0x7fffffff:		return ("FILTER");
		case 0x6ffffff1:		return ("LOOS+ffffff1");
		case 0x6ffffff3:		return ("LOOS+ffffff3");
		case SHT_GNU_LIBLIST:	return ("GNU_LIBLIST");
		default:
			sprintf(buf, "%08x: <unknown>", type);
			return (buf);
	}
}

/**
 * flags_ - retrieves section flags
 * @flags: unsigned long representing flags of a section
 * Return: string representation of section flags
*/
static char *flags_(unsigned long flags)
{
	static char buf[32];
	char *p = buf;

	if (flags & SHF_WRITE)
		*p++ = 'W';
	if (flags & SHF_ALLOC)
		*p++ = 'A';
	if (flags & SHF_EXECINSTR)
		*p++ = 'X';
	if (flags & SHF_MERGE)
		*p++ = 'M';
	if (flags & SHF_STRINGS)
		*p++ = 'S';
	if (flags & SHF_INFO_LINK)
		*p++ = 'I';
	if (flags & SHF_LINK_ORDER)
		*p++ = 'L';
	if (flags & SHF_OS_NONCONFORMING)
		*p++ = 'O';
	if (flags & SHF_GROUP)
		*p++ = 'G';
	if (flags & SHF_TLS)
		*p++ = 'T';
	if (flags & SHF_EXCLUDE)
		*p++ = 'E';
	*p = 0;
	return (buf);
}

/**
 * print_col_titles - prints section column names
*/
static void print_col_titles(void)
{
	printf("  [%2s] ", "Nr");
	printf("%-17s ", "Name");
	printf("%-15.15s ", "Type");
	if (prog.fclass == ELFCLASS32)
		printf("%-8.8s ", "Addr");
	else if (prog.fclass == ELFCLASS64)
		printf("%-16.16s ", "Address");
	printf("%-6.6s ", "Off");
	printf("%-6.6s ", "Size");
	printf("%2.2s ", "ES");
	printf("%3s ", "Flg");
	printf("%2s ", "Lk");
	printf("%3s ", "Inf");
	printf("%2s\n", "Al");
}

/**
 * print_flag_key - prints to section flags
*/
static void print_flag_key(void)
{
	printf("Key to Flags:\n  ");
	printf("W (write), ");
	printf("A (alloc), ");
	printf("X (execute), ");
	printf("M (merge), ");
	printf("S (strings)");
	if (prog.fclass == ELFCLASS64)
		printf(", l (large)");
	printf("\n  ");
	printf("I (info), ");
	printf("L (link order), ");
	printf("G (group), ");
	printf("T (TLS), ");
	printf("E (exclude), ");
	printf("x (unknown)\n  ");
	printf("O (extra OS processing required) ");
	printf("o (OS specific), ");
	printf("p (processor specific)\n");
}