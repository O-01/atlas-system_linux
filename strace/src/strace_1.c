#include "strace.h"

/**
 * main - entry point
 * @argc: count of arguments passed to program upon launch
 * @argv: argument vector
 * @envp: environmental variable vector
 * Return: 0 upon success
*/
int main(__attribute__((unused)) int argc, char **argv, char **envp)
{
	pid_t pid;
	int status, alt = 0;
	user_regs regs;

	pid = fork();
	if (pid == -1)
		perror(argv[1]), exit(1);
	else if (pid == 0)
	{
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		if (execve(argv[1], &argv[1], envp) == -1)
			perror(argv[1]), exit(1);
	}
	else
	{
		while (1)
		{
			wait(&status);
			if (WIFEXITED(status))
				break;
			if (!ptrace(PTRACE_GETREGS, pid, NULL, &regs) && (!alt || alt & 1))
				fprintf(stderr, "%lu : ", (size_t)regs.orig_rax),
				fprintf(stderr, "%s\n", SYSCALL_NAME(regs));
			ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
			alt++;
		}
	}
	return (0);
}
