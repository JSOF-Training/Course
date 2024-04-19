CC := gcc
MAKE := make
OBJCOPY:= objcopy
FLAGS = -Iinc -Wall -Werror -fno-stack-protector -U_FORTIFY_SOURCE


.PHONY: all
all: bizarro

bizarro: bizarro_raw post.py
	./post.py $@ $<
	strip -s $@
strdefs.h: stringmaker.py
	./stringmaker.py $@
bizarro_raw: bizarro.c strdefs.h
	$(CC) $(FLAGS)  $< -o $@
