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
	{
		if (!inventory)
			return (NULL);
		else if (inventory->next)
			for (; inventory; inventory = file)
			{
				file = inventory->next;
				if (inventory->buffer)
					free(inventory->buffer), inventory->buffer = NULL;
				free(inventory);
			}
		free(inventory), inventory = NULL;
		return (NULL);
	}
	file = summon(&inventory, fd);
	if (!file)
		return (NULL);
	line = next_line(file);
	if (!line)
		free(file->buffer), file->buffer = NULL;
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

	add = malloc(sizeof(_file_inv));
	if (!add)
		return (NULL);
	add->fd = fd;
	add->buffer_size = 0;
	add->marker = 0;
	add->solo = 0;
	if (!(*inv))
		*inv = add;
	else
		for (; file; file = file->next)
			if (!file->next)
			{
				file->next = add;
				break;
			}
	add->next = NULL;
	add->buffer = read_loop(fd);
	if (!(add->buffer))
	{
		free(add);
		return (NULL);
	}
	for (
		;
		add->buffer[add->buffer_size] != '\0' &&
		add->buffer[add->buffer_size + 1 != '\0'];
		add->buffer_size++
	)
		;
	return (add);
}

/**
 * read_loop - read fd to collect all readable bytes from file into string
 * @fd: input file descriptor to be read
 * Return: null-terminated string containing readable bytes from file,
 *         NULL upon failure (unreadable fd or memory allocation error)
*/

char *read_loop(int fd)
{
	char *buffer = NULL, *out = NULL, *tmp = NULL;
	ssize_t bytes_read = 0, size = 0;

	buffer = malloc(READ_SIZE);
	if (!buffer)
		return (NULL);
	out = malloc(READ_SIZE);
	if (!out)
		return (NULL);
	while ((bytes_read = read(fd, buffer, READ_SIZE)))
	{
		if (bytes_read == -1)
		{
			free(out), out = NULL;
			break;
		}
		tmp = realloc(out, size + bytes_read + 1);
		if (!tmp)
		{
			free(out), out = NULL;
			break;
		}
		out = tmp;
		memcpy(out + size, buffer, bytes_read);
		size += bytes_read;
		out[size] = '\0';
	}
	free(buffer);
	return (out);
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

	if ((mark < end || (end == 0 && txt[mark] == '\n')) && !file->solo)
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
			file->marker++, span++
		)
			line[span] = txt[file->marker];
		if (txt[file->marker] == '\0' || txt[file->marker] == '\n')
			file->marker++;
		line[span] = '\0';
		if (!end && !file->solo)
			file->solo = 1;
	}
	else
		return (NULL);
	return (line);
}
