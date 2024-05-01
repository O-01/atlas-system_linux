#include "multithreading.h"

/**
 * tprintf - uses printf family to print input formatted string
 * @format: format string to be printed
 * Return: number of characters successfully printed
*/
int tprintf(char const *format, ...)
{
	return (!format ? -1 : printf("[%lu] %s", pthread_self(), format));
}
