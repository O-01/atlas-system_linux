#include "hls.h"

static void printer_loop(DIR *open_up, int flags);
static void l_print(char *file);
static void mode_decoder(mode_t mode, char **file_mode);

/**
 * printer - print directory contents
 * @list: directory of which contents are to be printed
 * @flags: flags that determine what should be printed
 * @o_p: flag indicates loop has already printed, if applicable
 * @loop_flag: flag indicates loop usage
 * Return: 2 upon file/directory not found, 13 permission denied, 0 otherwise
*/

int printer(file_l *list, int flags, int o_p, int loop_flag)
{
	DIR *open_up = NULL;

	if (!list)
		open_up = opendir(".");
	else
		open_up = opendir(list->name);
	if (!open_up)
		return (errno);
	if (loop_flag != 0)
	{
		if (o_p)
			printf("\n\n");
		printf("%s:\n", list->name);
	}
	printer_loop(open_up, flags);
	closedir(open_up);
	return (0);
}

/**
 * printer_loop - prints what is requested by command
 * @open_up: directory of which contents to be printed
 * @flags: flags by which format of what is to be printed is determined
*/

static void printer_loop(DIR *open_up, int flags)
{
	struct dirent *read_d = NULL;
	char *name = NULL, *delim = "  ";
	int printed = 0;

	if (flags & (1 << 0) || flags & (1 << 1))
		delim = "\n";
	for (printed = 0; (read_d = readdir(open_up));)
	{
		name = _strdup(read_d->d_name);
		if (name[0] == '.')
		{
			if (
				(!name[1] || (name[1] == '.' && !name[2])) &&
				!(flags & (1 << 2))
			)
			{
				free(name), name = NULL;
				continue;
			}
			else if (
				(name[1] || (name[1] == '.' && name[2])) &&
				!((flags & (1 << 2)) | (flags & (1 << 3)))
			)
			{
				free(name), name = NULL;
				continue;
			}
		}
		if (printed)
			printf("%s", delim);
		if (flags & (1 << 1))
			l_print(name);
		printf("%s", name);
		free(name), name = NULL;
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
		printf("%.24s ", ctime(&extract.st_mtime));
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
