CC := gcc
MAKE := make

# Remove symbols
FLAGS_STRIP = -s -fvisibility=hidden
# Keep symbols and add debugging information 
FLAGS_DBG = -g
# Compile for 32-bit targets
FLAGS_32 = -m32
# Compile for 64-bit targets
FLAGS_64 = -m64

# All flags after -Wall and -Werror are there to strip the functions of optimizations and
#   security protections, which we'll get to later in the course.
# This way the compiled functions are simpler.
FLAGS = -Wall -Werror -no-pie -fno-pie -fno-stack-protector -U_FORTIFY_SOURCE

FINAL_TARGETS = functions32_dbg functions64_dbg functions32 functions64


.PHONY: all
all: $(FINAL_TARGETS)

functions32_dbg: functions.c
		$(CC) $(FLAGS) $(FLAGS_32) $(FLAGS_DBG)   $< -o functions32_dbg

functions64_dbg: functions.c
		$(CC) $(FLAGS) $(FLAGS_64) $(FLAGS_DBG)   $< -o functions64_dbg

functions32: functions.c
		$(CC) $(FLAGS) $(FLAGS_32) $(FLAGS_STRIP)  $< -o functions32

functions64: functions.c
		$(CC) $(FLAGS) $(FLAGS_64) $(FLAGS_STRIP) $< -o functions64
		
.PHONY: clean
clean:
	-rm -f $(FINAL_TARGETS)

