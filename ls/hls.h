#ifndef _HLS_H_
#define _HLS_H_

#include <dirent.h>
#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

/**
 * struct file_list - list of directories related to command data
 * @name: name of directory
 * @next: next directory entry in list
 * @prev: previous directory entry in list
*/

typedef struct file_list
{
	char *name;
	/* int is_dir; */
	/* mode_t mode; */
	/* char *mode_decode; */
	/* nlink_t links; */
	/* uid_t owner_id; */
	/* gid_t owner_gp; */
	/* off_t size; */
	/* time_t mtime; */
	struct file_list *next;
	struct file_list *prev;
} file_l;

/**
 * struct command_data - structure to retain data from command input
 * @arg_c: count of arguments minus the first argument (executable name)
 * @file_count: count of directories of which to find contents
 * @file_list: list of directories of which to find contents
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
	int file_count;
	file_l *file_list;
	int flags;
} c_dt;

int printer(file_l *list, int flags, int o_p, int o_);

void error_dump(char *exec, char *filename, int code);

char *_strcpy(char *dest, char *src);
int _strcmp(char *str1, char *str2);
int _strlen(char *str);
char *_strdup(char *str);
char *_strcat(char *dest, char *src);

#endif
