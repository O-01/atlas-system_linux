#include "strace.h"

int main(__attribute__((unused)) int argc, char **argv, char **envp)
{
	pid_t pid;
	/* long call_no; */
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
		forever
		{
			wait(&status);
			if (WIFEXITED(status))
				break;
			/* call_no = ptrace(PTRACE_PEEKUSER, pid, 8 * ORIG_RAX, NULL); */
			if (!ptrace(PTRACE_GETREGS, pid, NULL, &regs) && (!alt || alt & 1))
				fprintf(stderr, "%llu\n", regs.orig_rax);
			ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
			alt++;
		}
	}
	return (0);
}
