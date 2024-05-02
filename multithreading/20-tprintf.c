#include "multithreading.h"

pthread_mutex_t mutex;

/**
 * initialize_mutex - initialize mutual exception
*/
void __attribute__((constructor)) initialize_mutex(void)
{
	if (pthread_mutex_init(&mutex, NULL))
		exit(1);
}

/**
 * destroy_mutex - destroy mutual exception
*/
void __attribute__((destructor)) destroy_mutex(void)
{
	pthread_mutex_destroy(&mutex);
}

/**
 * tprintf - uses printf family to print input formatted string
 * @format: format string to be printed
 * Return: number of characters successfully printed
*/
int tprintf(char const *format, ...)
{
	return (!format ? -1 : printf("[%lu] %s", pthread_self(), format));
}
