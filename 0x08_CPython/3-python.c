#include <Python.h>
#include <stdio.h>

void print_python_list(PyObject *p);
void print_python_bytes(PyObject *p);
void print_python_float(PyObject *p);

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

	printf("[*] Python list info\n");
	if (!p || !PyList_Check(p))
	{
		printf("  [ERROR] Invalid List Object\n");
		return;
	}
	list_len = PyObject_Length(p);
	list_ptr = (PyListObject *)p;
	printf(
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
		else if (PyFloat_Check(item))
			print_python_float(item);
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

	printf("[.] bytes object info\n");
	if (!p || !PyBytes_Check(p))
	{
		printf("  [ERROR] Invalid Bytes Object\n");
		return;
	}
	bytes_cast = (PyBytesObject *)p;
	bytes_size = PyBytes_Size(p);
	bytes_len_null_10 = bytes_size + 1 > 10 ? 10 : bytes_size + 1;
	printf(
		"  size: %ld\n"
		"  trying string: %s\n"
		"  first %ld bytes:",
		bytes_size,
		bytes_cast->ob_sval,
		bytes_len_null_10);
	for (iter = 0; iter < bytes_len_null_10; ++iter)
		printf(" %02x", bytes_cast->ob_sval[iter] & 0xff);
	putchar('\n');
}

/**
 * print_python_float - prints python 'float' object
 * @p: pointer to 'PyObject' struct (python 'float' type expected)
*/
void print_python_float(PyObject *p)
{
	PyFloatObject *float_cast = NULL;
	char *float_str = NULL;

	printf("[.] float object info\n");
	if (!p || !PyFloat_Check(p))
	{
		printf("  [ERROR] Invalid Float Object\n");
		return;
	}
	float_cast = (PyFloatObject *)p;
	/**
	 * Correct arguments to PyOS_double_to_string in 3.11+ for desired output:
	 * float_cast->ob_fval, 'g', 16, Py_DTSF_ADD_DOT_0, NULL
	*/
	float_str = PyOS_double_to_string(float_cast->ob_fval, 'g', 16, 0, NULL);
	/**
	 * Print statement in 3.11+ assuming correct arguments used above:
	 * printf("  value %s\n", float_str);
	*/
	printf("  value: %s%s\n", float_str, strchr(float_str, '.') ? "" : ".0");

}
