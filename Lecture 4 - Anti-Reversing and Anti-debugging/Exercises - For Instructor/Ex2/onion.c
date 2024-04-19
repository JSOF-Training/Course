#include <sys/syscall.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/mman.h>
#include <limits.h>
#ifndef PAGESIZE
#define PAGESIZE (4096)
#endif


#define READ_BUF_SIZE (0x1000)

#define PAGE_ROUND_DOWN(x) (((uint64_t)(x)) & (~(PAGESIZE-1)))
#define PAGE_ROUND_UP(x) ( (((uint64_t)(x)) + PAGESIZE-1)  & (~(PAGESIZE-1)) )

#include <stdlib.h>



#define STRINGIZE1(x) #x
#define STRINGIZE(x) STRINGIZE1(x)

#define CODE_LABEL(n) __asm__("	   .text\t\n" \
"    .globl "STRINGIZE(n)"\n" \
"    .align 8\n" \
STRINGIZE (n) ":\n");

#define SYSCALL_ASM(n, c) __asm__(\
  ".text\n" \
  ".align 8\n" \
  STRINGIZE(n) ":\n" \
    "movq $" STRINGIZE(c) ", %rax\n" \
    "syscall\n" \
    ".globl " STRINGIZE(n) "_e\n" \
    ".align 8\n" \
    STRINGIZE(n) "_e:\n" \
    "ret\n ");
char dbg[] = "0\n";
void *code_start(void);
CODE_LABEL(code_start)
// void returning, one time use
#define CALL_FUNC_VO(f,m,...) unscramble_func(f,f ## _e, m); \
f (__VA_ARGS__); \
unscramble_func(f, f ## _e, BURN_MAGIC);

// get return value, one time use
#define CALL_FUNC_RO(f,m,r,...) unscramble_func(f,f ## _e, m); \
r = f (__VA_ARGS__); \
unscramble_func(f, f ## _e, BURN_MAGIC);

// void returning, multiple use
#define CALL_FUNC_VM(f,m,...) unscramble_func(f,f ## _e, m); \
f (__VA_ARGS__); \
unscramble_func(f, f ## _e, m);

// get return value, multiple use
#define CALL_FUNC_RM(f,m,r,...) unscramble_func(f,f ## _e, m); \
r = f (__VA_ARGS__); \
unscramble_func(f, f ## _e, m);



#include "bitutils.h"
#include "strdefs.h"



char atlantic_herring[] = ATLANTIC_HERRING;
char pacific_herring[] = PACIFIC_HERRING;
char input_error_str[] = INPUT_ERROR_STR;
char start_str[] = START_STR;
char nope_str[] = NOPE_STR;
char second_input_error_str[] = SECOND_INPUT_ERROR_STR;
char second_start_str[] = SECOND_START_STR;
char reward_str[] = REWARD_STR;


char inputbuff[READ_BUF_SIZE] = {0};

void patch_jump();

void * get_rip();
void my_exit(int ret);
int my_mprotect(void *addr, size_t len, int prot);
ssize_t my_read(int fd, void *buf, size_t count);
ssize_t my_write(int fd, const void *buf, size_t count);
void writable_text(void);
void _start(void);
void unscramble_code(void * start, void * end, uint64_t key);
char *my_getstr(char *s, size_t size);
char *getstr_and_patch(char *s, size_t size);

void unscramble_func(void * start, void * end, uint64_t key);
void xor_string(char * string, unsigned char magic);
int my_puts(const char * str);
void actual_code(void);
int real_check_pass(char * password, uint64_t true0, uint64_t true1, uint64_t true2, uint64_t true3, uint64_t xormagic);
int fake_check_pass(char * password, uint64_t true0, uint64_t true1, uint64_t true2, uint64_t true3, uint64_t xormagic);
size_t my_strlen(const char* str);
int red_herring(const char *);
void *layer1_start(void);
void *layer1_end(void);
void *code_end(void);

__asm__(
  ".text\n"
  "get_rip:\n"
    "movq (%rsp), %rax\n"
    "ret\n"
    //noreturn
  );

SYSCALL_ASM(my_exit, SYS_exit)
SYSCALL_ASM(my_mprotect, SYS_mprotect)



void writable_text(void)
{
  void* page = (void *)PAGE_ROUND_DOWN(code_start);
  size_t length = (size_t)PAGE_ROUND_UP(code_end - code_start);
  int ret = my_mprotect(page, length,
                     PROT_READ | PROT_WRITE | PROT_EXEC);
  if(ret)
  {
    my_exit(-1);
  }
}

__attribute__ ((noinline))
void unscramble_code(void * start, void * end, uint64_t key)
{

  for (uint32_t *p = (uint32_t *) start; p < (uint32_t *) end; p++)
  {
    *p ^= key;
  }
  CALL_FUNC_VO(actual_code, ACTUAL_CODE_MAGIC)

}

__attribute__((naked))
void _start(void)
{
  char * ret = NULL;
  writable_text();

  unscramble_code((void *) layer1_start, (void*) layer1_end, LAYER1_MAGIC);
  CODE_LABEL(layer1_start)
  xor_string(start_str, START_STR_MAGIC);
  my_puts(start_str);
  xor_string(start_str, START_STR_MAGIC);

  ret = my_getstr(inputbuff, READ_BUF_SIZE);
  if(!ret)
  {
    xor_string(input_error_str, INPUT_ERROR_STR_MAGIC);
    my_puts(input_error_str);
    xor_string(input_error_str, INPUT_ERROR_STR_MAGIC);
  }
  if (ret && red_herring(inputbuff))
  {
    xor_string(atlantic_herring, ATLANTIC_HERRING_MAGIC);
    my_puts(atlantic_herring);
    xor_string(atlantic_herring, PACIFIC_HERRING_MAGIC);
  }
  else
  {
    xor_string(pacific_herring, ATLANTIC_HERRING_MAGIC);
    my_puts(pacific_herring);
    xor_string(pacific_herring, PACIFIC_HERRING_MAGIC);
  }
  my_exit(0);
}
CODE_LABEL(_start_e)

__attribute__ ((noinline))
void unscramble_func(void * start, void * end, uint64_t key)
{
  uint64_t buffer = key;
  for (uint64_t *p = (uint64_t *) start; p < (uint64_t *) end; p++)
  {
    buffer = roll64(buffer, FUNC_PATTERN);
    *p ^= buffer;
  }
}
CODE_LABEL(unscramble_func_e);

SYSCALL_ASM(my_read, SYS_read)

SYSCALL_ASM(my_write, SYS_write)

int my_puts(const char * str)
{
  size_t length = 0, ret = 0;
  length = my_strlen(str);
  ret = my_write(STDOUT_FILENO, str, length);
  if (length != ret)
  {
    my_exit(-1);
  }
  return 0;
}
CODE_LABEL(my_puts_e);
__attribute__((used))
char *my_getstr(char *s, size_t size)
{
  ssize_t ret = 0;
  for(size_t i = 0; i < size; i+=ret)
  {
    ret = my_read(STDIN_FILENO, s + i, 1);
    if (ret < 0)
    {
      my_exit(-1);
    }
    if ((s[i] == '\n') || (s[i] == '\0'))
    {
      s[i] = '\0';
      return s;
    }
  }
  return NULL;
}
CODE_LABEL(my_getstr_e);


__attribute__ ((noinline))
size_t my_strlen(const char* str)
{
    for(size_t i = 0; ; i++)
     {
      if (str[i] == 0x00)
      {
        return i;
      }
    }
}
CODE_LABEL(my_strlen_e);

__attribute__((noinline))
void xor_string(char * string, unsigned char magic)
{
  for(int i=0; i<MAX_STR_LENGTH; i++)
  {
    string[i] ^= (0x55 + i * magic);
  }
}
CODE_LABEL(xor_string_e)

CODE_LABEL(red_herring)
__attribute__((used,section(".text#"))) // # suppresses assembly shenanigans
char randbytes[] = RANDOM_RED_HERRING;

CODE_LABEL(red_herring_e)

__attribute__((noinline))
void scramble_string(char * string, unsigned int count)
{
  uint64_t *nums = (uint64_t *)string;
  for(int i=0; i<MAX_STR_LENGTH / sizeof(uint64_t); i++)
  {
    nums[i] = rolln64(nums[i] + i, STR_PATTERN, count);
  }
}
CODE_LABEL(scramble_string_e)

__attribute__((noinline))
void unscramble_string(char * string, unsigned int count)
{
  uint64_t *nums = (uint64_t *)string;
  for(int i=0; i<MAX_STR_LENGTH / sizeof(uint64_t); i++)
  {
    nums[i] = unrolln64(nums[i], STR_PATTERN, count) - i;
  }
}
CODE_LABEL(unscramble_string_e)

char *getstr_and_patch(char *s, size_t size)
{
  ssize_t ret = 0;
  for(size_t i = 0; i < size; i+=ret)
  {
    ret = my_read(STDIN_FILENO, s + i, 1);
    if (ret < 0)
    {
      my_exit(-1);
    }
    if ((s[i] == '\n') || (s[i] == '\0'))
    {
      s[i] = '\0';
      CALL_FUNC_VO(patch_jump, PATCH_JUMP_MAGIC)
      return s;
    }
  }
  return NULL;
}
CODE_LABEL(getstr_and_patch_e);

void before_check_pass(void);
__attribute__ ((noinline))
void actual_code()
{
    char password[READ_BUF_SIZE] = {0};
    char * gret = NULL;
    int ret = 0;
    CALL_FUNC_VM(unscramble_string, UNSCRAMBLE_STRING_MAGIC, second_start_str, SECOND_START_STR_MAGIC)
    my_puts(second_start_str);
    CALL_FUNC_VM(scramble_string, SCRAMBLE_STRING_MAGIC, second_start_str, SECOND_START_STR_MAGIC)

    CALL_FUNC_RO(getstr_and_patch, GETSTR_AND_PATCH_MAGIC, gret, password, READ_BUF_SIZE);
    if(!gret) {
      CALL_FUNC_VM(unscramble_string, UNSCRAMBLE_STRING_MAGIC, second_input_error_str, SECOND_INPUT_ERROR_STR_MAGIC)
      my_puts(second_input_error_str);
      CALL_FUNC_VO(scramble_string, SCRAMBLE_STRING_MAGIC, second_input_error_str, SECOND_INPUT_ERROR_STR_MAGIC)
      my_exit(0);
    }
    unscramble_func(fake_check_pass, fake_check_pass_e, FAKE_CHECK_PASS_MAGIC);
    __asm__("\nbefore_check_pass:\n");
    ret = fake_check_pass(password,
      PASS1_INT0, PASS1_INT1, PASS1_INT2, PASS1_INT3, PASS_XORMAGIC);
    unscramble_func(fake_check_pass, fake_check_pass_e, BURN_MAGIC);
    if (ret==0)
    {
      CALL_FUNC_VO(unscramble_string, UNSCRAMBLE_STRING_MAGIC, nope_str, NOPE_STR_MAGIC)
      my_puts(nope_str);
      CALL_FUNC_VO(scramble_string, SCRAMBLE_STRING_MAGIC, nope_str, NOPE_STR_MAGIC)
    } else {
      CALL_FUNC_VO(unscramble_string, UNSCRAMBLE_STRING_MAGIC, reward_str, REWARD_STR_MAGIC)
      my_puts(reward_str);
      CALL_FUNC_VO(scramble_string, SCRAMBLE_STRING_MAGIC, reward_str, REWARD_STR_MAGIC)
    }
    my_exit(0);
    return;
}
CODE_LABEL(actual_code_e);



// Change call from fake_check_pass to real_check_pass
// Change %r9 (last param) from PASS_XORMAGIC to (PASS_XORMAGIC ^ PASS_OBFMAGIC)
#define FAKE_TO_REAL_CALL_OPERAND_OFFSET (0x36)
#define FAKE_TO_REAL_XOR_LITERAL_OFFSET (2)
#define OBFUSCATION_OFFSET -0x45252123 //=0xbadadedd = 0xbadadd12 + 0x1cb
void location_reference(void);

__attribute__((noinline))
void patch_jump()
{
  int32_t callptr_diff;
//  void * xor_op_offset;
  void * call_op_offset;
  int32_t func_diff;

  // obfuscate the address:
  __asm__(
     "location_reference:\n"
     "movl $(before_check_pass - location_reference +"STRINGIZE(OBFUSCATION_OFFSET)"), %0\n"
     : "=r" (callptr_diff) : :
   );
/*
   __asm__(
      "movl $(before_check_pass - location_reference +"STRINGIZE(OBFUSCATION_OFFSET)"), %0\n"
      : "=r" (xor_op_offset) : :
    );
    */
  unscramble_func(real_check_pass, real_check_pass_e, REAL_CHECK_PASS_MAGIC);
/*
  xor_op_offset = (void *)(location_reference + callptr_diff
    - OBFUSCATION_OFFSET + FAKE_TO_REAL_XOR_LITERAL_OFFSET);
*/

  call_op_offset = (void *)(location_reference + callptr_diff
     - OBFUSCATION_OFFSET + FAKE_TO_REAL_CALL_OPERAND_OFFSET);


  // callptr = (void *)(location_reference + callptr_diff - OBFUSCATION_OFFSET);

  // This part is needed so that the diff between the functions will
  // be calculated at compile-time
  __asm__(
     "movl $(real_check_pass - fake_check_pass), %0\n"
     : "=r" (func_diff) : :
   );
  //*((int64_t *)(xor_op_offset)) = (PASS_XORMAGIC);
  *((int32_t *)(call_op_offset)) += func_diff;

}
CODE_LABEL(patch_jump_e)


__attribute__((naked))
int real_check_pass(char * password, uint64_t true0, uint64_t true1, uint64_t true2, uint64_t true3, uint64_t xormagic);
/*
"    .text\n"
         "	.type	real_check_pass, @function\n"
         "  .globl real_check_pass\n"
         "real_check_pass: \n"
         */
__asm__ (
         ".globl real_check_pass\n"
         "real_check_pass:\n"
         "  movq $" STRINGIZE(PASS_XORDIFF0) ", %rax \n"
         "  xorq %rax, %rsi \n"
         "  movq $" STRINGIZE(PASS_XORDIFF1) ", %rax \n"
         "  xorq %rax, %rdx \n"
         "  movq $" STRINGIZE(PASS_XORDIFF2) ", %rax \n"
         "  xorq %rax, %rcx \n"
         "  movq $" STRINGIZE(PASS_XORDIFF3) ", %rax \n"
         "  xorq %rax, %r8 \n"
         "  movq $" STRINGIZE(PASS_OBFMAGIC) ", %rax \n"
         "  xorq %rax, %r9 \n"
       );

CODE_LABEL(real_check_pass_e)

__attribute__ ((noinline))
int fake_check_pass(char * password, uint64_t true0, uint64_t true1, uint64_t true2, uint64_t true3, uint64_t xormagic)
{
  uint64_t *password_ints = (uint64_t *) password;
  if(password_ints[0] == (true0 ^ xormagic) &&
     password_ints[1] == (true1 ^ xormagic) &&
     password_ints[2] == (true2 ^ xormagic) &&
     password_ints[3] == (true3 ^ xormagic)) {
       return 1;
     }
  return 0;
}
CODE_LABEL(fake_check_pass_e)


CODE_LABEL(layer1_end)

CODE_LABEL(code_end)
