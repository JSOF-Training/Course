#!/usr/bin/env python3

import getcolor
import string
import random
import sys
from subprocess import check_output
from io import StringIO

def main():
    chars = ''.join(list(set(string.ascii_letters) - set(string.whitespace) | set(' ')))
    for i in range(0x400):
        if (0 == i % 0x40):
            print(f'i={i}')
        random.seed()
        nl = random.randint(4,16)
        ql = random.randint(4,16)
        name = ''.join(random.sample(chars, nl))
        quest = ''.join(random.sample(chars, ql))
        color = getcolor.colors[getcolor.get_magic_index(bytes(name, 'ascii'), bytes(quest, 'ascii'))]
        instring = name + '\n' + quest + '\n' + color + '\n'
        with open('tempfile', 'w') as file:
            file.write(instring)
        with open('tempfile', 'r') as file:
            output = check_output(["./bridgeofdeath"], stdin=file)
        if b'Alright' not in output:
            print(name, quest, color, output)
            raise Exception('False color given')

    print("Done.")


if __name__ == "__main__":
    main()
