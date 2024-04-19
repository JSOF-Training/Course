#!/usr/bin/env python3
# aUPutgPd58o

import random, sys, codecs

colors = [
'Black',
'Navy',
'Dark blue',
'Medium blue',
'Blue',
'Dark green',
'Green',
'Teal',
'Dark cyan',
'Deep sky blue',
'Dark turquoise',
'Lime',
'Spring green',
'Aqua',
'Cyan',
'Midnight blue',
'Dodger blue',
'Light sea green',
'Forest green',
'Seagreen',
'Dark slate gray',
'Lime green',
'Turquoise',
'Royal blue',
'Steel blue',
'Dark slate blue',
'Medium turquoise',
'Indigo',
'Dark olive green',
'Cadet blue',
'Cornflower blue',
'Dim gray',
'Slate blue',
'Olive drab',
'Lawn green',
'Chartreuse',
'Aquamarine',
'Maroon',
'Purple',
'Gray',
'Sky blue',
'Light sky blue',
'Blue violet',
'Dark red',
'Dark magenta',
'Saddle brown',
'Dark sea green',
'Light green',
'Medium purple',
'Dark violet',
'Pale green',
'Dark orchid',
'Yellow green',
'Sienna',
'Brown',
'Dark gray',
'Light blue',
'Green yellow',
'Pale turquoise',
'Powder blue',
'Firebrick',
'Dark goldenrod',
'Medium orchid',
'Rosy brown',
'Dark khaki',
'Silver',
'Indian red',
'Peru',
'Chocolate',
'Tan',
'Light grey',
'Thistle',
'Orchid',
'Goldenrod',
'Crimson',
'Gainsboro',
'Plum',
'Burlywood',
'Light cyan',
'Lavender',
'Dark salmon',
'Violet',
'Light coral',
'Khaki',
'Alice blue',
'Honeydew',
'Azure',
'Sandy brown',
'Wheat',
'Beige',
'Whitesmoke',
'Mint cream',
'Ghost white',
'Salmon',
'Antique white',
'Linen',
'Old lace',
'Red',
'Fuchsia',
'Magenta',
'Deep pink',
'Orange red',
'Tomato',
'Hot pink',
'Coral',
'Dark orange',
'Light salmon',
'Orange',
'Light pink',
'Pink',
'Gold',
'Peach puff',
'Navajo white',
'Moccasin',
'Bisque',
'Misty rose',
'Blanched almond',
'Papaya whip',
'Lavender blush',
'Sea shell',
'Cornsilk',
'Lemon chiffon',
'Floral white',
'Snow',
'Yellow',
'Light yellow',
'Ivory',
'White'
]

def randbytes(n):
    return bytes([random.getrandbits(8) for i in range(n)])

def cbytearray(b):
    return '{' + ', '.join(hex(c) for c in b) + '}'

def present(string):
    CHUNK = 8
    chunky = [string[i:i+CHUNK] for i in range(0, len(string), CHUNK)]
    out_string = '{\\\n' + '\\\n'.join(['\t '+' '.join([hex(c) + ',' for c in line]) for line in chunky])
    return out_string[0:-1] + '\\\n}'


def encode_color(i, cbytes):
    magic_byte =  (7 * i + 0x33) ^ 0xCC;
    return bytes([((b + 111 * (magic_byte + i)) % 0x100) for i, b in enumerate(cbytes)])

def level_0():
    maxlen = max([len(c) for c in colors])

    preamble = (f'#define COLOR_SIZE ({maxlen + 1}) \n' +
                f'#define COLOR_LEN ({len(colors)}) \n' +
                '#define ENCODED_COLORS { \\\n')
    postamble = '\\\n}\n'

    size_str = f'#define COLOR_SIZE ({maxlen})'
    random.seed(0x1337d00d) # Be deterministic
    encoded_all = []
    for i, c in enumerate(colors):
        magic_byte = (7 * i + 0x33) ^ 0xCC
        rest = maxlen - len(c)
        colorbytes = bytes(c, 'ascii') + b'\x00' + randbytes(rest)
        encoded_c = encode_color(i, colorbytes)
        encoded_all.append(encoded_c)
    return preamble + ', \\\n'.join(cbytearray(c) for c in encoded_all) + postamble


def main():
    outfile = sys.argv[1]
    outstring = level_0() #+ level_1() + level_2()
    f = open(outfile, 'w')
    f.write(outstring)
    f.close()


if __name__ == "__main__":
    main()
