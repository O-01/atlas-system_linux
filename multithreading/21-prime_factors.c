#include "multithreading.h"

static unsigned long sqrtul(unsigned long n);
static int new_factor(list_t **factor_list, unsigned long number);

/**
 * prime_factors - factorizes number into list of prime factors
 * @s: string representation of number to factorize
 * Return: prime factor list
*/
list_t *prime_factors(char const *s)
{
	unsigned long s_lit = strtoul(s, NULL, 10), prime;
	list_t *factor_list;

	if (!s)
		return (NULL);
	factor_list = malloc(sizeof(list_t));
	if (!factor_list)
		return (NULL);
	list_init(factor_list);
	for (; (s_lit - 1) & 1; s_lit >>= 1)
		if (new_factor(&factor_list, 2) == -1)
			return (NULL);
	for (prime = 3; prime < sqrtul(s_lit); prime += 2)
		for (; !(s_lit % prime); s_lit /= prime)
			if (new_factor(&factor_list, prime) == -1)
				return (NULL);
	if (s_lit > 2)
		if (new_factor(&factor_list, s_lit) == -1)
			return (NULL);
	return (factor_list);
}

/**
 * sqrtul - calculates square root of input number
 * @n: input number
 * Return: square root of input number n
*/
static unsigned long sqrtul(unsigned long n)
{
	double root;

	asm("fsqrt" : "=t" (root) : "0" ((double)n));
	return ((unsigned long)root);
}

/**
 * new_factor - adds new factor node to doubly linked list of prime factors
 * @factor_list: doubly linked list of prime factors
 * @prime: prime factor for node content
 * Return: 0 upon success, -1 upon memory allocation failure
*/
static int new_factor(list_t **factor_list, unsigned long prime)
{
	unsigned long *add_factor;

	add_factor = malloc(sizeof(unsigned long));
	if (!add_factor)
		return (-1);
	*add_factor = prime;
	list_add(*factor_list, add_factor);
	return (0);
}
