#ifndef _HREADELF_H_
#define _HREADELF_H_

#include <elf.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define LSB 1
#define MSB 2

#define ERR_STR ": Error: "
#define ERR_OPEN ": No such file"
#define ERR_ELF "Not an ELF file - it has the wrong magic bytes at the start"

#define FH_MAGIC "  Magic:   "
#define FH_CLASS "Class:"
#define FH_DATA "Data:"
#define FH_VERSION "Version:"
#define FH_OSABI "OS/ABI:"
#define FH_ABIVERSION "ABI Version:"
#define FH_TYPE "Type:"
#define FH_MACHINE "Machine:"
#define FH_EVERSION "Version:"
#define FH_ENTRY "Entry point address:"
#define FH_PHOFF "Start of program headers:"
#define FH_SHOFF "Start of section headers:"
#define FH_FLAGS "Flags:"
#define FH_EHSIZE "Size of this header:"
#define FH_PHENTSIZ "Size of program headers:"
#define FH_PHNUM "Number of program headers:"
#define FH_SHENTSIZ "Size of section headers:"
#define FH_SHNUM "Number of section headers:"
#define FH_SHSTRNDX "Section header string table index:"

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
 * @elf32: file header struct for 32-bit ELF files
 * @elf64: file header struct for 64-bit ELF files
*/
typedef struct elf_data
{
	Elf32_Ehdr elf32;
	Elf64_Ehdr elf64;
} elf_dt;

extern prog_dt prog;

/** 0-hreadelf **/

/* common.c */
int is_elf(char *fd_map);
void error_manager(int cause, int err);
void convert_endian_32(Elf32_Ehdr *map_32);
void convert_endian_64(Elf64_Ehdr *map_64);

/* file_header.c */
void fh_details_32(Elf32_Ehdr *map_32);
void fh_details_64(Elf64_Ehdr *map_64);

/** 1-hreadelf **/

#endif
