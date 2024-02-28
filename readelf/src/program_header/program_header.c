#include "hreadelf.h"

static void type_(uint16_t type);
static int read_ph_32(elf_dt *elfen);
static int read_ph_64(elf_dt *elfen);
static int read_sh_32(elf_dt *elfen);
static int read_sh_64(elf_dt *elfen);
static char *sh_str_table_32(elf_dt *elfen);
static char *sh_str_table_64(elf_dt *elfen);
static char *type_segment(uint32_t p_type);
static void print_col_titles(void);
static void interp(elf_dt *elfen, int iter);
static void section_segment_map_32(elf_dt *elfen, char *str_table);
static void section_segment_map_64(elf_dt *elfen, char *str_table);
static int sect_in_pseg_32(Elf32_Shdr sect, Elf32_Phdr pseg);
static int sect_in_pseg_64(Elf64_Shdr sect, Elf64_Phdr pseg);
static unsigned long sect_siz_32(Elf32_Shdr sect, Elf32_Phdr pseg);
static unsigned long sect_siz_64(Elf64_Shdr sect, Elf64_Phdr pseg);

/**
 * ph_details_32 - prints program header information for 32-bit ELF file
 * @elfen: ELF file data struct
 * Return: 0 upon success, 1 upon failure
*/
int ph_details_32(elf_dt *elfen)
{
	int iter = 0;
	char *str_table = NULL;

	if (!elfen->fh32.e_phnum)
	{
		printf("\nThere are no program headers in this file.\n");
		return (1);
	}
	printf("\nElf file type is "), type_(elfen->fh32.e_type);
	printf("Entry point 0x%x\n", elfen->fh32.e_entry);
	printf("There are %d program headers, ", elfen->fh32.e_phnum);
	printf("starting at offset %u\n", elfen->fh32.e_phoff);
	if (!read_sh_32(elfen))
		error_manager(0, 1);
	if (!read_ph_32(elfen))
		error_manager(0, 1);
	if (prog.endianness == MSB)
	{
		convert_sh_endian_32(elfen->sh32, elfen->fh32.e_shnum);
		convert_ph_endian_32(elfen->ph32, elfen->fh32.e_phnum);
	}
	str_table = sh_str_table_32(elfen);
	printf("\nProgram Headers:\n");
	print_col_titles();
	for (iter = 0; iter != elfen->fh32.e_phnum; iter++)
	{
		if (elfen->ph32[iter].p_type == PT_NULL)
			continue;
		printf("  %-14s ", type_segment(elfen->ph32[iter].p_type));
		printf("0x%6.6x ", elfen->ph32[iter].p_offset);
		printf("0x%8.8x ", elfen->ph32[iter].p_vaddr);
		printf("0x%8.8x ", elfen->ph32[iter].p_paddr);
		printf("0x%5.5x ", elfen->ph32[iter].p_filesz);
		printf("0x%5.5x ", elfen->ph32[iter].p_memsz);
		printf("%c", elfen->ph32[iter].p_flags & PF_R ? 'R' : ' ');
		printf("%c", elfen->ph32[iter].p_flags & PF_W ? 'W' : ' ');
		printf("%c ", elfen->ph32[iter].p_flags & PF_X ? 'E' : ' ');
		printf("%#x\n", elfen->ph32[iter].p_align);
		if (elfen->ph32[iter].p_type == PT_INTERP)
			interp(elfen, iter);
	}
	section_segment_map_32(elfen, str_table), free(str_table);
	return (0);
}

/**
 * ph_details_64 - prints program header information for 64-bit ELF file
 * @elfen: ELF file data struct
 * Return: 0 upon success, 1 upon failure
*/
int ph_details_64(elf_dt *elfen)
{
	int iter = 0;
	char *str_table = NULL;

	if (!elfen->fh64.e_phnum)
	{
		printf("\nThere are no program headers in this file.\n");
		return (1);
	}
	printf("\nElf file type is "), type_(elfen->fh64.e_type);
	printf("Entry point 0x%lx\n", elfen->fh64.e_entry);
	printf("There are %d program headers, ", elfen->fh64.e_phnum);
	printf("starting at offset %lu\n", elfen->fh64.e_phoff);
	if (!read_sh_64(elfen))
		error_manager(0, 1);
	if (!read_ph_64(elfen))
		error_manager(0, 1);
	if (prog.endianness == MSB)
	{
		convert_sh_endian_64(elfen->sh64, elfen->fh64.e_shnum);
		convert_ph_endian_64(elfen->ph64, elfen->fh64.e_phnum);
	}
	str_table = sh_str_table_64(elfen);
	printf("\nProgram Headers:\n");
	print_col_titles();
	for (iter = 0; iter != elfen->fh64.e_phnum; iter++)
	{
		if (elfen->ph64[iter].p_type == PT_NULL)
			continue;
		printf("  %-14s ", type_segment(elfen->ph64[iter].p_type));
		printf("0x%6.6lx ", elfen->ph64[iter].p_offset);
		printf("0x%16.16lx ", elfen->ph64[iter].p_vaddr);
		printf("0x%16.16lx ", elfen->ph64[iter].p_paddr);
		printf("0x%6.6lx ", elfen->ph64[iter].p_filesz);
		printf("0x%6.6lx ", elfen->ph64[iter].p_memsz);
		printf("%c", elfen->ph64[iter].p_flags & PF_R ? 'R' : ' ');
		printf("%c", elfen->ph64[iter].p_flags & PF_W ? 'W' : ' ');
		printf("%c ", elfen->ph64[iter].p_flags & PF_X ? 'E' : ' ');
		printf("%#lx\n", elfen->ph64[iter].p_align);
		if (elfen->ph64[iter].p_type == PT_INTERP)
			interp(elfen, iter);
	}
	section_segment_map_64(elfen, str_table), free(str_table);
	return (0);
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
 * read_ph_32 - populates program header data for 32-bit ELF file
 * @elfen: ELF file data struct
 * Return: 1 upon success, 0 upon 0 phnum, mem allocation error, or read error
*/
static int read_ph_32(elf_dt *elfen)
{
	size_t phnum = elfen->fh32.e_phnum, rd = 0;
	size_t read_siz = elfen->fh32.e_phentsize * elfen->fh32.e_phnum;
	char *ph_map = NULL;

	if (!phnum)
		return (0);
	ph_map = calloc(phnum, read_siz * phnum);
	if (!ph_map)
		return (0);
	lseek(prog.fd, elfen->fh32.e_phoff, SEEK_SET);
	rd = read(prog.fd, ph_map, read_siz);
	if (rd < read_siz)
		return (0);
	elfen->ph32 = (void *)ph_map;
	return (1);
}

/**
 * read_ph_64 - populates program header data for 64-bit ELF file
 * @elfen: ELF file data struct
 * Return: 1 upon success, 0 upon 0 phnum, mem allocation error, or read error
*/
static int read_ph_64(elf_dt *elfen)
{
	size_t phnum = elfen->fh64.e_phnum, rd = 0;
	char *ph_map = NULL;
	size_t read_siz = elfen->fh64.e_phentsize * elfen->fh64.e_phnum;

	if (!phnum)
		return (0);
	ph_map = calloc(phnum, read_siz * phnum);
	if (!ph_map)
		return (0);
	lseek(prog.fd, elfen->fh64.e_phoff, SEEK_SET);
	rd = read(prog.fd, ph_map, read_siz);
	if (rd < read_siz)
		return (0);
	elfen->ph64 = (void *)ph_map;
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
 * type_segment - retrieves name of program segment type
 * @type: unsigned 32-bit integer representing program segment type
 * Return: name of program segment type
*/
static char *type_segment(uint32_t type)
{
	switch (type)
	{
		case PT_NULL:			return ("NULL");
		case PT_LOAD:			return ("LOAD");
		case PT_DYNAMIC:		return ("DYNAMIC");
		case PT_INTERP:			return ("INTERP");
		case PT_NOTE:			return ("NOTE");
		case PT_SHLIB:			return ("SHLIB");
		case PT_PHDR:			return ("PHDR");
		case PT_TLS:			return ("TLS");
		case PT_GNU_EH_FRAME:	return ("GNU_EH_FRAME");
		case PT_GNU_STACK:		return ("GNU_STACK");
		case PT_GNU_RELRO:		return ("GNU_RELRO");
		case 0x6464E550:		return ("LOOS+464e550");
		default:				return ("UNKNOWN");
	}
}

/**
 * print_col_titles - prints program column names
*/
static void print_col_titles(void)
{
	printf("  %-14s ", "Type");
	printf("%-8.8s ", "Offset");
	if (prog.fclass == ELFCLASS32)
	{
		printf("%-10.10s ", "VirtAddr");
		printf("%-10.10s ", "PhysAddr");
		printf("%-7.7s ", "FileSiz");
		printf("%-7.7s ", "MemSiz");
	}
	else if (prog.fclass == ELFCLASS64)
	{
		printf("%-18.18s ", "VirtAddr");
		printf("%-18.18s ", "PhysAddr");
		printf("%-8.8s ", "FileSiz");
		printf("%-8.8s ", "MemSiz");
	}
	printf("%c%c%c ", 'F', 'l', 'g');
	printf("%s\n", "Align");
}

/**
 * interp - retrieves program interpreter name if program segment is PT_INTERP
 * @elfen: ELF file data struct
 * @iter: index within program header
*/
static void interp(elf_dt *elfen, int iter)
{
	FILE *fd_stream;
	char name[32], path[4096];

	fd_stream = fdopen(prog.fd, "r");
	snprintf(name, sizeof(name), "%%%ds", 4096);
	if (prog.fclass == ELFCLASS32)
		lseek(prog.fd, elfen->ph32[iter].p_offset, SEEK_SET);
	else if (prog.fclass == ELFCLASS64)
		lseek(prog.fd, elfen->ph64[iter].p_offset, SEEK_SET);
	if (fscanf(fd_stream, name, path) > 0)
		printf("      "),
		printf("[Requesting program interpreter: %s]\n", path);
	fclose(fd_stream);
}

/**
 * section_segment_map_32 - prints section to segment mapping data (32-bit)
 * @elfen: ELF file data struct
 * @str_table: section header string table
*/
static void section_segment_map_32(elf_dt *elfen, char *str_table)
{
	int iter = 0, nest = 0;

	printf("\n Section to Segment mapping:\n");
	printf("  Segment Sections...\n");
	for (iter = 0; iter < elfen->fh32.e_phnum; iter++)
	{
		if (elfen->ph32[iter].p_type == PT_NULL)
			continue;
		printf("   %2.2u     ", iter);
		for (nest = 0; nest < elfen->fh32.e_shnum; nest++)
		{
			if (sect_in_pseg_32(elfen->sh32[nest], elfen->ph32[iter]))
				printf("%s ", str_table + elfen->sh32[nest].sh_name);
		}
		putchar('\n');
	}
}

/**
 * section_segment_map_64 - prints section to segment mapping data (64-bit)
 * @elfen: ELF file data struct
 * @str_table: section header string table
*/
static void section_segment_map_64(elf_dt *elfen, char *str_table)
{
	int iter = 0, nest = 0;

	printf("\n Section to Segment mapping:\n");
	printf("  Segment Sections...\n");
	for (iter = 0; iter < elfen->fh64.e_phnum; iter++)
	{
		if (elfen->ph64[iter].p_type == PT_NULL)
			continue;
		printf("   %2.2u     ", iter);
		for (nest = 0; nest < elfen->fh64.e_shnum; nest++)
		{
			if (sect_in_pseg_64(elfen->sh64[nest], elfen->ph64[iter]))
				printf("%s ", str_table + elfen->sh64[nest].sh_name);
		}
		putchar('\n');
	}
}

/**
 * sect_in_pseg_32 - checks if a section is within a program segment (32-bit)
 * @sect: section from section header
 * @pseg: program segment from program header
 * Return: 1 if section is within program segment, 0 otherwise
*/
static int sect_in_pseg_32(Elf32_Shdr sect, Elf32_Phdr pseg)
{
	int is_in = 0;

	if ((
		((((sect.sh_flags & SHF_TLS) != 0) &&
			(pseg.p_type == PT_TLS || pseg.p_type == PT_LOAD)) ||
			((sect.sh_flags & SHF_TLS) == 0 &&
				pseg.p_type != PT_TLS)) &&
		(sect.sh_type == SHT_NOBITS || (sect.sh_offset >= pseg.p_offset &&
			(sect.sh_offset + sect_siz_32(sect, pseg) <=
				pseg.p_offset + pseg.p_filesz))) &&
		((sect.sh_flags & SHF_ALLOC) == 0 ||
			(sect.sh_addr >= pseg.p_vaddr &&
			(sect.sh_addr + sect_siz_32(sect, pseg) <=
				pseg.p_vaddr + pseg.p_memsz)))) &&
		sect_siz_32(sect, pseg) > 0
	)
		is_in = 1;
	return (is_in);
}

/**
 * sect_in_pseg_64 - checks if a section is within a program segment (64-bit)
 * @sect: section from section header
 * @pseg: program segment from program header
 * Return: 1 if section is within program segment, 0 otherwise
*/
static int sect_in_pseg_64(Elf64_Shdr sect, Elf64_Phdr pseg)
{
	int is_in = 0;

	if ((
		((((sect.sh_flags & SHF_TLS) != 0) &&
			(pseg.p_type == PT_TLS || pseg.p_type == PT_LOAD)) ||
			((sect.sh_flags & SHF_TLS) == 0 &&
				pseg.p_type != PT_TLS)) &&
		(sect.sh_type == SHT_NOBITS || (sect.sh_offset >= pseg.p_offset &&
			(sect.sh_offset + sect_siz_64(sect, pseg) <=
				pseg.p_offset + pseg.p_filesz))) &&
		((sect.sh_flags & SHF_ALLOC) == 0 ||
			(sect.sh_addr >= pseg.p_vaddr &&
			(sect.sh_addr + sect_siz_64(sect, pseg) <=
				pseg.p_vaddr + pseg.p_memsz)))) &&
		sect_siz_64(sect, pseg) > 0
	)
		is_in = 1;
	return (is_in);
}

/**
 * sect_siz_32 - retrieves section size, as applicable (32-bit)
 * @sect: section from section header
 * @pseg: program segment from program header
 * Return: section size upon conditions met, 0 otherwise
*/
static unsigned long sect_siz_32(Elf32_Shdr sect, Elf32_Phdr pseg)
{
	return ((sect.sh_flags & SHF_TLS) == 0 ||
			sect.sh_type != SHT_NOBITS ||
			pseg.p_type == PT_TLS ?
			sect.sh_size : 0);
}

/**
 * sect_siz_64 - retrieves section size, as applicable (64-bit)
 * @sect: section from section header
 * @pseg: program segment from program header
 * Return: section size upon conditions met, 0 otherwise
*/
static unsigned long sect_siz_64(Elf64_Shdr sect, Elf64_Phdr pseg)
{
	return ((sect.sh_flags & SHF_TLS) == 0 ||
			sect.sh_type != SHT_NOBITS ||
			pseg.p_type == PT_TLS ?
			sect.sh_size : 0);
}
