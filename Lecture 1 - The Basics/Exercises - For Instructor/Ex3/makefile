CC := gcc
MAKE := make

FLAGS = -Wall -Werror -O0 -fno-stack-protector -fno-pie -no-pie -m32 -s -fvisibility=hidden -U_FORTIFY_SOURCE

.PHONY: all
all: mystery

mystery: mystery.c 
	$(CC) $(FLAGS) $< -o $@