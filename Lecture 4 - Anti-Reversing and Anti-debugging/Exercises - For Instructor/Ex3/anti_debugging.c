#define _GNU_SOURCE

#include <stdio.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stddef.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_FUNC_SIZE (0x1000)
#define NUM_ELEMENTS(a) (sizeof(a) / sizeof(*a))

int code_hash_check();

unsigned int xor = 0;
time_t ts = 0;
unsigned int scramble_index = 0;

void scramble1(unsigned int x)
{
    xor ^= ( (xor << 8) ^ x ) ;
}

void scramble2(unsigned int x)
{
    xor ^= x ;
    xor += 1337;
}

void scramble3(unsigned int x)
{
    xor ^= ( (xor * x ) << 16);
}


void (*scramble[])(unsigned int) = {&scramble1, &scramble2, &scramble3};

void call_scrambler(unsigned int x)
{
    scramble_index = (scramble_index + 1) % NUM_ELEMENTS(scramble);
    scramble[scramble_index](x);
}


int ptrace_check()
{
    long res = 0;
    if ( ptrace(PTRACE_TRACEME, 0, 1, 0) < 0 ) 
    {
        printf("You cheeky bastard, don't you trace me!!!\n");
        exit(1);
    }

   
    res = ptrace(PTRACE_TRACEME, 0, 1, 0);

    call_scrambler(res);

    pid_t p;
    if (p = fork())
    {
        waitpid(p, NULL, 0);
        exit(0);
    }
   
}


int timestamp_check()
{
   time_t now;
   double diff;
   ptrdiff_t res = 0 ;


   time(&now);
   diff = difftime(now, ts);

   if (diff > 1)
   {
       printf("There is only enough time for everything\n");
       exit(1);
   }

    res = (void *)&timestamp_check - (void *)&ptrace_check;

    call_scrambler(res);

}

void AD_checks()
{

    ptrace_check();
    timestamp_check();
    code_hash_check();
}

void init_checks()
{
    time(&ts);
    xor = 0x13371337;
    scramble_index  = 1;
}

int my_memcmp(const void *mem1, const void *mem2, size_t n)
{
    const char * s1 = mem1;
    const char * s2 = mem2;
    while (n-- > 0)
    {
      if (*s1++ != *s2++)
          return s1[-1] < s2[-1] ? -1 : 1;
    }
    return 0;
}

char pass_enc[] = {0x8d,0x6f,0x16,0xd5,0xc4,0x20,0x3f,0xde, 0xce,0x2a,0x21,0x87,0x8d };

int check_input(char * in)
{
    char buff[32] = {0};

    size_t n = strlen(in);
    if (n != sizeof(pass_enc))
    {
        printf("password not the right length\n");
        exit(1);
    }
    
    strcpy(buff, in);

    AD_checks();

    for (int i = 0; i < n; i++)
    {
        buff[i] ^= ((unsigned char *)&xor)[i % 4];
    }

    AD_checks();

    return (0 == my_memcmp(buff, pass_enc, sizeof(pass_enc)));
}


int main(int argc, char ** argv)
{

    if (argc < 2 )
    {
        printf("USAGE: AD <pass>\n");
        exit(0);
    }

    init_checks();

    AD_checks();

    if (check_input(argv[1]))
    {
        printf("SUCCESS!!\n");
        return 0;
    }
    printf("FAILURE!!\n");


}


int code_hash_check()
{
    
    ptrdiff_t len = (void *)&code_hash_check - (void *)&scramble1;
    unsigned char h = 0;

    char * code = (char *)&scramble1;

    for (int i =0; i < len; i++)
    {
        h^=code[i];
    }
    
    if (h != 0x51)
    {
       printf("Something is wrong with the code\n");
       exit(1);
    }
	
	for (int i =0; i < len; i++)
    {
        h+=code[i];
    }
        
    call_scrambler(h);
    
}


/*
* For more complex exercises I thought to move the differnet checks around the memory.
* The problem with that is that there are a lot of relative instructions which won't work after
* the move. Need to somehow make it PIC.
*/

/*

int (*f_ptrace_check)(void) = NULL;
int (*f_timestamp_check)(void) = NULL;
int (*f_code_hash_check)(void) = NULL;


void * copy_function(void * func, size_t s)
{
    char int3 = 0xCC;

    //cc_detected += (char)memmem(func,s, &int3, 1 );
    

    void * code = mmap(NULL, s, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (MAP_FAILED == code)
    {
        printf("map failed\n");
        exit(1);
    }

    memcpy(code, func, s);

    if (-1 == mprotect(code, s, PROT_READ | PROT_EXEC))
    {
        printf("mprotect failed\n");
        exit(1);
    }
    
    return code;
}


void AD_checks()
{
    f_ptrace_check = copy_function(&ptrace_check, MAX_FUNC_SIZE);
    f_timestamp_check = copy_function(&timestamp_check , MAX_FUNC_SIZE);
    f_code_hash_check = copy_function(&code_hash_check , MAX_FUNC_SIZE);

    f_ptrace_check();
    f_timestamp_check();
    f_code_hash_check();

}
*/