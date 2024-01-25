#ifndef _HLS_H_
#define _HLS_H_

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

/**
 * struct dir_list - list of directories related to command data
 * @name: name of directory
 * @next: next directory entry in list
 * @prev: previous directory entry in list
*/

typedef struct dir_list
{
	char *name;
	struct dir_list *next;
	struct dir_list *prev;
} dir_l;

/**
 * struct command_data - structure to retain data from command input
 * @arg_c: count of arguments minus the first argument (executable name)
 * @dir_count: count of directories of which to find contents
 * @dir_list: list of directories of which to find contents
 * @flags: hex value corresponding to bits set based on flags requested
 *         0x01 = 00000001 : -1
 *         0x02 = 00000010 : -l
 *         0x04 = 00000100 : -a
 *         0x08 = 00001000 : -A
 *         0x10 = 00010000 : -r
 *         0x20 = 00100000 : -S
 *         0x40 = 01000000 : -t
 *         0x80 = 10000000 : -R
 *         ex. 0x8A = 10001010 : -laR
*/

typedef struct command_data
{
	int arg_c;
	int dir_count;
	dir_l *dir_list;
	int flags;
} c_dt;

char *_strcpy(char *dest, char *src);
int _strcmp(char *str1, char *str2);
int _strlen(char *str);

#endif
