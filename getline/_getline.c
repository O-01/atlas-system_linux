#include "_getline.h"

/**
 * _getline - reads from file descriptor, line-by-line per call until end
 * @fd: input file descriptor, next line of content requested
 * Return: next line of file stream as null-terminated string upon success,
 *         NULL otherwise
*/

char *_getline(const int fd)
{
	static _file_inv *inventory;
	_file_inv *file = NULL;
	char *line = NULL;

	if (fd == -1)
		return (NULL); /* for now, part of task 2 */
	file = summon(&inventory, fd);
	if (!file)
		return (NULL);
	line = next_line(file);
	if (!line)
		free(file->buffer), file->buffer = NULL, free(file);
	return (line);
}

/**
 * summon - queries file list & returns node matching file descriptor
 * @inv: pointer to first node of file list, if applicable
 * @fd: input file descriptor by which inv is queried
 * Return: pointer to file in inventory referenced by fd
*/

_file_inv *summon(_file_inv **inv, int fd)
{
	_file_inv *tmp = *inv;
	size_t found = 0;

	for (; tmp; tmp = tmp->next)
		if (tmp->fd == fd)
		{
			found = 1;
			return (tmp);
		}
	if (!found)
		tmp = add_file(inv, fd);
	return (tmp);
}

/**
 * add_file - add file content & rel attributes as node to inv linked list
 * @inv: pointer to first node of file list, if applicable
 * @fd: input file descriptor supplied as identifier for added node
 * Return: pointer to added node upon success, NULL otherwise
*/

_file_inv *add_file(_file_inv **inv, int fd)
{
	_file_inv *add = NULL, *file = *inv;
	char *file_content = NULL;

	add = malloc(sizeof(_file_inv));
	if (!add)
		return (NULL);
	add->fd = fd;
	add->marker = 0;
	add->next = NULL;
	file_content = malloc(READ_SIZE);
	if (!file_content)
		return (NULL);
	add->buffer_size = read(fd, file_content, READ_SIZE);
	if (add->buffer_size == -1)
	{
		free(file_content), free(add);
		return (NULL);
	}
	add->buffer = _realloc(file_content, add->buffer_size);
	if (!add->buffer)
		return (NULL);
	if (!(*inv))
		*inv = add;
	else
		for (; file; file = file->next)
			if (!file->next)
				file->next = add;
	return (add);
}

/**
 * next_line - pull next line from buffer of node related to input file
 * @file: input file contents from which to extract next line
 * Return: next line from buffer of input file stream as null-terminated string
 *         upon success, NULL otherwise
*/

char *next_line(_file_inv *file)
{
	size_t mark = file->marker, span = 0, end = file->buffer_size;
	char *txt = file->buffer, *line = NULL;

	if (mark < end)
	{
		for (
			;
			mark != end &&
			txt[mark] != '\n' &&
			txt[mark] != '\0';
			mark++, span++
		)
			;
		line = malloc(sizeof(char) * span + 1);
		if (!line)
			return (NULL);
		for (
			span = 0;
			file->marker < mark &&
			txt[file->marker] != '\n' &&
			txt[file->marker] != '\0';
			++file->marker, ++span
		)
			line[span] = txt[file->marker];
		line[span] = '\0';
		if (txt[file->marker] == '\0' || txt[file->marker] == '\n')
			file->marker++;
/*
*		if (file->marker == end)
*			free(txt), txt = NULL;
*/
	}
	return (line);
}

/**
 * _realloc - reallocate memory segment
 * @input: allocated memory segment to be adjusted
 * @desired_size: size to which input to be reallocated
 * Return: pointer to reallocated memory upon success
*/

void *_realloc(void *input, ssize_t desired_size)
{
	void *update = NULL;

	if (input && !desired_size)
	{
		free(input);
		return (NULL);
	}
	else
	{
		update = malloc(sizeof(char) * desired_size);
		if (!update)
		{
			if (input)
				free(input);
			return (NULL);
		}
		memcpy(update, input, desired_size);
		free(input);
		return (update);
	}
}
