#! /usr/bin/env python3

import sys

strings = {'usage_str': b"Usage: bizarro password\0" ,
'congrats_str': b"Congratulations! this is the correct password.\0",
'sorry_str': b"Sorry, the password is incorrect.\0",
'reward_fmt': b"Here's your educational reward: https://youtu.be/%s\n\0"}

keys = strings.keys();

defs = [f"char {key}[] = {'{'}{', '.join([hex(c ^ 0xA5) for c in strings[key]])}{'}'};" for key in keys ]

defstring = "\\\n".join(defs) + "\n"

def main():
    if (len(sys.argv) != 2):
        print("Usage: stringmaker.py outfile")
    outfile = sys.argv[1]
    with open(outfile, 'w') as f:
        f.writelines(defstring)


if __name__ == "__main__":
    main()
