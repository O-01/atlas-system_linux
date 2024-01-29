#include "hls.h"

static int printer_d(dir_l *list, int flags, int o_p, int loop_flag);
static void printer_f(file_l *list, int flags, int o_p, int loop_flag);
static void printer_loop(DIR *open_up, char *dir, int flags);
static void l_print(char *file);
static void mode_decoder(mode_t mode, char **file_mode);
static char *path_prep(char *dir, char *filename);

/**
 * print_manager - tells things what they need to do
 * @argv: argument vector passed to program upon launch
 * @cmd: command data struct
 * @error_flag: flag indicating one or more arguments has raised error
*/

void print_manager(char **argv, c_dt cmd, int error_flag)
{
	dir_l *tmp_d;
	file_l *tmp_f;
	int printed = 0, loop_flag = error_flag, print_error = 0;

	if (cmd.file_count == 1)
		printer_f(cmd.file_list, cmd.flags, printed, loop_flag),
		printed = 1,
		loop_flag = 1;
	else if (cmd.file_count > 1)
		for (tmp_f = cmd.file_list, loop_flag = 1; tmp_f; tmp_f = tmp_f->next)
			printer_f(tmp_f, cmd.flags, printed, loop_flag), printed = 1;
	if (cmd.dir_count == 1)
	{
		print_error = printer_d(cmd.dir_list, cmd.flags, printed, loop_flag);
		if (print_error)
			error_dump(argv[0], cmd.dir_list->name, print_error);
		printed = 1;
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
	else if (!cmd.dir_count && !cmd.file_count && !error_flag)
		printer_d(NULL, cmd.flags, printed, loop_flag), printed = 1;
	if (printed && !print_error)
		printf("\n");
}

/**
 * printer_d - print directory contents
 * @list: directory of which contents are to be printed
 * @flags: flags that determine what should be printed
 * @o_p: flag indicates loop has already printed, if applicable
 * @loop_flag: flag indicates loop usage
 * Return: 2 upon file/directory not found, 13 permission denied, 0 otherwise
*/

static int printer_d(dir_l *list, int flags, int o_p, int loop_flag)
{
	DIR *open_up = NULL;
	char *dir_name = NULL;

	if (!list)
		dir_name = ".", open_up = opendir(dir_name);
	else
		dir_name = list->name, open_up = opendir(list->name);
	if (!open_up)
		return (errno);
	if (loop_flag != 0)
	{
		if (o_p)
			printf("\n\n");
		printf("%s:\n", list->name);
	}
	printer_loop(open_up, dir_name, flags);
	closedir(open_up);
	return (0);
}

/**
 * printer_f - print input files
 * @list: list of input files to print
 * @flags: flags that determine what should be printed
 * @o_p: flag indicates loop has already printed, if applicable
 * @loop_flag: flag indicates loop usage
*/

static void printer_f(file_l *list, int flags, int o_p, int loop_flag)
{
	char *delim = "  ";

	if (flags & (1 << 0) || flags & (1 << 1))
		delim = "\n";
	if (loop_flag != 0)
	{
		if (o_p)
			printf("%s", delim);
	}
	if (flags & (1 << 1))
		l_print(list->name);
	printf("%s", list->name);
}

/**
 * printer_loop - prints what is requested by command
 * @open_up: directory of which contents to be printed
 * @dir: opened directory name passed for processing as necessary
 * @flags: flags by which format of what is to be printed is determined
*/

static void printer_loop(DIR *open_up, char *dir, int flags)
{
	struct dirent *re_d = NULL;
	char *delim = "  ", *fuller_path = NULL;
	int printed = 0;

	if (flags & (1 << 0) || flags & (1 << 1))
		delim = "\n";
	for (printed = 0; (re_d = readdir(open_up));)
	{
		if (re_d->d_name[0] == '.')
		{
			if ((!re_d->d_name[1] || (re_d->d_name[1] == '.' &&
					!re_d->d_name[2])) &&
				!(flags & (1 << 2))
			)
				continue;
			else if ((re_d->d_name[1] || (re_d->d_name[1] == '.' &&
					re_d->d_name[2])) &&
				!((flags & (1 << 2)) | (flags & (1 << 3)))
			)
				continue;
		}
		if (printed)
			printf("%s", delim);
		if (flags & (1 << 1))
		{
			fuller_path = path_prep(dir, re_d->d_name);
			l_print(fuller_path);
			free(fuller_path), fuller_path = NULL;
		}
		printf("%s", re_d->d_name);
		printed = 1;
	}
}

/**
 * l_print - handles printing when -l flag supplied
 * @file: file at hand of which attributes are to be printed
*/

static void l_print(char *file)
{
	struct stat extract;
	struct passwd *uid_check;
	struct group *gid_check;
	char *decode = NULL;

	if (lstat(file, &extract) != -1)
	{
		decode = malloc(sizeof(char) * 12);
		if (!decode)
			return;
		mode_decoder(extract.st_mode, &decode);
		printf("%s ", decode);
		if (decode)
			free(decode), decode = NULL;
		printf("%ld ", extract.st_nlink);
		uid_check = getpwuid(extract.st_uid);
		if (uid_check)
			printf("%s ", uid_check->pw_name);
		else
			printf("%d ", extract.st_uid);
		gid_check = getgrgid(extract.st_gid);
		if (gid_check)
			printf("%s ", gid_check->gr_name);
		else
			printf("%d ", extract.st_gid);
		printf("%ld ", extract.st_size);
		/* printf("%*ld ", 5, extract.st_size); */
		/* printf("%.24s ", ctime(&extract.st_mtime)); */
		printf("%.12s ", &ctime(&extract.st_mtime)[4]);
	}
}

/**
 * mode_decoder - decodes input mode from octal into the 'rwx' letter groups
 *                for l_print
 * @mode: file permissions value to be decoded
 * @file_mode: string that serves as collector for file permissions string
*/

static void mode_decoder(mode_t mode, char **file_mode)
{
	char *tmp = *file_mode;
	int pos = 8, iter = 0;

	if S_ISDIR(mode)
		tmp[iter] = 'd';
	else if S_ISLNK(mode)
		tmp[iter] = 'l';
	else
		tmp[iter] = '-';
	for (iter++; pos >= 0; pos--, iter++)
	{
		if ((pos == 0 || pos == 3 || pos == 6) && (mode & (1 << pos)))
			tmp[iter] = 'x';
		else if ((pos == 1 || pos == 4 || pos == 7) && (mode & (1 << pos)))
			tmp[iter] = 'w';
		else if ((pos == 2 || pos == 5 || pos == 8) && (mode & (1 << pos)))
			tmp[iter] = 'r';
		else
			tmp[iter] = '-';
	}
	tmp[iter] = '\0';
}

/**
 * path_prep - prepares file name to be properly handled by lstat
 * @dir: directory name
 * @filename: file name
 * Return: string containing filename appended to containing dir with '/'
*/

static char *path_prep(char *dir, char *filename)
{
	char *path = NULL;

	if (!dir || !filename)
		return (NULL);
	path = malloc(sizeof(char) * (_strlen(dir) + 1 + _strlen(filename)) + 1);
	if (!path)
		return (NULL);
	path[0] = '\0';
	_strcat(path, dir);
	if (path[_strlen(dir) - 1] == '/')
		path[_strlen(dir) - 1] = '\0';
	_strcat(path, "/");
	_strcat(path, filename);
	return (path);
}
