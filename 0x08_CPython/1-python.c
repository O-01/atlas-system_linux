#include <Python.h>
#include <stdio.h>

/**
 * print_python_list - Prints Python-list internals
 * @p: Pointer to 'PyObject' struct (python 'list' type expected)
*/
void print_python_list(PyObject *p)
{
	PyListObject *list_ptr = NULL;
	PyObject *item = NULL;
	Py_ssize_t list_len, i;
	const char *type_name = NULL;

	/**
	 * python3 equivalent:
	 * if not isinstance(p, list):
	 *     return
	 */
	if (!p || !PyList_Check(p))
		return;
	/**
	 * python3 equivalent:
	 * `list_len = len(p)`
	 */
	list_len = PyObject_Length(p);
	/* print expected `list` implementation details */
	list_ptr = (PyListObject *)p;
	printf(
		"[*] Python list info\n"
		"[*] Size of the Python List = %ld\n"
		"[*] Allocated = %ld\n",
		list_len, list_ptr->allocated);
	/**
	 * python3 equivalent:
	 * for i in range(list_len):
	 *     print(f'Element {i}: {type(p[i]).__name__}')
	 */
	for (i = 0; i < list_len; ++i)
	{
		item = list_ptr->ob_item[i];
		type_name = item->ob_type->tp_name;
		printf("Element %ld: %s\n", i, type_name);
	}
}
