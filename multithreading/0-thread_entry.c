#include "multithreading.h"

/**
 * thread_entry - entry point to new thread
 * @arg: holds address of string to be printed
 * Return: 
*/
void *thread_entry(void *arg)
{
	printf("%s\n", (char *)arg);
	pthread_exit(arg);
}
