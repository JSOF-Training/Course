#include <stdio.h>
#include <time.h>
#include <stdlib.h>


#define MAX_VALUE	(0x7F)
#define MIN_VALUE	(-0x80)
#define DIFF_VALUE	((MAX_VALUE)-(MIN_VALUE)+1)
#define ARR_SIZE	(0x10)

static inline void swap(int *px, int *py)
{
    int x = *px;
    *px = *py;
    *py = x;
}

__attribute__((used)) void bubble_sort(int values[], size_t n)
{
   size_t i, j;
   for (i = 0; i < n-1; i++)
   {
       for (j = 0; j < n-i-1; j++)
	   {
           if (values[j] > values[j+1])
		   {
              swap(&values[j], &values[j+1]);
		   }
	   }
   }
}

int main()
{
    printf("Your task is to describe what the function at address %p does.\n", bubble_sort);
}
