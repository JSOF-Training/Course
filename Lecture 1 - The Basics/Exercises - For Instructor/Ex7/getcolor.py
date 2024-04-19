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

"""
char color_index = 0x55;
for(i=0; i<0x100; i++)
{
if (0 == name[i])
  break;
color_index += (name[i] ^ (0xcc + i));;
}
for(i=0; i<0x100; i++)
{
if (0 == name[i])
  break;
color_index += (name[i] ^ (0x33 - i));
}
"""

def get_magic_index(name, quest):
    index = 0x55
    for b in name:
        index = (index + b) & 0x7f
        index = (index << 3 | index >> 4) & 0x7F
    for b in quest:
        index = (index + b) & 0x7f
        index = (index << 4 | index >> 3) & 0x7F
    for b in b'UUUU':
        index = (index + b) & 0x7f
        index = (index << 3 | index >> 4) & 0x7F

    return index & 0x7F

def main():
    if (2 == len(sys.argv)):
        color_index = eval(sys.argv[1])
    elif (1 == len(sys.argv)):
        name = input('What is your name? ')
        quest = input('What is your quest? ')
        color_index = get_magic_index(bytes(name, 'ascii'), bytes(quest, 'ascii'))
    elif (3 == len(sys.argv)):
        name = sys.argv[1]
        quest = sys.argv[2]
        color_index = get_magic_index(bytes(name, 'ascii'), bytes(quest, 'ascii'))
    else:
        print(f'Usage: {sys.argv[0]} name quest')
        print(f'Usage: {sys.argv[0]} color_index')
        print(f'or use without arguments and enter name and quest in terminal')
        return -1

    print(hex(color_index), colors[color_index])
    return 0


if __name__ == "__main__":
    main()
