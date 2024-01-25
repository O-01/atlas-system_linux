#include "hls.h"

static void arg_munch(char **argv, c_dt *cmd);
static void flag_setter(char *flag_string, int *flags);
static int list_packer(char *dir_name, dir_l **list);
static void print_content(char *filename, char *delim, int flags);

/**
 * main - program entry
 * @argc: argument count to program
 * @argv: vector of input arguments
 * Return: 0 upon success
*/

int main(const int argc, char **argv)
{
	c_dt cmd;
	dir_l *tmp, *current;
	char *delim = "  ";
	int printed = 0;

	cmd.arg_c = argc;
	cmd.dir_count = 0;
	cmd.dir_list = NULL;
	cmd.flags = 0x00;
	if (argc > 1)
	{
		arg_munch(argv, &cmd);
		if (cmd.flags & (1 << 0) || cmd.flags & (1 << 1))
			delim = "\n";
	}
	if (cmd.dir_count == 1)
		print_content(cmd.dir_list->name, delim, cmd.flags);
	else if (cmd.dir_count > 1 || cmd.flags & (1 << 7))
		for (tmp = cmd.dir_list; tmp; tmp = tmp->next)
		{
			if (printed)
				printf("\n\n");
			print_content(tmp->name, delim, cmd.flags);
			printed = 1;
		}
	else
		print_content(".", delim, cmd.flags);
	/* temporary */
	for (tmp = cmd.dir_list; tmp;)
	{
		current = tmp;
		tmp = tmp->next;
		free(current->name), current->name = NULL;
		free(current), current = NULL;
	}
	/* temporary */
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
			list_packer(argv[iter], &cmd->dir_list), cmd->dir_count++;
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
 * @dir_name: name of directory to be added to list
 * @list: command data dir list to which directory to be added
 * Return: -1 upon memory allocation failure, 0 otherwise
*/

static int list_packer(char *dir_name, dir_l **list)
{
	dir_l *tmp = NULL;

	if (!*list)
	{
		tmp = malloc(sizeof(dir_l));
		if (!tmp)
			return (-1);
		tmp->name = malloc(sizeof(char *) + 1);
		if (!tmp->name)
			return (-1);
		_strcpy(tmp->name, dir_name);
		tmp->next = NULL;
		tmp->prev = NULL;
		*list = tmp;
	}
	return (0);
}

/**
 * print_content - print directory contents
 * @filename: name of directory whose contents are to be printed
 * @delim: delimiter to be placed between content entries
 * @flags: flags that determine what should be printed
*/

static void print_content(char *filename, char *delim, int flags)
{
	DIR *open_up = NULL;
	struct dirent *read_d = NULL;
	char *name = NULL;
	int printed = 0;

	open_up = opendir(filename);
	if (!open_up)
	{
		perror("WIP");
		return;
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
}
