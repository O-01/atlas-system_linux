#include "hnm_hobjdump.h"

static uint16_t bswap16(uint16_t num);
static uint32_t bswap32(uint32_t num);
static uint64_t bswap64(uint64_t num);

/**
 * is_elf - verifies if input file is ELF based on magic number
 * @fd_map: mmap of input file
 * Return: 1 if magic number found (ELF), 0 otherwise (not ELF)
*/
int is_elf(char *fd_map) { return (!memcmp(fd_map, ELFMAG, 4)); }

/**
 * error_manager - prints error and/or closes file descriptor, as applicable,
 *                 then exits
 * @cause: number representing cause:
 *         0: N/A or not implemented
 *         1: open failure
 *         2: file is not a regular file
 *         3: file is not ELF
 * @err: errno supplied for exit status
*/
void error_manager(int cause, int err)
{
	if (prog.fd && prog.fd != -1)
		close(prog.fd);
	switch (cause)
	{
		case 0:
			break;
		case 1:
			fprintf(stderr, "%s%s'%s'%s\n",
				prog.name, ERR_STR, prog.file, ERR_OPEN);
		break;
		case 2:
			fprintf(stderr, "%s%s%s\n", prog.name, ERR_STR, ERR_ELF);
		break;
		default:
			break;
	}
	exit(err);
}

/**
 * convert_fh_endian_32 - converts 32-bit ELF file header info from little
 *                        endian to big endian or vice-versa
 * @map_32: Elf32_Ehdr info struct for 32-bit input file
*/
void convert_fh_endian_32(Elf32_Ehdr *map_32)
{
	map_32->e_type = bswap16(map_32->e_type);
	map_32->e_machine = bswap16(map_32->e_machine);
	map_32->e_version = bswap32(map_32->e_version);
	map_32->e_entry = bswap32(map_32->e_entry);
	map_32->e_phoff = bswap32(map_32->e_phoff);
	map_32->e_shoff = bswap32(map_32->e_shoff);
	map_32->e_flags = bswap32(map_32->e_flags);
	map_32->e_ehsize = bswap16(map_32->e_ehsize);
	map_32->e_phentsize = bswap16(map_32->e_phentsize);
	map_32->e_phnum = bswap16(map_32->e_phnum);
	map_32->e_shentsize = bswap16(map_32->e_shentsize);
	map_32->e_shnum = bswap16(map_32->e_shnum);
	map_32->e_shstrndx = bswap16(map_32->e_shstrndx);
}

/**
 * convert_fh_endian_64 - converts 64-bit ELF file header info from little
 *                        endian to big endian or vice-versa
 * @map_64: Elf64_Ehdr info struct for 64-bit input file
*/
void convert_fh_endian_64(Elf64_Ehdr *map_64)
{
	map_64->e_type = bswap16(map_64->e_type);
	map_64->e_machine = bswap16(map_64->e_machine);
	map_64->e_version = bswap32(map_64->e_version);
	map_64->e_entry = bswap64(map_64->e_entry);
	map_64->e_phoff = bswap64(map_64->e_phoff);
	map_64->e_shoff = bswap64(map_64->e_shoff);
	map_64->e_flags = bswap32(map_64->e_flags);
	map_64->e_ehsize = bswap16(map_64->e_ehsize);
	map_64->e_phentsize = bswap16(map_64->e_phentsize);
	map_64->e_phnum = bswap16(map_64->e_phnum);
	map_64->e_shentsize = bswap16(map_64->e_shentsize);
	map_64->e_shnum = bswap16(map_64->e_shnum);
	map_64->e_shstrndx = bswap16(map_64->e_shstrndx);
}

/**
 * convert_sh_endian_32 - converts 32-bit ELF section header info from little
 *                        endian to big endian or vice-versa
 * @sh_32: Elf32_Shdr info struct for 32-bit input file
 * @shnum: number of entries in section header
*/
void convert_sh_endian_32(Elf32_Shdr *sh_32, uint16_t shnum)
{
	uint16_t iter = 0;

	for (; iter != shnum; iter++)
	{
		sh_32[iter].sh_name = bswap32(sh_32[iter].sh_name);
		sh_32[iter].sh_type = bswap32(sh_32[iter].sh_type);
		sh_32[iter].sh_flags = bswap32(sh_32[iter].sh_flags);
		sh_32[iter].sh_addr = bswap32(sh_32[iter].sh_addr);
		sh_32[iter].sh_offset = bswap32(sh_32[iter].sh_offset);
		sh_32[iter].sh_size = bswap32(sh_32[iter].sh_size);
		sh_32[iter].sh_link = bswap32(sh_32[iter].sh_link);
		sh_32[iter].sh_info = bswap32(sh_32[iter].sh_info);
		sh_32[iter].sh_addralign = bswap32(sh_32[iter].sh_addralign);
		sh_32[iter].sh_entsize = bswap32(sh_32[iter].sh_entsize);
	}
}

/**
 * convert_sh_endian_64 - converts 64-bit ELF section header info from little
 *                        endian to big endian or vice-versa
 * @sh_64: Elf64_Shdr info struct for 64-bit input file
 * @shnum: number of entries in section header
*/
void convert_sh_endian_64(Elf64_Shdr *sh_64, uint16_t shnum)
{
	uint16_t iter = 0;

	for (; iter != shnum; iter++)
	{
		sh_64[iter].sh_name = bswap32(sh_64[iter].sh_name);
		sh_64[iter].sh_type = bswap32(sh_64[iter].sh_type);
		sh_64[iter].sh_flags = bswap64(sh_64[iter].sh_flags);
		sh_64[iter].sh_addr = bswap64(sh_64[iter].sh_addr);
		sh_64[iter].sh_offset = bswap64(sh_64[iter].sh_offset);
		sh_64[iter].sh_size = bswap64(sh_64[iter].sh_size);
		sh_64[iter].sh_link = bswap32(sh_64[iter].sh_link);
		sh_64[iter].sh_info = bswap32(sh_64[iter].sh_info);
		sh_64[iter].sh_addralign = bswap64(sh_64[iter].sh_addralign);
		sh_64[iter].sh_entsize = bswap64(sh_64[iter].sh_entsize);
	}
}

/**
 * convert_ph_endian_32 - converts 32-bit ELF program header info from little
 *                        endian to big endian or vice-versa
 * @ph_32: Elf32_Shdr info struct for 32-bit input file
 * @phnum: number of entries in program header
*/
void convert_ph_endian_32(Elf32_Phdr *ph_32, uint16_t phnum)
{
	uint16_t iter = 0;

	for (; iter != phnum; iter++)
	{
		ph_32[iter].p_flags = bswap32(ph_32[iter].p_flags);
		ph_32[iter].p_type = bswap32(ph_32[iter].p_type);
		ph_32[iter].p_offset = bswap32(ph_32[iter].p_offset);
		ph_32[iter].p_vaddr = bswap32(ph_32[iter].p_vaddr);
		ph_32[iter].p_paddr = bswap32(ph_32[iter].p_paddr);
		ph_32[iter].p_filesz = bswap32(ph_32[iter].p_filesz);
		ph_32[iter].p_memsz = bswap32(ph_32[iter].p_memsz);
		ph_32[iter].p_align = bswap32(ph_32[iter].p_align);
	}
}

/**
 * convert_ph_endian_64 - converts 64-bit ELF program header info from little
 *                        endian to big endian or vice-versa
 * @ph_64: Elf64_Shdr info struct for 64-bit input file
 * @phnum: number of entries in program header
*/
void convert_ph_endian_64(Elf64_Phdr *ph_64, uint16_t phnum)
{
	uint16_t iter = 0;

	for (; iter != phnum; iter++)
	{
		ph_64[iter].p_flags = bswap32(ph_64[iter].p_flags);
		ph_64[iter].p_type = bswap32(ph_64[iter].p_type);
		ph_64[iter].p_offset = bswap32(ph_64[iter].p_offset);
		ph_64[iter].p_vaddr = bswap32(ph_64[iter].p_vaddr);
		ph_64[iter].p_paddr = bswap32(ph_64[iter].p_paddr);
		ph_64[iter].p_filesz = bswap32(ph_64[iter].p_filesz);
		ph_64[iter].p_memsz = bswap32(ph_64[iter].p_memsz);
		ph_64[iter].p_align = bswap32(ph_64[iter].p_align);
	}
}

/**
 * bswap16 - converts endianness of unsigned 16-bit integer
 * @num: number to be converted
 * Return: converted number
*/
static uint16_t bswap16(uint16_t num)
{
	return (((num >> 8) & 0xff) |
			((num << 8) & 0xff00));
}

/**
 * bswap32 - converts endianness of unsigned 32-bit integer
 * @num: number to be converted
 * Return: converted number
*/
static uint32_t bswap32(uint32_t num)
{
	return (((num >> 24) & 0xff) |
			((num >> 8)  & 0xff00) |
			((num << 8)  & 0xff0000) |
			((num << 24) & 0xff000000));
}

/**
 * bswap64 - converts endianness of unsigned 64-bit integer
 * @num: number to be converted
 * Return: converted number
*/
static uint64_t bswap64(uint64_t num)
{
	return (((num >> 56) & 0xff) |
			((num >> 40) & 0xff00) |
			((num >> 24) & 0xff0000) |
			((num >> 8)  & 0xff000000) |
			((num << 8)  & 0xff00000000) |
			((num << 24) & 0xff0000000000) |
			((num << 40) & 0xff000000000000) |
			((num << 56) & 0xff00000000000000));
}
