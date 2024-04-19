#! /usr/bin/env bash

git clone https://github.com/openssh/openssh-portable/
cd openssh-portable/

# If the future compiled file is optimized so that function names are NOT
#   even passed to sshlogv and derivatives when showfunc=False,
#   you can try the following,
# But first you can skip changing the code and check the output in IDA/Ghidra
# Try either:
# open log.h and change 0 to 1 in lines that look like:
#    #define do_log2(level, ...)   sshlog(__FILE__, __func__, __LINE__, 0, level, NULL, __VA_ARGS__)
#                                                        This zero here ^
# Or:
# Open log.c and force 	showfunc = 1; in the sshlogv function
# Maybe there's a better and more efficient way to force logging function names, I didn't look into the code too much
# The former will probably produce more consistent code (less likely to confuse students if they reverse the actual function)
# since they'll actually be calling the function with showfunc=True rather than not using the argument inside the function

# You may need to install autoconf (sudo apt-get install autoconf) for the next one:
# sudo apt-get install autoconf
autoreconf
./configure
make && make tests
cp ./ssh ../../something
strip -s ../../something

# Open in IDA/Ghidra and make sure when calling sshlogv and derivatives
#    filenames, function names and line numbers are still passed to the 
#    function even if showfunc=0