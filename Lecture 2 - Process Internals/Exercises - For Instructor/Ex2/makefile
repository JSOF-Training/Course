CC := gcc
MAKE := make

FLAGS = -lz -Wall -Werror -O0 -fno-stack-protector -fno-pie -no-pie -m64

.PHONY: all
all: patchme

patchme: patchme.c
	$(CC) $< $(FLAGS) -o $@
