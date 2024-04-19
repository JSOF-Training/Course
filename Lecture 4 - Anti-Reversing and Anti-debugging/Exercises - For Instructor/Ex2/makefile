CC := gcc
MAKE := make
OBJCOPY:= objcopy
FLAGS = -Wall -Werror -O1 -mmanual-endbr -nostdlib -falign-functions=8 -fno-toplevel-reorder -fno-stack-protector -U_FORTIFY_SOURCE
DBGFLAGS = -g

.PHONY: all
all: onion onion_dbg

onion: raw_onion post.py funcs.py
	./post.py $@ $<
	strip -s $@

onion_dbg: raw_onion_dbg post.py funcs.py
	./post.py $@ $<

raw_onion_dbg: onion.c strdefs.h bitutils.h
	$(CC) $(FLAGS) $(DBGFLAGS) $< -o $@

raw_onion: onion.c strdefs.h bitutils.h
	$(CC) $(FLAGS)  $< -o $@

strdefs.h: pre.py funcs.py
	./pre.py $@
