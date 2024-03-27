#include "hnm_hobjdump.h"

static int read_sh_32(elf_dt *elfen);
static int read_sh_64(elf_dt *elfen);
static char *sh_str_table_32(elf_dt *elfen);
static char *sh_str_table_64(elf_dt *elfen);
static int read_sym_32(elf_dt *elfen, int iter);
static int read_sym_64(elf_dt *elfen, int iter);
static char *sym_str_table_32(elf_dt *elfen, int iter);
static char *sym_str_table_64(elf_dt *elfen, int iter);
static char symbol_type_32(Elf32_Sym sym_32, Elf32_Shdr *sh32);
static char symbol_type_64(Elf64_Sym sym_64, Elf64_Shdr *sh64);
static size_t symbol_table_32(elf_dt *elfen, int iter);
static size_t symbol_table_64(elf_dt *elfen, int iter);

/**
 * sym_details_32 - prints symbol table data for 32-bit ELF file
 * @elfen: ELF file data struct
 * Return: 0 upon success, 1 upon failure
*/
int sym_details_32(elf_dt *elfen)
{
	int iter = 0, symbols = 0;
	char *str_table = NULL;

	if (!elfen->fh32.e_shnum)
	{
		printf("\nThere are no sections in this file.\n");
		return (1);
	}
	if (!read_sh_32(elfen))
		error_manager(0, 1);
	if (prog.endianness == MSB)
		convert_sh_endian_32(elfen->sh32, elfen->fh32.e_shnum);
	str_table = sh_str_table_32(elfen);
	for (; iter != elfen->fh32.e_shnum; iter++)
		if (elfen->sh32[iter].sh_type == SHT_SYMTAB)
			symbols += symbol_table_32(elfen, iter);
	free(str_table);
	if (!symbols)
		fprintf(stderr, "%s: %s: no symbols\n", prog.name, prog.file);
	return (0);
}

/**
 * sym_details_64 - prints symbol table data for 64-bit ELF file
 * @elfen: ELF file data struct
 * Return: 0 upon success, 1 upon failure
*/
int sym_details_64(elf_dt *elfen)
{
	int iter = 0, symbols = 0;
	char *str_table = NULL;

	if (!elfen->fh64.e_shnum)
	{
		printf("\nThere are no sections in this file.\n");
		return (1);
	}
	if (!read_sh_64(elfen))
		error_manager(0, 1);
	if (prog.endianness == MSB)
		convert_sh_endian_64(elfen->sh64, elfen->fh64.e_shnum);
	str_table = sh_str_table_64(elfen);
	for (; iter != elfen->fh64.e_shnum; iter++)
		if (elfen->sh64[iter].sh_type == SHT_SYMTAB)
			symbols += symbol_table_64(elfen, iter);
	free(str_table), str_table = NULL;
	if (!symbols)
		fprintf(stderr, "%s: %s: no symbols\n", prog.name, prog.file);
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
 * read_sym_32 - populates symbol table data for 32-bit ELF file section
 * @elfen: ELF file data struct
 * @iter: section header index of symbol table
 * Return: 1 upon success, 0 upon memory allocation or read error
*/
static int read_sym_32(elf_dt *elfen, int iter)
{
	size_t read_siz = elfen->sh32[iter].sh_size, rd = 0;
	char *sym_map = NULL;

	sym_map = calloc(1, read_siz);
	if (!sym_map)
		return (0);
	lseek(prog.fd, elfen->sh32[iter].sh_offset, SEEK_SET);
	rd = read(prog.fd, sym_map, read_siz);
	if (rd < read_siz)
		return (0);
	elfen->sym32 = (void *)sym_map;
	return (1);
}

/**
 * read_sym_64 - populates symbol table data for 64-bit ELF file section
 * @elfen: ELF file data struct
 * @iter: section header index of symbol table
 * Return: 1 upon success, 0 upon memory allocation or read error
*/
static int read_sym_64(elf_dt *elfen, int iter)
{
	size_t read_siz = elfen->sh64[iter].sh_size, rd = 0;
	char *sym_map = NULL;

	sym_map = calloc(1, read_siz);
	if (!sym_map)
		return (0);
	lseek(prog.fd, elfen->sh64[iter].sh_offset, SEEK_SET);
	rd = read(prog.fd, sym_map, read_siz);
	if (rd < read_siz)
		return (0);
	elfen->sym64 = (void *)sym_map;
	return (1);
}

/**
 * sym_str_table_32 - retrieves symbol string table of 32-bit ELF file section
 * @elfen: ELF file data struct
 * @iter: section header index of symbol table
 * Return: symbol string table of section upon success
*/
static char *sym_str_table_32(elf_dt *elfen, int iter)
{
	char *sym_str_table = NULL;

	sym_str_table = calloc(1, elfen->sh32[iter].sh_size);
	if (!sym_str_table)
		error_manager(0, 1);
	lseek(prog.fd, elfen->sh32[iter].sh_offset, SEEK_SET);
	read(prog.fd, sym_str_table, elfen->sh32[iter].sh_size);
	return (sym_str_table);
}

/**
 * sym_str_table_64 - retrieves symbol string table of 64-bit ELF file section
 * @elfen: ELF file data struct
 * @iter: section header index of symbol table
 * Return: symbol string table of section upon success
*/
static char *sym_str_table_64(elf_dt *elfen, int iter)
{
	char *sym_str_table = NULL;

	sym_str_table = calloc(1, elfen->sh64[iter].sh_size);
	if (!sym_str_table)
		error_manager(0, 1);
	lseek(prog.fd, elfen->sh64[iter].sh_offset, SEEK_SET);
	read(prog.fd, sym_str_table, elfen->sh64[iter].sh_size);
	return (sym_str_table);
}

/**
 * symbol_type_32 - retrieves character indicating symbol type (32-bit)
 * @sym_32: symbol table entry
 * @sh32: section header data array
 * Return: character indicating symbol type
*/
static char symbol_type_32(Elf32_Sym sym_32, Elf32_Shdr *sh32)
{
	char type;

	if (ELF32_ST_BIND(sym_32.st_info) == STB_GNU_UNIQUE)
		type = 'u';
	else if (ELF32_ST_BIND(sym_32.st_info) == STB_WEAK &&
		ELF32_ST_TYPE(sym_32.st_info) == STT_OBJECT)
		type = sym_32.st_shndx == SHN_UNDEF ? 'v' : 'V';
	else if (ELF32_ST_BIND(sym_32.st_info) == STB_WEAK)
		type = sym_32.st_shndx == SHN_UNDEF ? 'w' : 'W';
	else if (sym_32.st_shndx == SHN_UNDEF)
		type = 'U';
	else if (sym_32.st_shndx == SHN_ABS)
		type = 'A';
	else if (sym_32.st_shndx == SHN_COMMON)
		type = 'C';
	else if (sh32[sym_32.st_shndx].sh_type == SHT_NOBITS &&
		sh32[sym_32.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		type = 'B';
	else if (sh32[sym_32.st_shndx].sh_type == SHT_PROGBITS &&
		sh32[sym_32.st_shndx].sh_flags == SHF_ALLOC)
		type = 'R';
	else if (sh32[sym_32.st_shndx].sh_type == SHT_PROGBITS &&
		sh32[sym_32.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		type = 'D';
	else if (sh32[sym_32.st_shndx].sh_type == SHT_PROGBITS &&
		sh32[sym_32.st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
		type = 'T';
	else if (sh32[sym_32.st_shndx].sh_type == SHT_DYNAMIC)
		type = 'D';
	else
		type = 'T';
	if (ELF32_ST_BIND(sym_32.st_info) == STB_LOCAL && type != '?')
		type += 32;
	return (type);
}

/**
 * symbol_type_64 - retrieves character indicating symbol type (64-bit)
 * @sym_64: symbol table entry
 * @sh64: section header data array
 * Return: character indicating symbol type
*/
static char symbol_type_64(Elf64_Sym sym_64, Elf64_Shdr *sh64)
{
	char type;

	if (ELF64_ST_BIND(sym_64.st_info) == STB_GNU_UNIQUE)
		type = 'u';
	else if (ELF64_ST_BIND(sym_64.st_info) == STB_WEAK &&
		ELF64_ST_TYPE(sym_64.st_info) == STT_OBJECT)
		type = sym_64.st_shndx == SHN_UNDEF ? 'v' : 'V';
	else if (ELF64_ST_BIND(sym_64.st_info) == STB_WEAK)
		type = sym_64.st_shndx == SHN_UNDEF ? 'w' : 'W';
	else if (sym_64.st_shndx == SHN_UNDEF)
		type = 'U';
	else if (sym_64.st_shndx == SHN_ABS)
		type = 'A';
	else if (sym_64.st_shndx == SHN_COMMON)
		type = 'C';
	else if (sh64[sym_64.st_shndx].sh_type == SHT_NOBITS &&
		sh64[sym_64.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		type = 'B';
	else if (sh64[sym_64.st_shndx].sh_type == SHT_PROGBITS &&
		sh64[sym_64.st_shndx].sh_flags == SHF_ALLOC)
		type = 'R';
	else if (sh64[sym_64.st_shndx].sh_type == SHT_PROGBITS &&
		sh64[sym_64.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		type = 'D';
	else if (sh64[sym_64.st_shndx].sh_type == SHT_PROGBITS &&
		sh64[sym_64.st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
		type = 'T';
	else if (sh64[sym_64.st_shndx].sh_type == SHT_DYNAMIC)
		type = 'D';
	else
		type = 'T';
	if (ELF64_ST_BIND(sym_64.st_info) == STB_LOCAL && type != '?')
		type += 32;
	return (type);
}

/**
 * symbol_table_32 - prints symbol table data for 32-bit ELF file section
 * @elfen: ELF file data struct
 * @iter: section header index of symbol table
 * Return: number of symbols printed
*/
static size_t symbol_table_32(elf_dt *elfen, int iter)
{
	char *sym_str_table = NULL;
	size_t size = elfen->sh32[iter].sh_size / elfen->sh32[iter].sh_entsize;
	size_t entry = 0, count = 0;

	if (!read_sym_32(elfen, iter))
		error_manager(0, 1);
	if (prog.endianness == MSB)
		convert_sym_endian_32(elfen->sym32, size);
	sym_str_table = sym_str_table_32(elfen, iter + 1);
	for (; entry < size; entry++)
	{
		if ((elfen->sym32[entry].st_info & 0xf) == STT_SECTION ||
			(elfen->sym32[entry].st_info & 0xf) == STT_FILE || !entry)
			continue;
		if (symbol_type_32(elfen->sym32[entry], elfen->sh32) != 'U' &&
			symbol_type_32(elfen->sym32[entry], elfen->sh32) != 'w')
			printf("%8.8x ", elfen->sym32[entry].st_value);
		else
			printf("%8s ", "");
		printf("%c %s\n", symbol_type_32(elfen->sym32[entry], elfen->sh32),
			sym_str_table + elfen->sym32[entry].st_name), count++;
	}
	free(elfen->sym32), elfen->sym32 = NULL;
	free(sym_str_table), sym_str_table = NULL;
	return (count);
}

/**
 * symbol_table_64 - prints symbol table data for 64-bit ELF file section
 * @elfen: ELF file data struct
 * @iter: section header index of symbol table
 * Return: number of symbols printed
*/
static size_t symbol_table_64(elf_dt *elfen, int iter)
{
	char *sym_str_table = NULL;
	size_t size = elfen->sh64[iter].sh_size / elfen->sh64[iter].sh_entsize;
	size_t entry = 0, count = 0;

	if (!read_sym_64(elfen, iter))
		error_manager(0, 1);
	if (prog.endianness == MSB)
		convert_sym_endian_64(elfen->sym64, size);
	sym_str_table = sym_str_table_64(elfen, iter + 1);
	for (; entry < size; entry++)
	{
		if ((elfen->sym64[entry].st_info & 0xf) == STT_SECTION ||
			(elfen->sym64[entry].st_info & 0xf) == STT_FILE || !entry)
			continue;
		if (symbol_type_64(elfen->sym64[entry], elfen->sh64) != 'U' &&
			symbol_type_64(elfen->sym64[entry], elfen->sh64) != 'w')
			printf("%16.16lx ", elfen->sym64[entry].st_value);
		else
			printf("%16s ", "");
		printf("%c %s\n", symbol_type_64(elfen->sym64[entry], elfen->sh64),
			sym_str_table + elfen->sym64[entry].st_name), count++;
	}
	free(elfen->sym64), elfen->sym64 = NULL;
	free(sym_str_table), sym_str_table = NULL;
	return (count);
}
