#include "hls.h"

/**
 * *_strcpy - copies src string to dest
 * @dest: destination memory
 * @src: string to copy terminated by \0
 * Return: pointer to dest
 */

char *_strcpy(char *dest, char *src)
{
	int iter = 0;

	for (; src[iter]; iter++)
		dest[iter] = src[iter];
	dest[iter] = '\0';
	return (dest);
}

/**
 * _strcmp - compares 2 strings & shows value upon reaching difference
 * @str1: string 1
 * @str2: string 2
 * Return: value of difference
 */

int _strcmp(char *str1, char *str2)
{
	int iter = 0;

	while (str1[iter] == str2[iter] && (str1[iter] || str2[iter]))
		iter++;
	iter = (str1[iter] - str2[iter]);
	return (iter);
}

/**
 * _strlen - returns length of input string
 * @str: string to find length
 * Return: number of characters in string
 */

int _strlen(char *str)
{
	int iter = 0;

	for (; str[iter]; iter++)
		;
	return (iter);
}
