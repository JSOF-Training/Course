#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <limits.h>

#ifndef PAGESIZE
#define PAGESIZE (4096)
#endif

#include "strdefs.h"

#define PASS_SIZE (sizeof(video_id) - 1)
void fix1(void);
void fix2(void);

int check(char * password);
int main(int argc, char *argv[]);

#define start  ((char*)check + 0x10)
#define end  ((char*)main)

//                 wvfR3XLXPvw
//                 kDgvnbUIqT4
//                 notrickroll
char video_id[] = {0x5, 0x2b, 0x13, 0x4, 0x7, 0x1, 0x3e, 0x3b, 0x1e, 0x38, 0x58, 0x00};
//

__attribute__ ((noinline)) int check(char * password)
{
  int test1 = 0;
  int test2 = 0;
  int test3 = 0;

  char passcopy[PASS_SIZE + 1] = {0};
  if (strlen(password) != PASS_SIZE) {
    return 0;
  }
  memcpy(passcopy, password, PASS_SIZE + 1);

  for (int i=0; i<PASS_SIZE; i++) {
    passcopy[i] ^= 0x20;
    passcopy[i] += 1;
  }
  if ((passcopy[0] != 'O') || (passcopy[7] != 'S')) {
    return 0;
  }
  char temp = passcopy[4];
  passcopy[4] = passcopy[6];
  passcopy[6] = temp;

  if (passcopy[2] - passcopy[1] != 5) {
    test1 = 0;
  } else {
    test1 = 1;
  }

  if ((passcopy[2] - passcopy[3]) != (passcopy[0] - passcopy[10])) {
    return 0;
  }

  if (passcopy[1] - passcopy[0] != 1) {
    test1 = 0;
  }

  if ((passcopy[1] == passcopy[8]) &&
      (passcopy[9] == passcopy[10]) &&
      (passcopy[7] == passcopy[3])) {
    test2 = 1;
  } else {
    test2 = 0;
  }
  test3 = ((passcopy[4] == 'L') && (passcopy[4] != 'C') &&
           (passcopy[5] == 'D') && (passcopy[5] != 'R') &&
           (passcopy[6] == 'J') && (passcopy[5] != 'B') );


  return test1 && test2 && test3;
}

int main(int argc, char *argv[])
{
  fix2();
  if (argc != 2) {
    puts(usage_str);
    return 0;
  }
  if(check(argv[1]))
  {
    puts(congrats_str);
    for (int i=0; i<PASS_SIZE; i++) {
      video_id[i] ^= argv[1][i];
    }

    printf(reward_fmt, video_id);
  }
  else {
    puts(sorry_str);
  }

}

int my_mprotect(void * page, size_t length, unsigned long prot);
__asm__(
  "my_mprotect: \n"
    "movq $10, %rax\n"
    "syscall\n"
    "ret\n"
  );

void fix2(void)
{
  for (char * b = start; b < end; b++) {
    if ((*b & 0xF0) == 0x70) {
      *b ^= 0x01;
    }
    else if ((*b == 0x0F) && ((*(b+1) & 0xF0) == 0x80)) {
      *(b+1) ^= 0x01;
    }
  }
  for (int i = 0; i<sizeof(usage_str); i++) {
    usage_str[i] ^= 0xA5;
  }
  for (int i = 0; i<sizeof(congrats_str); i++) {
    congrats_str[i] ^= 0xA5;
  }
  for (int i = 0; i<sizeof(sorry_str); i++) {
    sorry_str[i] ^= 0xA5;
  }
  for (int i = 0; i<sizeof(reward_fmt); i++) {
    reward_fmt[i] ^= 0xA5;
  }
}

__attribute__((constructor)) void fix1(void)
{
  void* page = (void*)((unsigned long) start - (unsigned long) start % PAGESIZE);
  int ret = my_mprotect(page, PAGESIZE,
                     PROT_READ | PROT_WRITE | PROT_EXEC);
  if(ret)
  {
    exit(-10);
  }
  for (char * b = start; b < end; b++) {
    if ((*b == 0x3C) && ((*(b+1) & 0xE0) == 0x60)) {
      *(b+1) ^= 0x25;
    }
  }
  usleep(10);

}
