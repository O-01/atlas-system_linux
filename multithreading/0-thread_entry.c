#include "multithreading.h"

/**
 * thread_entry - entry point to new thread
 * @arg: holds address of string to be printed
 * Return: exit thread with arg
*/
void *thread_entry(void *arg)
{
	printf("%s\n", (char *)arg);
	pthread_exit(arg);
}
