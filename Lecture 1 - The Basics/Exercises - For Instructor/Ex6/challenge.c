#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
// #include <time.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>


#include "bitutils.h"

#define PATTERN_A (0xaa55deadbeefaa55)
#define PATTERN_B (0xdecafc0ffee15bad)
#define PATTERN_C (0xbadc0ffeebadc0d3)
#define PATTERN_D (0xbada55acedec0ded)


uint64_t get_challenge(uint64_t seed);
uint64_t check_response(uint64_t seed, uint64_t response);
int rand_64(uint64_t * randnum);

int main(int argc, char **argv)
{
  uint64_t seed, challenge, response;
  // srand((unsigned int)time(NULL));
  if(!rand_64(&seed))
  {
    printf("Error reading from /dev/urandom\n");
    return -1;
  }
#ifdef DEBUG
  printf("The seed is %016lx\n", seed);
#endif
  challenge = get_challenge(seed);
  printf("Answer my challenge correctly to get to the treasure.\n");
  printf("Your challenge is %016lx\n", challenge);
  printf("Your answer: ");
  if (1 != scanf("%lx", &response))
  {
    printf("Input error\n");
    return -1;
  }

  if (check_response(seed, response))
    printf("Success! You may claim your treasure.\n");
  else
    printf("The dragon is angry.\n");

  return 0;

}

uint64_t get_challenge(uint64_t seed)
{
  uint64_t temp = rolln64(seed, PATTERN_A, 0x43);
  temp = rotr64(temp, 57+16) ^ rotr64(temp, 57-16) ^ rotr64(temp, 57);
  return unrolln64(temp, PATTERN_B, 0x52);
}

uint64_t check_response(uint64_t seed, uint64_t response)
{
  uint64_t temp = rolln64(response, PATTERN_C, 0x41);
  temp = rotr64(temp, 2) ^ rotr64(temp, 2+16) ^ rotr64(temp, 2-16);
  temp = unrolln64(temp, PATTERN_D, 0x42);
  return (temp == seed);
}

int rand_64(uint64_t * randnum)
{
  int randfile = open("/dev/urandom", O_RDONLY);
  if (randfile < 0)
  {
      return 0;
  }
  else
  {
      ssize_t result = read(randfile, randnum,  sizeof(*randnum));
      if (result < 0)
      {
          return 0;
      }
  }
  return 1;
/*
  uint64_t a, b, c;
  a = (uint64_t) rand();
  b = (uint64_t) rand();
  c = (uint64_t) rand();
  *randnum = (a << 62) | (b << 31) | c;
  return 1;
  */
}
