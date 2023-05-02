#!/usr/bin/python
import os
import sys

from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes

def encrypt(plaintext, key):
    cipher = Cipher(algorithms.AES(key), modes.ECB(), backend=default_backend())
    encryptor = cipher.encryptor()
    return encryptor.update(plaintext) + encryptor.finalize()

def read_file(filename):
    with open(filename, 'r') as f:
        return f.read()

def write_file(filename, contents):
    with open(filename, 'w') as f:
        f.write(contents)

def replace_strings(c_code, key):
    new_code = ''
    in_string = False
    string = ''

    for char in c_code:
        if char == '"' and not in_string:
            in_string = True
        elif char == '"' and in_string:
            in_string = False
            encrypted_string = encrypt(string.encode(), key)
            new_code += f'"{encrypted_string.hex()}'
            string = ''
        elif in_string:
            string += char

        if not in_string:
            new_code += char

    return new_code

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print(f'Usage: {sys.argv[0]} <input_file> <output_file>')
        sys.exit(1)

    input_file = sys.argv[1]
    output_file = sys.argv[2]

    c_code = read_file(input_file)
    key = os.urandom(16)
    encrypted_c_code = replace_strings(c_code, key)
    write_file(output_file, encrypted_c_code)
