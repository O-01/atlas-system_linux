#include "hls.h"

static void arg_munch(char **argv, c_dt *cmd);
static void flag_setter(char *flag_string, int *flags);
static int list_packer(char *filename, file_l **list);
static int printer(char *file_, char *delim, int flags, int o_p, int o_);
static void free_cdt(file_l **list);
static void error_dump(char *exec, char *filename);

/**
 * main - program entry
 * @argc: argument count to program
 * @argv: vector of input arguments
 * Return: 0 upon success
*/

int main(const int argc, char **argv)
{
	c_dt cmd;
	file_l *tmp;
	char *delim = "  ";
	int printed = 0, o_ = 0, p_ = 0;

	cmd.arg_c = argc, cmd.file_count = 0, cmd.file_list = NULL;
	cmd.flags = 0x00;
	if (argc > 1)
	{
		arg_munch(argv, &cmd);
		if (cmd.flags & (1 << 0) || cmd.flags & (1 << 1))
			delim = "\n";
	}
	if (cmd.file_count == 1)
	{
		p_ = printer(cmd.file_list->name, delim, cmd.flags, printed, o_);
		if (p_ == 2)
			error_dump(argv[0], cmd.file_list->name);
	}
	else if (cmd.file_count > 1 || cmd.flags & (1 << 7))
		for (tmp = cmd.file_list, o_ = 1; tmp; tmp = tmp->next)
		{
			p_ = printer(tmp->name, delim, cmd.flags, printed, o_);
			if (p_ == 2)
				error_dump(argv[0], tmp->name);
			if (!p_)
				printed = 1;
		}
	else
		printer(".", delim, cmd.flags, printed, o_);
	free_cdt(&cmd.file_list);
	if (p_ == 0)
		printf("\n");
	return (0);
}

/**
 * arg_munch - process arguments by flags &/or directory names
 * @argv: argument vector, passed from main for processing
 * @cmd: command data struct
*/

static void arg_munch(char **argv, c_dt *cmd)
{
	int iter = 1;

	for (; iter != cmd->arg_c; iter++)
	{
		if (argv[iter][0] == '-')
		{
			if (argv[iter][1] != '-')
				flag_setter(argv[iter], &cmd->flags);
			else if (argv[iter][0] == '-' && argv[iter][1] == '-')
			{
				if (!argv[iter][2])
					continue;
				else if (argv[iter][2] == '-')
					/* possible error message for unknown parameter --- */
					continue;
			}
		}
		else
			list_packer(argv[iter], &cmd->file_list), cmd->file_count++;
	}
}

/**
 * flag_setter - adjust command data flags value based on requested flags
 * @flag_string: string beginning with '-' that contains possible flags
 * @flags: command data flags value to be updated if necessary
*/

static void flag_setter(char *flag_string, int *flags)
{
	unsigned int iter = 1;

	for (; flag_string[iter]; iter++)
		switch (flag_string[iter])
		{
			case '1':
				*flags |= (1 << 0);
				continue;
			case 'l':
				*flags |= (1 << 1);
				continue;
			case 'a':
				*flags |= (1 << 2);
				continue;
			case 'A':
				*flags |= (1 << 3);
				continue;
			case 'r':
				*flags |= (1 << 4);
				continue;
			case 'S':
				*flags |= (1 << 5);
				continue;
			case 't':
				*flags |= (1 << 6);
				continue;
			case 'R':
				*flags |= (1 << 7);
				continue;
			default:
				/* possible error message for unknown flag(s)? */
				continue;
		}
}

/**
 * list_packer - populate command data dir list
 *               (for use with -R or multiple directory input)
 * @filename: name of directory to be added to list
 * @list: command data dir list to which directory to be added
 * Return: -1 upon memory allocation failure, 0 otherwise
*/

static int list_packer(char *filename, file_l **list)
{
	file_l *add = NULL, *tmp = NULL;
	int diff = 0;

	add = malloc(sizeof(file_l));
	if (!add)
		return (-1);
	add->name = malloc(sizeof(char) * _strlen(filename) + 1);
	if (!add->name)
		return (-1);
	_strcpy(add->name, filename);
	if (!*list)
	{
		add->next = NULL;
		add->prev = NULL;
		*list = add;
	}
	else
	{
		diff = _strcmp(add->name, (*list)->name);
		if (diff < 0 && diff != -32)
		{
			add->prev = NULL;
			add->next = *list;
			(*list)->prev = add;
			*list = add;
		}
		else
		{
			for (tmp = *list; tmp; tmp = tmp->next)
			{
				diff = _strcmp(add->name, tmp->name);
				if (tmp->next && _strcmp(add->name, tmp->next->name) < 0)
					continue;
				add->next = tmp->next;
				add->prev = tmp;
				if (tmp->next)
					tmp->next->prev = add;
				tmp->next = add;
				break;
			}
		}
	}
	return (0);
}

/**
 * printer - print directory contents
 * @file_: name of directory whose contents are to be printed
 * @delim: delimiter to be placed between content entries
 * @flags: flags that determine what should be printed
 * @o_p: flag indicates loop has already printed, if applicable
 * @o_: flag indicates loop usage
 * Return: 2 upon file/directory not found, 0 otherwise
*/

static int printer(char *file_, char *delim, int flags, int o_p, int o_)
{
	DIR *open_up = NULL;
	struct dirent *read_d = NULL;
	char *name = NULL;
	int printed = 0;

	open_up = opendir(file_);
	if (!open_up)
		return (2);
	if (o_)
	{
		if (o_p)
			printf("\n\n");
		printf("%s:\n", file_);
	}
	for (printed = 0; (read_d = readdir(open_up));)
	{
		name = read_d->d_name;
		if (name[0] == '.')
		{
			if (
				(!name[1] || (name[1] == '.' && !name[2])) &&
				!(flags & (1 << 2))
			)
				continue;
			else if (
				(name[1] || (name[1] == '.' && name[2])) &&
				!((flags & (1 << 2)) | (flags & (1 << 3)))
			)
				continue;
		}
		if (printed)
			printf("%s", delim);
		/* printf("inode no. - %ld\n", read_d->d_ino);*/
		/* printf("offset to next dirent - %ld\n", read_d->d_off);*/
		/* printf("length of this record - %d\n", read_d->d_reclen);*/
		/* printf("file type - %c|\n", read_d->d_type);*/
		printf("%s", name);
		printed = 1;
	}
	closedir(open_up);
	return (0);
}

/**
 * free_cdt - free all memory related to command data
 * @list: pointer to beginning of allocated memory
*/

static void free_cdt(file_l **list)
{
	file_l *tmp = NULL, *current = NULL;

	for (tmp = *list; tmp;)
	{
		current = tmp;
		tmp = tmp->next;
		free(current->name), current->name = NULL;
		free(current), current = NULL;
	}
}

/**
 * error_dump - handles errors
 * @exec: name of program executable
 * @filename: name of file that caused error
*/

static void error_dump(char *exec, char *filename)
{
	fprintf(
		stderr,
		"%s: cannot access %s: No such file or directory\n",
		exec, filename
	);
}