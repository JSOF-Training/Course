#! /usr/bin/env python3

from elftools.elf.elffile import ELFFile
import sys


def patch(outfile, infile):
    with open(infile, 'rb') as f:
        elf = ELFFile(f)
        # Find the symbol table.
        symtab = elf.get_section_by_name('.symtab')
        text = elf.get_section_by_name('.text')
        vr_offset = text['sh_offset'] - text['sh_addr']
        # Test we can find a symbol by its name.
        # print([s.name for s in symtab.iter_symbols()])
        mainf = symtab.get_symbol_by_name('check')[0]
        value = mainf['st_value']
        location = value + vr_offset
        size = mainf['st_size']

    with open(infile, 'rb') as f:
        data = list(f.read())
    for i in range(location + 0x10, location + size - 1):
        if ((data[i] & 0xF0) == 0x70):
            data[i] ^= 0x01
        elif ((data[i] == 0x0F) and ((data[i+1] & 0xF0) == 0x80)):
            data[i+1] ^= 0x01
    for i in range(location + 0x10, location + size - 1):
        if ((data[i] == 0x3C) and ((data[i+1] & 0xE0) == 0x40)):
            data[i+1] ^= 0x25
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
