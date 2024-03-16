#include "signals.h"

static void quit_handler(int quit_sig, siginfo_t *info, void *nothing);

/**
 * trace_signal_sender - defines handler for signal SIGQUIT, traces origin PID
 * Return: 0 upon success, -1 upon error
*/
int trace_signal_sender(void)
{
	sigact_t action;

	action.sa_sigaction = quit_handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = SA_RESTART | SA_SIGINFO;
	return (sigaction(SIGQUIT, &action, NULL));
}

/**
 * quit_handler - handler for SIGQUIT, prints PID of sending process
 * @quit_sig: incoming signal, expected to be SIGQUIT
 * @info: struct containing data relevant to sent signal
 * @_: not used
*/
static void
quit_handler(int quit_sig, siginfo_t *info, __attribute__((unused)) void *_)
{
	char pid_str[16];
	int sender_pid = info->si_pid, pid_digit = 15, pid_len = 0;

	if (quit_sig == SIGQUIT)
	{
		do {
			pid_str[--pid_digit] = sender_pid % 10 + '0';
			sender_pid /= 10;
			pid_len++;
		} while (sender_pid > 0);
		write(1, "SIGQUIT sent by ", 16);
		write(1, &pid_str[pid_digit], pid_len);
		write(1, "\n", 1);
	}
}
