#ifndef _GETLINE_H_
#define _GETLINE_H_

#define READ_SIZE (1024 * 1024 * 4)
#define STOP ('\n' && '\0')

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

/**
 * struct _file_inventory - node template designed to handle contents related
 *                          to an input file descriptor as linked list
 * @fd: file descriptor as identifier
 * @buffer: buffer to contain applicable text content from file stream
 * @buffer_size: bytes read from file stream
 * @marker: keeps track of cursor position within buffer
 * @next: pointer to next node if present, else NULL
 */

typedef struct _file_inventory
{
	int fd;
	char *buffer;
	ssize_t buffer_size;
	size_t marker;
	struct _file_inventory *next;
} _file_inv;

char *_getline(const int fd);
_file_inv *summon(_file_inv **inv, int id);
_file_inv *add_file(_file_inv **inv, int fd);
char *next_line(_file_inv *file);
void *_realloc(void *mem, ssize_t desired_size);

#endif
