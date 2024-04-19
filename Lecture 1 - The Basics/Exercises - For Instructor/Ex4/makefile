CC := gcc
MAKE := make
PYTHON := python3


FLAGS = -Wall -Werror -s -fvisibility=hidden -fno-stack-protector -U_FORTIFY_SOURCE

.PHONY: all
all: troll0 troll1 troll2

troll0: troll.c precalculated.h
	$(CC) $(FLAGS) -DCHALLENGE_LEVEL=0  $< -o $@

troll1: troll.c precalculated.h
	$(CC) $(FLAGS) -DCHALLENGE_LEVEL=1  $< -o $@

troll2: troll.c precalculated.h
	$(CC) $(FLAGS) -DCHALLENGE_LEVEL=2  $< -o $@

precalculated.h: encode.py
	$(PYTHON) $^ $@
