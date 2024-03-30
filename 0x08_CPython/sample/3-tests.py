#!/usr/bin/python3 -u

import ctypes

lib = ctypes.CDLL('./libPython.so')
lib.print_python_list.argtypes = [ctypes.py_object]
lib.print_python_bytes.argtypes = [ctypes.py_object]
lib.print_python_float.argtypes = [ctypes.py_object]
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
f = 3.14
lib.print_python_float(f)
listo = [
    -1.0, -0.1, 0.0, 1.0, 3.14, 3.14159, 3.14159265,
    3.1415926535897932384626433832795028841971693993751058209749445923078164062
]
print(listo)
lib.print_python_list(listo)
lib.print_python_float(listo)
lib.print_python_list(f)
f = 9492307816406286.0
lib.print_python_float(f)
