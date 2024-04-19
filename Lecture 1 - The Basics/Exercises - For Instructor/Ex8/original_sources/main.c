#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include <fcntl.h>
#include <sys/mman.h>


void s2md5_openssl(char *s, unsigned char out_md5[16])
{
        MD5_CTX md5;
        MD5_Init(&md5);
        MD5_Update(&md5, s, strlen(s));
        MD5_Final(out_md5, &md5);
        return;
}

void s2md5_openssl_short(char * s, unsigned char out_md5[16])
{
        MD5(s, strlen(s), out_md5);
        return;
}

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
//void print_digest(char dg[16])
//{
//      unsigned int i = 0;
//      for (i=0;i<16;++i)
//              printf("%.2x ",dg[i]);
//      printf("\n");
//      return;
//}

int main(int argc, char ** argv)
{
        unsigned char md5[16] = {0};
        uint16_t cs = 0;
        void * d = -1;
        int ret = 0;
        size_t ld = 0;

        if (2 != argc)
        {
                printf("usage: test_your_might [password]\n");
                return -1;
        }

        s2md5_openssl_short(argv[1], md5);

        if (memcmp(pwd,md5,16))
        {
                printf("Wrong password!\n");
                //print_digest(md5);
                return -2;
        }


        if (0 != (ret = read_f(argv[0], &d, &ld)))
        {
                return ret;
        }

        cs = adler16_and_co(d,ld);
        free(d);

        if (cs != hmmm)
        {
                printf("Nope\n");
                return -8;
        }

        printf("Well Done! Your reward:\n%s\n", reward_url);
        return 0;
}

#define SANITY_LEN (0x80000)

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
                ret -3;
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
