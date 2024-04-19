#! /usr/bin/env python3
import struct
import zlib
import sys


def main():
    if (len(sys.argv) != 4):
        print("Usage: checksummer.py [input filename] [output filename] [crc32 hex offset]")
        return 0
    infile = sys.argv[1]
    outfile = sys.argv[2]
    offset = eval(sys.argv[3])
    with open(infile, 'rb') as f:
        contents = f.read()
    checksum = test(contents, offset)
    if (-1 == checksum):
        print("Couldn't find checksum")
    else:
        print(f"Checksum is {hex(checksum)}")

    with open(outfile, 'wb') as f:
        f.write(contents[:offset])
        f.write(struct.pack('<L', checksum))
        f.write(contents[offset+4:])

    return 0


def test(contents, offset):
    for i in range(2 ** 32):
        if (i % 0x10000 == 0):
            print(f"i={hex(i)}")

        new_contents = contents[:offset] + struct.pack('<L', i) + contents[offset+4:]
        checksum = zlib.crc32(new_contents)
        if ((i % 0x10000) == (checksum % 0x10000)):
            return i
    return -1




if __name__ == "__main__":
    main()
