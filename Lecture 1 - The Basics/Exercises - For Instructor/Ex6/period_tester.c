#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#include "bitutils.h"
uint64_t test_period(uint64_t iv, uint64_t pattern, uint64_t give_up);

#define PATTERN_A (0xaa55deadbeefaa55)
#define PATTERN_B (0xdecafc0ffee15bad)
#define PATTERN_C (0xbadc0ffeebadc0d3)
#define PATTERN_D (0xbada55acedec0ded)


int main(int argc, char **argv)
{
  uint64_t patterns[] = {PATTERN_A, PATTERN_B, PATTERN_C, PATTERN_D};
  uint64_t period = 0;
  for (int i=0; i<4; ++i)
  {
    period = test_period(-1, patterns[i], (uint64_t)1 << 32);
    printf("For pattern=0x%016lx, iv=0x%016lx period is 0x%016lx\n",
      patterns[i], (uint64_t)1, period);
  }
  return 0;
}

uint64_t test_period(uint64_t iv, uint64_t pattern, uint64_t give_up)
{
  uint64_t state = iv;
  uint64_t count = 0;
  do {
    state = roll64(state, pattern);
    count++;
  } while ((iv != state) && (count < give_up));

  return count;
}
