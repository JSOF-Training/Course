all:
	gcc -w -c main.c -o test_your_might.o
	gcc test_your_might.o -o test_your_might -I ~/../usr/include/openssl -lssl -lcrypto

clean:
	rm test_your_might.o test_your_might
