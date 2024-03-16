#include <signal.h>
#include <sys/types.h>

/**
 * pid_exist - verifies if process exists, given its PID
 * @pid: process ID
 * Return: 1 if process with PID exists, else 0
*/
int pid_exist(pid_t pid) { return (!kill(pid, 0)); }
