#!/usr/bin/env python3


import sys
from subprocess import check_output
from io import StringIO
import struct

ORIGINAL_HMMM = 0xA5C5
def main():
    if (3 != len(sys.argv)):
        print(f"Usage: {sys.argv[0]} [binary input file] [binary output file]")
    binfn = sys.argv[1]
    outfn = sys.argv[2]

    hmmm = struct.pack("<H", ORIGINAL_HMMM)
    with open(binfn, 'rb') as file:
        filedata = file.read()
    offset = filedata.find(hmmm);
    if (-1 == offset):
        print("Couldn't find original hmmm")
        return -1;
    print(f"found offset {hex(offset)}")
    output = check_output(["./checksummer", binfn, "{0:X}".format(offset), outfn])
    MATCH_STR = b'Match for hmm='
    if (MATCH_STR not in output):
        print("No checksum match or checksummer error")
        print(output)
        return -1;
    new_cs = output.split(MATCH_STR)[1].split(b'\n')[0]
    print(f"Checksum match found for hmmm = {new_cs}")


    print("Done.")
    return 0;


if __name__ == "__main__":
    main()
