#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#include "precalculated.h"

int get_str(char * prompt, char * buf, size_t len);
static inline int check_response(char * response);

#define NAME_PROMPT ("What is your name?\n")
#define NAME_SHORT ("Your name is too short.\n")
#define QUEST_PROMPT ("What is your quest?\n")
#define QUEST_SHORT ("That's not a meaningful quest.")
#define CHALLENGE_PROMPT ("What is your favorite colour?\n")

char color_names[COLOR_LEN][COLOR_SIZE] = ENCODED_COLORS;

#define INERR_RESPONSE ("Input error.\n")
#define BAD_RESPONSE ("Aaaaaaaaaaaaaaaaaaaaaaaaaaah.\n")
#define GOOD_FMT ("Alright %s, you may proceed to %s.\n")

char name[0x100] = "";
char quest[0x100] = "";
char response[0x100] = "";

int main(int argc, char **argv)
{
  if (get_str(NAME_PROMPT, name, sizeof(name)))
    return -1;
  if (strlen(name) < 4)
  {
    printf(NAME_SHORT);
    return 0;
  }
  if (get_str(QUEST_PROMPT, quest, sizeof(quest)))
    return -1;
  if (strlen(quest) < 4)
  {
    printf(QUEST_SHORT);
    return 0;
  }


  if (get_str(CHALLENGE_PROMPT, response, sizeof(response)))
    return -1;

  if (check_response(response))
  {
    printf(GOOD_FMT, name, quest);
  }
  else
  {
    printf(BAD_RESPONSE);
  }
  return 0;
}

int get_str(char * prompt, char * buf, size_t len)
{
  size_t last = 0;
  char * ret = NULL;
  printf("%s", prompt);
  ret = fgets(buf, len, stdin);
  if (ret != buf) {
    printf(INERR_RESPONSE);
    return -1;
  }
  last = strlen(buf) - 1;
  if ('\n' == buf[last])
    buf[last] = '\0';
  return 0;
}

__attribute__((always_inline)) static inline int check_response(char * response)
{
  char color[COLOR_SIZE];
  unsigned char color_index = 0x55;
  char magic_byte = 0;

  size_t i = 0;
  for(i=0; i<0x100; i++)
  {
    if (0 == name[i])
      break;
    color_index = (color_index + name[i]) & 0x7F;
    color_index = (color_index << 3 | color_index >> 4) & 0x7F;
  }
  for(i=0; i<0x100; i++)
  {
    if (0 == quest[i])
      break;
    color_index = (color_index + quest[i]) & 0x7F;
    color_index = (color_index << 4 | color_index >> 3) & 0x7F;
  }
  for(i=0; i<4; i++)
  {
    color_index = (color_index + 0x55) & 0x7F;
    color_index = (color_index << 3 | color_index >> 4) & 0x7F;
  }

  color_index = color_index % COLOR_LEN;
  memcpy(color, color_names + color_index, COLOR_SIZE);
  magic_byte = (7 * color_index + 0x33) ^ 0xCC;
  // the encoding in python is:
  // [((b + 111 * (magic_byte + i + 1)) % 0x100) for i, b in enumerate(cbytes)]
  for (i=0; i<COLOR_SIZE; i++)
  {
    color[i] -= (111 * (magic_byte + i)) % 0x100;
  }
  return (0 == strcmp(color, response));
}
