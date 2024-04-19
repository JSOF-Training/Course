CC := gcc
MAKE := make


FLAGS = -Wall -Werror -O2 -s -fvisibility=hidden -fno-stack-protector -U_FORTIFY_SOURCE

.PHONY: all
all: dragon solver period_tester

dragon: challenge.c bitutils.h
	$(CC) $(FLAGS)  $< -o $@

solver: solver.c bitutils.h
	$(CC) $(FLAGS)  $< -o $@

period_tester: period_tester.c bitutils.h
		$(CC) $(FLAGS)  $< -o $@
