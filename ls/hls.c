#include "hls.h"

static int arg_munch(char **argv, c_dt *cmd);
static int flag_setter(char *flag_string, int *flags);
static int logistics(char *argument, c_dt *cmd);
static int list_packer_d(char *filename, dir_l **list);
static int list_packer_f(char *filename, file_l **list);
static void free_cdt(dir_l **d_list, file_l **f_list);

/**
 * main - program entry
 * @argc: argument count to program
 * @argv: vector of input arguments
 * Return: 0 upon success
*/

int main(const int argc, char **argv)
{
	c_dt cmd;
	dir_l *tmp_d;
	file_l *tmp_f;
	int printed = 0, loop_flag = 0, print_error = 0;

	cmd.arg_c = argc, cmd.dir_count = 0, cmd.dir_list = NULL;
	cmd.file_count = 0, cmd.file_list = NULL, cmd.flags = 0x00;
	if (argc > 1)
		arg_munch(argv, &cmd);
	if (cmd.file_count == 1)
		printer_f(cmd.file_list, cmd.flags, printed, loop_flag), printed = 1;
	else if (cmd.file_count > 1)
	{
		for (tmp_f = cmd.file_list, loop_flag = 1; tmp_f; tmp_f = tmp_f->next)
			printer_f(tmp_f, cmd.flags, printed, loop_flag), printed = 1;
	}
	if (cmd.dir_count == 1)
	{
		if (printed == 1)
			loop_flag = 1;
		print_error = printer_d(cmd.dir_list, cmd.flags, printed, loop_flag);
		if (print_error)
			error_dump(argv[0], cmd.dir_list->name, print_error);
	}
	else if (cmd.dir_count > 1 || cmd.flags & (1 << 7))
	{
		if (printed == 1)
			printf("\n"), printed = 0;
		for (tmp_d = cmd.dir_list, loop_flag = 1; tmp_d; tmp_d = tmp_d->next)
		{
			print_error = printer_d(tmp_d, cmd.flags, printed, loop_flag);
			if (print_error)
				error_dump(argv[0], tmp_d->name, print_error);
			if (!print_error)
				printed = 1;
		}
	}
	else if (argc == 1)
		printer_d(NULL, cmd.flags, printed, loop_flag);
	free_cdt(&cmd.dir_list, &cmd.file_list);
	if (print_error == 0)
		printf("\n");
	return (0);
}

/**
 * arg_munch - process arguments by flags &/or directory names
 * @argv: argument vector, passed from main for processing
 * @cmd: command data struct
 * Return: -1 upon NULL input, 0 otherwise
*/

static int arg_munch(char **argv, c_dt *cmd)
{
	int iter = 1;

	if (!argv || !cmd)
		return (-1);
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
		{
			if (logistics(argv[iter], cmd) == -1)
				error_dump(argv[0], argv[iter], 2);
		}
	}
	return (0);
}

/**
 * flag_setter - adjust command data flags value based on requested flags
 * @flag_string: string beginning with '-' that contains possible flags
 * @flags: command data flags value to be updated if necessary
 * Return: -1 if flag_string is NULL, 0 otherwise
*/

static int flag_setter(char *flag_string, int *flags)
{
	unsigned int iter = 1;

	if (!flag_string)
		return (-1);
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
	return (0);
}

/**
 * logistics - fill
 * @argument: argv element
 * @cmd: command data struct
 * Return: -1 upon lstat failure, 0 otherwise
*/

static int logistics(char *argument, c_dt *cmd)
{
	struct stat file_stat;

	if (lstat(argument, &file_stat) != -1)
	{
		if (S_ISDIR(file_stat.st_mode))
			list_packer_d(argument, &cmd->dir_list), cmd->dir_count++;
		else
			list_packer_f(argument, &cmd->file_list), cmd->file_count++;
	}
	else
		return (-1);
	return (0);
}

/**
 * list_packer_d - populate command data dir list
 *               (for use with -R or multiple directory input)
 * @filename: name of directory to be added to list
 * @list: command data dir list to which directory to be added
 * Return: -1 upon memory allocation failure, 0 otherwise
*/

static int list_packer_d(char *filename, dir_l **list)
{
	dir_l *add = NULL, *tmp = NULL;
	int diff = 0;

	add = malloc(sizeof(dir_l));
	if (!add)
		return (-1);
	add->name = _strdup(filename);
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
 * list_packer_f - populate command data file list
 * @filename: name of file to be added to list
 * @list: command data file list to which file to be added
 * Return: -1 upon memory allocation failure, 0 otherwise
*/

static int list_packer_f(char *filename, file_l **list)
{
	file_l *add = NULL, *tmp = NULL;
	int diff = 0;

	add = malloc(sizeof(file_l));
	if (!add)
		return (-1);
	add->name = _strdup(filename);
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
 * free_cdt - free all memory related to command data
 * @d_list: pointer to beginning of allocated memory for dir list
 * @f_list: pointer to beginning of allocated memory for file list
*/

static void free_cdt(dir_l **d_list, file_l **f_list)
{
	dir_l *tmp_d = *d_list, *current_d = NULL;
	file_l *tmp_f = *f_list, *current_f = NULL;

	if (tmp_d)
		for (; tmp_d;)
		{
			current_d = tmp_d;
			tmp_d = tmp_d->next;
			if (current_d->name)
				free(current_d->name), current_d->name = NULL;
			free(current_d), current_d = NULL;
		}
	if (tmp_f)
		for (; tmp_f;)
		{
			current_f = tmp_f;
			tmp_f = tmp_f->next;
			if (current_f->name)
				free(current_f->name), current_f->name = NULL;
			free(current_f), current_f = NULL;
		}
}
