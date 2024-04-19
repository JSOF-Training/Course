#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>

#define VALIDATE_SUCCESS (0x42415243)
#define MAGIC_NUMBER (0x4d554e434947414d)
#define SANITY_LEN (0x80000)

int is_allzero(const char * buf, size_t len);
int check_file(const char * filename, uint32_t checksum);
int readfile(const char * path, void ** out_d, size_t *out_ld);
uint32_t calc_crc32(const unsigned char * bytes, size_t size);
int correct_validation_function();

const char padding[1012] = {0};
const uint64_t magic = 0;
const char more_padding[468] = {0};
int (*const validate)() = NULL;
const char even_more_padding[784] = {0};
const uint32_t checksum = 0;
const char yet_more_padding[508] = {0};



int main(int argc, char ** argv)
{
  if (!argc) {
    printf("Error: 0 arguments given\n");
    return -1;
  }

  char * filename = argv[0];

  if (  !(is_allzero(padding, sizeof(padding)) &&
          is_allzero(more_padding, sizeof(more_padding)) &&
          is_allzero(even_more_padding, sizeof(even_more_padding))  &&
          is_allzero(yet_more_padding, sizeof(yet_more_padding)))) {
    printf("Error: Unexpected nonzero bytes\n");
    return -1;
  }
  if (!check_file(filename, *((volatile uint32_t*) &checksum))) {
    printf("Error: Wrong checksum, file corrupted\n");
    return -1;
  }
  if (MAGIC_NUMBER !=  *((volatile uint64_t*) &magic)) {
    printf("Error: Bad voodoo magic\n");
    return -1;
  }
  if (NULL == *((volatile void **) &validate)) {
    printf("Error: No validation function\n");
    return -1;
  }
  if (VALIDATE_SUCCESS !=
      (*((volatile int ( ** )())
      &validate))()
    ){
    printf("Error: validation failed\n");
    return -1;
  }
  printf("Success: This file is valid\n");
  return 0;
}

__attribute__((used)) int correct_validation_function() {
  return VALIDATE_SUCCESS;
}


int is_allzero(const char * buf, size_t len) {
  for (size_t i = 0; i<len; i++) {
    if (0 != buf[i])
      return 0;
  }
  return 1;
}

uint32_t calc_crc32(const unsigned char * bytes, size_t size)
{
  uint32_t crc = crc32(0L, Z_NULL, 0);
  for (int i = 0; i < size; ++i)
  {
      crc = crc32(crc, bytes + i, 1);
  }
  return crc;
}

int check_file(const char * filename, uint32_t checksum) {
  void * filedata = NULL;
  size_t file_length = 0;
  if (readfile(filename, &filedata, &file_length)) {
    return 0;
  }

  if ((uint16_t) checksum == (uint16_t) calc_crc32(filedata, file_length)) {
    return 1;
  }
  return 0;

}
int readfile(const char * path, void ** out_d, size_t *out_ld)
{
        void * d = NULL;
        size_t ld = 0;
        ssize_t read_cur = 0;
        size_t read_tot = 0;
        int ret = 0;
        int fd = open(path, O_RDONLY);

        if (-1 == fd)
        {
                printf("File read error\n");
                ret = -3;
                goto cleanup;
        }

        ld = lseek(fd, 0, SEEK_END);

        if ((-1 == ld) || (SANITY_LEN < ld))
        {
                printf("File read error\n");
                ret = -4;
                goto cleanup;
        }

        if (-1 == lseek(fd, 0, SEEK_SET))
        {
                printf("File read error\n");
                ret = -5;
                goto cleanup;
        }

        d = malloc(ld);

        while(read_tot < ld)
        {
                read_cur = read(fd, d + read_tot, ld - read_tot);
                if (-1 == read_cur)
                {
                        printf("File read error\n");
                        ret = -6;
                        goto cleanup;
                }
                read_tot += (size_t)read_cur;
        }

        *out_d = d;
        *out_ld = ld;

cleanup:
        if (-1 != fd)
                close(fd);
        if ((0 != ret) && (NULL != d))
                free(d);
        return ret;
}
