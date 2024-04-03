#include <Python.h>
#include <stdio.h>

/**
 * print_python_int - prints python 'int' value
 * @p: pointer to 'PyObject' struct (python 'int' type expected)
*/
void print_python_int(PyObject *p)
{
	unsigned long int out = 0;

	if (!p || !PyLong_Check(p))
	{
		printf("Invalid Int Object\n");
		return;
	}
	digit *py_int = ((PyLongObject *)p)->ob_digit;
	Py_ssize_t size = ((PyVarObject *)p)->ob_size;
	Py_ssize_t abs_size = size < 0 ? -size : size;
	if (abs_size > 3 || (abs_size == 3 && py_int[2] > 0xf))
	{
		printf("C unsigned long int overflow\n");
		return;
	}
	for (Py_ssize_t iter = 0; iter < abs_size; ++iter)
		out += py_int[iter] * (1UL << (PyLong_SHIFT * iter));
	if (size < 0)
		putchar('-');
	printf("%lu\n", out);
}
