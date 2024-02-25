#ifndef _HREADELF_H_
#define _HREADELF_H_

#include <elf.h>
#include <endian.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * struct program_data - records program reference information
 * @name: name of program
 * @fd: file descriptor of input file, if opened
 * @endianness: endianness of input file
*/

typedef struct program_data
{
	char *name;
	int fd;
	int endianness;
} prog_dt;

extern prog_dt prog;

/* common.c */
int is_elf(char *fd_map);
void error_manager(char *cause, int err);
void data_(char *fd_map);

/** 0-hreadelf **/
/* file_header.c */
int file_header(char *fd_map);
/* fh_32.c */
void fh_details_32(Elf32_Ehdr *map_32);
/* fh_64.c */
void fh_details_64(Elf64_Ehdr *map_64);

/** 1-hreadelf **/

#endif