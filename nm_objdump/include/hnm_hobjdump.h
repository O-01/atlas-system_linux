#ifndef _HNM_H_
#define _HNM_H_

#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LSB 1
#define MSB 2

#define ERR_STR ": Error: "
#define ERR_OPEN ": No such file"
#define ERR_ELF "Not an ELF file - it has the wrong magic bytes at the start"

/**
 * struct program_data - records program reference information
 * @name: name of program
 * @file: name of input file
 * @fd: file descriptor of input file, if opened
 * @fclass: file class of input file
 * @endianness: endianness of input file
*/
typedef struct program_data
{
	char *name;
	char *file;
	int fd;
	int fclass;
	int endianness;
} prog_dt;

/**
 * struct elf_data - contains ELF header data from input file in header(s)
 *                   corresponding to file class
 * @fh32: file header struct for 32-bit ELF files
 * @fh64: file header struct for 64-bit ELF files
 * @sh32: section header struct for 32-bit ELF files
 * @sh64: section header struct for 64-bit ELF files
 * @ph32: program header struct for 32-bit ELF files
 * @ph64: program header struct for 64-bit ELF files
 * @sym32: symbol table entry struct for 32-bit ELF files
 * @sym64: symbol table entry struct for 64-bit ELF files
*/
typedef struct elf_data
{
	Elf32_Ehdr fh32;
	Elf64_Ehdr fh64;
	Elf32_Shdr *sh32;
	Elf64_Shdr *sh64;
	Elf32_Phdr *ph32;
	Elf64_Phdr *ph64;
	Elf32_Sym *sym32;
	Elf64_Sym *sym64;
} elf_dt;

extern prog_dt prog;

int sym_details_32(elf_dt *elfen);
int sym_details_64(elf_dt *elfen);

/* common.c */
int is_elf(char *fd_map);
void error_manager(int cause, int err);
void convert_fh_endian_32(Elf32_Ehdr *map_32);
void convert_fh_endian_64(Elf64_Ehdr *map_64);
void convert_sh_endian_32(Elf32_Shdr *sect_32, uint16_t shnum);
void convert_sh_endian_64(Elf64_Shdr *sect_64, uint16_t shnum);
void convert_ph_endian_32(Elf32_Phdr *ph_32, uint16_t phnum);
void convert_ph_endian_64(Elf64_Phdr *ph_64, uint16_t phnum);
void convert_sym_endian_32(Elf32_Sym *sym_32, size_t size);
void convert_sym_endian_64(Elf64_Sym *sym_64, size_t size);

#endif
