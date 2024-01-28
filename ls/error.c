#include "hls.h"

/**
 * error_dump - handles errors
 * @exec: name of program executable
 * @filename: name of file that caused error
 * @code: represents error code
*/

void error_dump(char *exec, char *filename, int code)
{
	if (code == 2)
		fprintf(
			stderr,
			"%s: cannot access %s: No such file or directory\n",
			exec, filename
		);
	else if (code == 13)
		fprintf(
			stderr,
			"%s: cannot open directory %s: Permission denied\n",
			exec, filename
		);
	else
		perror("eRrOr");
}
