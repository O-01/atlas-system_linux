#include "hls.h"

;

/**
 * sort_dir - sorts data for output as requested
 * 
*/

int dir_sort(DIR *open_up, char *dir, int flags)
{
	struct dirent *re_d = NULL;
	s_dt *sort_list = NULL;
	char *delim = "  ", *fuller_path = NULL;
	int printed = 0;

	if (flags & (1 << 0) || flags & (1 << 1))
		delim = "\n";
	for (; (re_d = readdir(open_up));)
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

	}
	for (;;)
	{
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

static int sort_maker(char *filename, char *dir_name, s_dt **list)
{
	s_dt *add = NULL;
	char *fuller_path = NULL;

	add = malloc(sizeof(s_dt));
	if (!add)
		return (-1);
	add->name = _strdup(filename);
	fuller_path = path_prep(dir_name, filename);
	populate_data(add, fuller_path);
	
}