#!/usr/bin/python3 -u

import ctypes

lib = ctypes.CDLL('./libPyList.so')
lib.print_python_list.argtypes = [ctypes.py_object]
listo = ['hello', 'World']
lib.print_python_list(listo)
del listo[1]
lib.print_python_list(listo)
listo = listo + [4, 5, 6.0, (9, 8), [9, 8, 1024], "Holberton"]
lib.print_python_list(listo)
listo = []
lib.print_python_list(listo)
listo.append(0)
lib.print_python_list(listo)
listo.append(1)
listo.append(2)
listo.append(3)
listo.append(4)
lib.print_python_list(listo)
listo.pop()
lib.print_python_list(listo)
