#!/usr/bin/env python3

import sys, codecs

password_0 = b"Please let me pass"
herring_1 =  b"This is not the real password?"
password_1 = b"Surprise! I am the correct one"
password_2 =      b"Seriously, just let me pass already"
initial_bytes_2 = codecs.decode('dec0deda7aba5ec0ffeefea575e1ec7b175f00dea770ffee70a57f007ba11fee11abe1', 'hex_codec')

def xorstrings(a, b):
    if len(a) != len(b):
        raise Exception("Length mismatch!")
    return bytes([a[i] ^ b[i] for i in range(len(a))])

def xorwithbyte(s, b):
    return bytes([c ^ b for c in s])

def level_0():
    return '#define PASSWORD_0 ("{s}")\n'.format(s = password_0.decode())

def level_1():
    s_herring = '#define HERRING_1 ("{s}")\n'.format(s = herring_1.decode())
    xorbytes = xorwithbyte(xorstrings(herring_1, password_1), 0x55)
    s_xorbytes = '#define LEVEL_1_BYTES {s}\n'.format(s = present(xorbytes))
    return s_herring + s_xorbytes

def level_2():
    xoredbytes = xorwithbyte(xorstrings(initial_bytes_2, initial_bytes_2[3:]+initial_bytes_2[:3]), 0xAA)
    s_xorbytes = '#define LEVEL_2_BYTES {s}\n'.format(s = present(initial_bytes_2))
    final = xorstrings(password_2, xoredbytes)
    s_final = '#define LEVEL_2_FINAL {s}\n'.format(s = present(final + b'\x00'))
    return s_xorbytes + s_final






def present(string):
    CHUNK = 8
    chunky = [string[i:i+CHUNK] for i in range(0, len(string), CHUNK)]
    out_string = '{\\\n' + '\\\n'.join(['\t '+' '.join([hex(c) + ',' for c in line]) for line in chunky])
    return out_string[0:-1] + '\\\n}'


def main():
    outfile = sys.argv[1]
    outstring = level_0() + level_1() + level_2()
    f = open(outfile, 'w')
    f.write(outstring)
    f.close()


if __name__ == "__main__":
    main()
