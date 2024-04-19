#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <string.h>


#ifndef CHALLENGE_LEVEL
#error CHALLENGE_LEVEL undefined. Define CHALLENGE_LEVEL=0, 1 or 2 in compilation flags
#endif

#include "precalculated.h"

static inline int check_response(char * response);

#if (0 == CHALLENGE_LEVEL)
char password[] = PASSWORD_0;
#elif (1 == CHALLENGE_LEVEL)
char herring[] = HERRING_1;
char decoder[] = LEVEL_1_BYTES;
#elif (2 == CHALLENGE_LEVEL)
char final_string[] = LEVEL_2_FINAL;
char initial_bytes[] = LEVEL_2_BYTES;
#endif

#define LEVEL_FMT ("I am troll number %d\n")
#define PASSWORD_PROMPT ("Give me the password to cross the bridge:\n")
#define INERR_RESPONSE ("Input error.\n")
#define BAD_RESPONSE ("You shall not pass.\n")
#define GOOD_RESPONSE ("Correct! You may pass!\n")

char passbuff[0x100] = "";



int main(int argc, char **argv)
{
  memset(passbuff, 0, sizeof(passbuff));
  printf(LEVEL_FMT, CHALLENGE_LEVEL);
  printf(PASSWORD_PROMPT);
  char * ret = fgets(passbuff, sizeof(passbuff), stdin);
  if (ret != passbuff) {
    printf(INERR_RESPONSE);
    return -1;
  }
  // Set trailing newline to null
  size_t last = strlen(passbuff) - 1;
  if ('\n' == passbuff[last])
    passbuff[last] = '\0';

  if (check_response(passbuff))
    printf(GOOD_RESPONSE);
  else
    printf(BAD_RESPONSE);

  return 0;

}

#if (0 == CHALLENGE_LEVEL)
static inline int check_response(char * response)
{
  if (strcmp(response, password))
    return 0;
  return 1;
}
#elif (1 == CHALLENGE_LEVEL)

static inline int check_response(char * response)
{
  size_t length = sizeof(decoder);
  for (int i=0; i<length; i++)
  {
    herring[i] = herring[i] ^ decoder[i] ^ 0x55;
  }
  if (strcmp(response, herring))
    return 0;
  return 1;
}

#elif (2 == CHALLENGE_LEVEL)
static inline int check_response(char * response)
{
  size_t full_length = sizeof(final_string);
  size_t length = full_length - 1;
  for (int i=0; i<length; i++)
  {
    response[i] ^= initial_bytes[i] ^ initial_bytes[(i + 3) % length] ^ 0xAA;
  }
  if (memcmp(response, final_string, full_length))
    return 0;
  return 1;
}
#endif
