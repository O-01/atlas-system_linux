#!/usr/bin/python3 -u

import ctypes

lib = ctypes.CDLL('./libPython.so')
lib.print_python_list.argtypes = [ctypes.py_object]
lib.print_python_bytes.argtypes = [ctypes.py_object]
s = b"Hello"
lib.print_python_bytes(s)
b = b'\xff\xf8\x00\x00\x00\x00\x00\x00'
lib.print_python_bytes(b)
b = b'What does the \'b\' character do in front of a string literal?'
lib.print_python_bytes(b)
listo = [b'Hello', b'World']
lib.print_python_list(listo)
del listo[1]
lib.print_python_list(listo)
listo = listo + [4, 5, 6.0, (9, 8), [9, 8, 1024], b"Holberton", "Betty"]
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
listo = ["Holberton"]
lib.print_python_list(listo)
lib.print_python_bytes(listo)
