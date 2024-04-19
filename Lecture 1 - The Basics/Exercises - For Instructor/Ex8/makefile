CC := gcc
MAKE := make
RM := rm
PYTHON := python3

NODEBUG := -s -fvisibility=hidden
LIBS := -I ~/../usr/include/openssl -lssl -lcrypto

.PHONY: all
all: might

test_your_might: main.c
	$(CC) -w -c $< -o test_your_might.o $(NODEBUG)
	$(CC) test_your_might.o -o $@ $(LIBS) $(NODEBUG)

checksummer: checksummer.c
	$(CC) -w -c $< -o checksummer.o $(NODEBUG)
	$(CC) checksummer.o -o $@ $(LIBS) $(NODEBUG)

might: patcher.py test_your_might checksummer
	$(PYTHON) patcher.py test_your_might $@

clean_temp:
	$(RM) *.o

clean: clean_temp
	$(RM) test_your_might checksummer might
