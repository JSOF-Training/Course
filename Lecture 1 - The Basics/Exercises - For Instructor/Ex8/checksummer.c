#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include <fcntl.h>
#include <sys/mman.h>


uint16_t rndata[0x10] = {0xbaba,0xb0b0,0x1234,0x6543,0x1357,0x8765,0xbabe,0xdead,0xbebe,0xfafa,0xf0f0,0xd0d0,0xe0e0,0xeaea,0xdede,0xfefe};


uint16_t adler16_and_co(unsigned char * m, size_t len)
{
        uint16_t checksum = 0;
        size_t flen = len >> 2;
        size_t i = 0;
        size_t rnlen = sizeof(rndata)/sizeof(uint16_t);
        uint16_t * data = (uint16_t *)m;

        for (i=0; i < flen; ++i)
        {
                checksum += data[i] ^ rndata[i % rnlen];
        }

        if ((2*flen) < len)
        {
                uint16_t t = (((uint16_t)(m[2*flen]))<<0x10)+0xff;
                checksum += t ^ rndata[i % rnlen];
        }

        return checksum;
}

uint16_t hmmm = 0;

unsigned char pwd[16] = {0xba,0xb0,0xbe,0xba,0xde,0xad,0xbe,0xef,0xda,0xda,0xb0,0xb0,0xd0,0xda,0xba,0xbe};
char * reward_url = "https://youtube.com/watch?v=DaVqewZYrNM";

int read_f(char * path, void **out_d, size_t * out_ld);
int write_f(char * path, void *d, size_t ld);

int main(int argc, char ** argv)
{
        uint16_t cs = 0;
        void * d = -1;
        int ret = 0;
        size_t ld = 0;
        size_t offset = 0;
        int match = 0;

        if (4 != argc)
        {
                printf("usage: checksummer [test_your_might file] [hmmm hex offset] [outfile]\n");
                return -1;
        }

        ret = sscanf(argv[2], "%hx", &offset);
        if (1 != ret) {
          printf("Error parsing file offset.\n");
          return -1;
        }

        if (0 != (ret = read_f(argv[1], &d, &ld)))
        {
                return ret;
        }

        if (offset + sizeof(uint16_t) >= ld) {
          printf("Offset out of range.\n");
          return -1;
        }

        for (unsigned int i = 0; i <= UINT16_MAX; ++i){
          *((uint16_t *)(d + offset)) = (uint16_t) i;
          cs = adler16_and_co(d,ld);
          if (cs == (uint16_t) i){
            printf("Match for hmm=0x%X\n", i);
            match = 1;
            break;
          }
          if (!(i%0x1000))
            printf("i=0x%X\n", i);
        }
        if(!match)
          printf("No checksum match\n");
        else {
          if(write_f(argv[3], d, ld)) {
            printf("Error writing outfile %s\n", argv[3]);
            ret = -1;
          }
        }
        free(d);
        return ret;
}

#define SANITY_LEN (0x80000)

int write_f(char * path, void *d, size_t ld)
{
  int ret = 0;
  ssize_t write_cur = 0;
  size_t write_tot = 0;
  int fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0777);

  if (-1 == fd)
  {
          perror("internal error that shouldn't happen errno");
          return -3;
          goto wcleanup;
  }

  while(write_tot < ld)
  {
          write_cur = write(fd, d + write_tot, ld - write_tot);
          if (-1 == write_cur)
          {
                  printf("internal error");
                  ret = -6;
                  goto wcleanup;
          }
          write_tot += (size_t)write_cur;
  }
  wcleanup:
  if (-1 != fd)
      close(fd);
  return ret;


}

int read_f(char * path, void ** out_d, size_t *out_ld)
{
        void * d = -1;
        size_t ld = 0;
        ssize_t read_cur = 0;
        size_t read_tot = 0;
        int ret = 0;
        int fd = open(path, O_RDONLY);

        if (-1 == fd)
        {
                printf("internal error that shouldn't happen errno %d\n");
                ret = -3;
                goto cleanup;
        }

        ld = lseek(fd, 0, SEEK_END);

        if ((-1 == ld) || (SANITY_LEN < ld))
        {
                if (-1 != ld)
                        printf("no need for this\n");
                else
                        printf("different internal error that shouldn't happen errno %d\n");
                ret =  -4;
                goto cleanup;
        }

        if (-1 == lseek(fd, 0, SEEK_SET))
        {
                printf("even different internal error that shouldn't happen errno %d\n");
                ret =  -5;
                goto cleanup;
        }

        d = malloc(ld);

        while(read_tot < ld)
        {
                read_cur = read(fd, d + read_tot, ld - read_tot);
                if (-1 == read_cur)
                {
                        printf("so many of these internal erros errno %d\n");
                        return -6;
                }
                read_tot += (size_t)read_cur;
        }

        *out_d = d;
        *out_ld = ld;

cleanup:
        if (-1 != fd)
                close(fd);
        if ((0 != ret) && (-1 != d))
                free(d);
        return ret;
}
