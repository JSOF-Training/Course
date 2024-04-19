CC := gcc
AS := nasm
LD := ld
MAKE := make

CFLAGS = -Wall -Werror
CFLAGS_32 = -m32
CFLAGS_64 = -m64
NOLINK = -c

ASFLAGS =
ASFLAGS_32 = -f elf32
ASFLAGS_64 = -f elf64
ASNAKED = -f bin

FINAL_TARGETS = helloworld anotherexample.o asm32 asm64 naked32 naked64
TEMP_TARGETS = helloworld.o asm64.o main64.o asm32.o main32.o

.PHONY: all
all: $(FINAL_TARGETS)

helloworld.o: helloworld.asm
	$(AS) $(ASFLAGS) $(ASFLAGS_64)  $< -o $@

helloworld: helloworld.o
	$(LD) $(ASFLAGS) $< -o $@

# No C wrapper for this one
anotherexample.o: anotherexample.asm
	$(AS) $(ASFLAGS) $(ASNAKED)  $< -o $@


naked32: naked32.asm
	$(AS) $(ASFLAGS) $(ASNAKED)  $< -o $@

naked64: naked64.asm
	$(AS) $(ASFLAGS) $(ASNAKED)  $< -o $@

asm64.o: asm64.asm
	$(AS) $(ASFLAGS) $(ASFLAGS_64)  $< -o $@

main64.o: main.c
	$(CC) $(CFLAGS) $(CFLAGS_64) $(NOLINK) $< -o $@

asm64: asm64.o main64.o
	$(CC) $(CFLAGS) $(CFLAGS_64) $^ -o $@

asm32.o: asm32.asm
	$(AS) $(ASFLAGS) $(ASFLAGS_32)  $< -o $@

main32.o: main.c
	$(CC) $(CFLAGS) $(CFLAGS_32) $(NOLINK)  $< -o $@

asm32: asm32.o main32.o
	$(CC) $(CFLAGS) $(CFLAGS_32) $^ -o $@

.PHONY: clean_temp
clean_temp:
	-rm -f $(TEMP_TARGETS)
	
.PHONY: clean
clean: clean_temp
	-rm -f $(FINAL_TARGETS)

