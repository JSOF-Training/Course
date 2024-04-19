#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#include "bitutils.h"
uint64_t solve(uint64_t challenge);

#define PATTERN_A (0xaa55deadbeefaa55)
#define PATTERN_B (0xdecafc0ffee15bad)
#define PATTERN_C (0xbadc0ffeebadc0d3)
#define PATTERN_D (0xbada55acedec0ded)


int main(int argc, char **argv)
{
  uint64_t challenge, response;
  printf("Enter challenge: ");
  if (1 != scanf("%lx", &challenge))
  {
    printf("Input error\n");
    return -1;
  }
  response = solve(challenge);
  printf("Correct answer is %016lx\n", response);
}

uint64_t solve(uint64_t challenge)
{
  uint64_t temp, seed, response;
  temp = rolln64(challenge, PATTERN_B, 0x52);
  temp = rotl64(temp, 57+16) ^ rotl64(temp, 57-16) ^ rotl64(temp, 57);
  seed = unrolln64(temp, PATTERN_A, 0x43);
  #ifdef DEBUG
    printf("The seed is %016lx\n", seed);
  #endif
  temp = rolln64(seed, PATTERN_D, 0x42);
  temp = rotl64(temp, 2) ^ rotl64(temp, 2+16) ^ rotl64(temp, 2-16);
  response = unrolln64(temp, PATTERN_C, 0x41);

  return response;
}
