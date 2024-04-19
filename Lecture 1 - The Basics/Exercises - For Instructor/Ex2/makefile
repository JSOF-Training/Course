CC := gcc
MAKE := make


FLAGS = -Wall -Werror -O2 -s -fvisibility=hidden -fno-pie -no-pie -fno-stack-protector -U_FORTIFY_SOURCE

.PHONY: all
all: bubblesort myfirstfunction

bubblesort: bubblesort.c
	$(CC) $(FLAGS)  $< -o $@
	
myfirstfunction: naked_bs.c
	$(CC) $(FLAGS)  $< -o $@
