#include <Python.h>
#include <stdio.h>

/**
 * print_python_string - prints python 'string' data
 * @p: pointer to 'PyObject' struct (python 'str' type expected)
*/
void print_python_string(PyObject *p)
{
	PyASCIIObject *unicode_cast = NULL;

	printf("[.] string object info\n");
	if (!p || !PyUnicode_Check(p))
	{
		printf("  [ERROR] Invalid String Object\n");
		return;
	}
	unicode_cast = (PyASCIIObject *)p;
	printf(
		"  type: compact %s\n"
		"  length: %ld\n"
		"  value: %ls\n",
		unicode_cast->state.ascii ? "ascii" : "unicode object",
		unicode_cast->length,
		_PyUnicode_AsUnicode(p));
}
