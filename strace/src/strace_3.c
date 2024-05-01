#include "strace.h"

static void print_param(user_regs *regs, size_t param);

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
	size_t param = 0;
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
			{
				printf("%s(", SYSCALLNAME(regs)),
				printed = 0;
				for (param = 0; param < PARAM_COUNT(regs); param++)
					if (SYSCALLPARAM(regs, param))
						print_param(&regs, param);
			}
			/* print ugly output upon write, per project */
			if ((!alt || ((alt - 1) & 1)) && !printed)
				printf(") = %s%lx\n", HEX_PREFIX(regs.rax), (size_t)regs.rax),
				printed = 1;
			ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
			alt++;
			fflush(stdout);
		}
		write(1, ") = ?\n", 6);
	}
	return (0);
}

/**
 * print_param - prints system call argument value held in register at `param`
 *               position
 * @regs: user regs struct corresponding to current system call
 * @param: system call argument position
*/
static void print_param(user_regs *regs, size_t param)
{
	if (SYSCALLPARAM(*regs, param) == VOID)
		return;
	else if (SYSCALLPARAM(*regs, param) == VARARGS)
	{
		printf("...");
		return;
	}
	switch (param)
	{
		case 0:
			printf("%s%lx", HEX_PREFIX(regs->rdi), (size_t)regs->rdi);
		break;
		case 1:
			printf("%s%lx", HEX_PREFIX(regs->rsi), (size_t)regs->rsi);
		break;
		case 2:
			printf("%s%lx", HEX_PREFIX(regs->rdx), (size_t)regs->rdx);
		break;
		case 3:
			printf("%s%lx", HEX_PREFIX(regs->r10), (size_t)regs->r10);
		break;
		case 4:
			printf("%s%lx", HEX_PREFIX(regs->r8), (size_t)regs->r8);
		break;
		case 5:
			printf("%s%lx", HEX_PREFIX(regs->r9), (size_t)regs->r9);
		break;
		default:
		break;
	};
	if ((param + 1) < PARAM_COUNT(*regs))
		printf(", ");
}
