CC := gcc
MAKE := make
PYTHON := python3
FLAGS := -Wall -Werror -s -fvisibility=hidden -fno-stack-protector -U_FORTIFY_SOURCE

.PHONY: all
all: bridgeofdeath

bridgeofdeath: bridgeofdeath.c precalculated.h
	$(CC) $(FLAGS) $< -o $@

precalculated.h: encode.py
	$(PYTHON) $^ $@
