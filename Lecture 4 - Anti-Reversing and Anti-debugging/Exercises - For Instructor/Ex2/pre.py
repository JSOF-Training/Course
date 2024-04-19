#! /usr/bin/env python3

import sys
import struct
from funcs import get_func_defs, LAYER1_MAGIC, FUNC_PATTERN, STR_PATTERN, BURN_MAGIC, LFSR
import random


simple_strings = {
'start_str': (b"Yet another assignment where you need to find the password, how original.\n\0", 0x17),
'atlantic_herring': (b"Species: Clupea harengus; Color: red\n\0" , 0x13) ,
'pacific_herring': (b"Species: Clupea pallasii; Color: red\n\0", 0x22),
'input_error_str': (b"Input error. This shouldn't happen\n\0", 0x5B)
}

complex_strings = {
'nope_str': (b"You have entered a password. It's not the right one.\n"
             b"Don't give up, you're so close!\n\0", 0x65),
'reward_str': (b"You've entered the right password! No red herrings were hurt in the making of this exercise.\n"
               b"In keeping with tradition, Here is your reward: \n"
               b"https://youtu.be/S_LFB4Q8d5A\n\0", 0x77),
'second_start_str': (b"Yet another assignment where you need to find the password, how original.\n\0", 0x17),
'second_input_error_str': (b"Input error. This shouldn't happen\n\0", 0x5B)
}

max_s = max([len(s[0]) for s in simple_strings.values()])
max_c = max([len(s[0]) for s in complex_strings.values()])
max_l = max(max_c, max_s)
max_l = max_l + (-max_l) % 8
simple_keys = simple_strings.keys();
complex_keys = complex_strings.keys();

def randbytes(n, seed):
    random.seed(seed)
    return bytes([random.getrandbits(8) for i in range(n)])

def present(string):
    CHUNK = 8
    chunky = [string[i:i+CHUNK] for i in range(0, len(string), CHUNK)]
    out_string = '{\\\n' + '\\\n'.join(['\t '+' '.join([hex(c) + ',' for c in line]) for line in chunky])
    return out_string[0:-1] + '\\\n}'


def mangle_simple(bytes, magic_byte):
    padded = bytes + (max_l-len(bytes)) * b"\xAA"
    return [hex(b ^ ((0x55 + i * magic_byte) % 0x100)) for (i,b) in enumerate(padded)]

def mangle_complex(bytes, num):
    padded = bytes + (max_l-len(bytes)) * b"\xAA"
    out = []
    i = 0
    for d in struct.iter_unpack("<Q", padded):
        reg = LFSR(STR_PATTERN, d[0] + i)
        buffer = reg.roll_n(num)
        out.append(buffer)
        i += 1
    outbytes = struct.pack("<" + len(out) * "Q", *out)
    return [hex(b) for b in outbytes]

pass_1_ints = struct.unpack("<QQQQ", b"You're almost there, but not yet")
pass_2_ints = struct.unpack("<QQQQ", b"That wasn't so hard now, was it?")
obfmagic = 0xeb9074e842b49c38;
xormagic = 0xaa6e7e37202da877;

pass_diff_ints = [pass_1_ints[i] ^ pass_2_ints[i] ^ obfmagic for i in range(len(pass_1_ints))]

defs = [f"#define {key.upper()} {'{'}{', '.join(mangle_simple(simple_strings[key][0], simple_strings[key][1]))}{'}'}\n" for key in simple_keys ]
magics = [f"#define {key.upper()}_MAGIC ({hex(simple_strings[key][1])})\n" for key in simple_keys ]

cdefs = [f"#define {key.upper()} {'{'}{', '.join(mangle_complex(complex_strings[key][0], complex_strings[key][1]))}{'}'}\n" for key in complex_keys ]
cmagics = [f"#define {key.upper()}_MAGIC ({hex(complex_strings[key][1])})\n" for key in complex_keys ]



pass1 = [f"#define PASS1_INT{i} ({hex(pass_1_ints[i] ^ xormagic)})\n" for i in range(len(pass_1_ints))]
pass2 = [f"#define PASS2_INT{i} ({hex(pass_2_ints[i] ^ xormagic)})\n" for i in range(len(pass_2_ints))]
passdiff = [f"#define PASS_XORDIFF{i} ({hex(pass_diff_ints[i])})\n" for i in range(len(pass_2_ints))]

randdef = "#define RANDOM_RED_HERRING " + present(randbytes(0x408, 0xdec0de12))
# call to fake_check_pass with (pass1^xormagic, xormagic);
# change in runtime to call to real_check_pass. don't change pass1^PASS_XORMAGIC
# do change xormagic to finmagic = obfmagic^xormagic

defstring = ("".join(defs) +
             "".join(cdefs) +
             f"\n#define MAX_STR_LENGTH ({max_l})\n\n" +
             "".join(magics)+"\n" +
             "".join(cmagics)+"\n" +
             "".join(pass1)+"\n" +
             "".join(pass2)+"\n" +
             "".join(passdiff) + "\n" +
             f"#define PASS_XORMAGIC ({hex(xormagic)})\n" +
             f"#define PASS_OBFMAGIC ({hex(obfmagic)})\n" +
             f"#define PASS_FINMAGIC ({hex(obfmagic^xormagic)})\n\n" +
             get_func_defs() + "\n" +
             f"#define LAYER1_MAGIC ({hex(LAYER1_MAGIC)})\n" +
             f"#define FUNC_PATTERN ({hex(FUNC_PATTERN)})\n" +
             f"#define STR_PATTERN ({hex(STR_PATTERN)})\n" +
             f"#define BURN_MAGIC ({hex(BURN_MAGIC)})\n" +
             randdef + '\n')


def main():
    if (len(sys.argv) != 2):
        print("Usage: stringmaker.py outfile")
    outfile = sys.argv[1]
    with open(outfile, 'w') as f:
        f.writelines(defstring)


if __name__ == "__main__":
    main()
