#! /usr/bin/env python3

from elftools.elf.elffile import ELFFile
from funcs import get_all_patches, get_layer1, LAYER1_MAGIC, FUNC_PATTERN, LFSR
import struct

import sys


def scramble(data, magic):
    buffer = magic
    reg = LFSR(FUNC_PATTERN, magic)
    out = []
    for d in struct.iter_unpack("<Q", data):
        buffer = reg.roll_once()
        out.append(d[0] ^ buffer)
    return struct.pack("<" + len(out) * "Q", *out)

def do_layer1(data, key):
    out = []
    for d in struct.iter_unpack("<L", data):
        out.append(d[0] ^ LAYER1_MAGIC)
    return struct.pack("<" + len(out) * "L", *out)


def patch(outfile, infile):
    with open(infile, 'rb') as f:
        elf = ELFFile(f)
        # Find the symbol table.
        patches = get_all_patches(elf)
        l1_start, l1_end = get_layer1(elf)

    with open(infile, 'rb') as f:
        data = f.read()

    for p in patches:
        data = (data[:p['start']] +
                scramble(data[p['start']:p['end']], p['magic']) +
                data[p['end']:])
    data = data[:l1_start] + do_layer1(data[l1_start:l1_end], LAYER1_MAGIC) + data[l1_end:]

    with open(outfile, 'wb') as f:
        f.write(bytes(data))








def main():
    if (len(sys.argv) != 3):
        print("Usage: post.py outfile infile")
    outfile = sys.argv[1]
    infile = sys.argv[2]
    patch(outfile, infile)


if __name__ == "__main__":
    main()
