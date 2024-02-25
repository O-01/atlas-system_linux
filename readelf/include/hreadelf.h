#ifndef _HREADELF_H_
#define _HREADELF_H_

#include <elf.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

/* common.c */
int is_elf(char *fd_map);
void error_manager(int fd, char *cause, int err);

/** 0-hreadelf **/
/* file_header.c */
int file_header(char *fd_map);
/* fh_32.c */
void fh_details_32(Elf32_Ehdr *map_32);
/* fh_64.c */
void fh_details_64(Elf64_Ehdr *map_64);

/** 1-hreadelf **/

#endif