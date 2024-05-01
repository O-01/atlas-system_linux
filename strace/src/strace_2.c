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
	int status, alt = 0, printed = 0;
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
			if (!GETREGS(regs) && ALT && SUPPORTED(regs))
				printf("%s", SYSCALLNAME(regs)),
				printed = 0;
			/* print ugly output upon write, per project */
			if ((!alt || ((alt - 1) & 1)) && !printed)
				printf(" = %s%lx\n", regs.rax ? "0x" : "", (size_t)regs.rax),
				printed = 1;
			ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
			alt++;
			fflush(stdout);
		}
		write(1, " = ?\n", 5);
	}
	return (0);
}
