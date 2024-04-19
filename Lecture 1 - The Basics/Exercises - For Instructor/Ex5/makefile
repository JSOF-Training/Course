CC := gcc
MAKE := make
PYTHON := python3

FLAGS = -Wall -Werror -s -fvisibility=hidden -fno-stack-protector -U_FORTIFY_SOURCE

.PHONY: all
all: hop

hop: hop.c
	$(CC) $(FLAGS) $< -o $@
