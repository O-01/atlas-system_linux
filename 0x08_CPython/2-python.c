#include <Python.h>
#include <stdio.h>

void print_python_list(PyObject *p);
void print_python_bytes(PyObject *p);

/**
 * print_python_list - prints python 'list' internals
 * @p: pointer to 'PyObject' struct (python 'list' type expected)
*/
void print_python_list(PyObject *p)
{
	PyListObject *list_ptr = NULL;
	PyObject *item = NULL;
	Py_ssize_t list_len = 0, i = 0;
	const char *type_name = NULL;

	if (!p || !PyList_Check(p))
		return;
	list_len = PyObject_Length(p);
	list_ptr = (PyListObject *)p;
	printf(
		"[*] Python list info\n"
		"[*] Size of the Python List = %ld\n"
		"[*] Allocated = %ld\n",
		list_len, list_ptr->allocated);
	for (i = 0; i < list_len; ++i)
	{
		item = list_ptr->ob_item[i];
		type_name = item->ob_type->tp_name;
		printf("Element %ld: %s\n", i, type_name);
		if (PyBytes_Check(item))
			print_python_bytes(item);
	}
}

/**
 * print_python_bytes - prints python 'bytes' object
 * @p: pointer to 'PyObject' struct (python 'bytes' type expected)
*/
void print_python_bytes(PyObject *p)
{
	PyBytesObject *bytes_cast = NULL;
	Py_ssize_t bytes_size = 0, bytes_len_null_10 = 0, iter = 0;

	if (!p || !PyBytes_Check(p))
	{
		printf(
			"[.] bytes object info\n"
			"  [ERROR] Invalid Bytes Object\n");
		return;
	}
	bytes_cast = (PyBytesObject *)p;
	bytes_size = PyBytes_Size(p);
	bytes_len_null_10 = bytes_size + 1 > 10 ? 10 : bytes_size + 1;
	printf(
		"[.] bytes object info\n"
		"  size: %ld\n"
		"  trying string: %s\n"
		"  first %ld bytes:",
		bytes_size,
		PyBytes_AsString(p),
		bytes_len_null_10);
	for (iter = 0; iter < bytes_len_null_10; ++iter)
		printf(" %02x", bytes_cast->ob_sval[iter] & 0xff);
	putchar('\n');
}
