#include "multithreading.h"

int tprintf(char const *format, ...);

pthread_mutex_t mutex;

/**
 * initialize_mutex - initialize mutual exception
*/
void __attribute__((constructor)) initialize_mutex(void)
{
	if (pthread_mutex_init(&mutex, NULL))
		write(2, "Mutex init failure\n", 19), exit(1);
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
	int printf_chars = 0, vprintf_chars = 0;
	va_list varargs;

	if (!format)
		return (-1);
	va_start(varargs, format);
	pthread_mutex_lock(&mutex);
	printf_chars = printf("[%lu] ", pthread_self());
	vprintf_chars = vprintf(format, varargs);
	pthread_mutex_unlock(&mutex);
	va_end(varargs);
	return (printf_chars + vprintf_chars);
}
