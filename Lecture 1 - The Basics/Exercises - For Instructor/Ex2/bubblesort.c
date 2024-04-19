#include <stdio.h>
#include <time.h>
#include <stdlib.h>


#define MAX_VALUE	(0x7F)
#define MIN_VALUE	(-0x80)
#define DIFF_VALUE	((MAX_VALUE)-(MIN_VALUE)+1)
#define ARR_SIZE	(0x10)

void swap(int *px, int *py)
{
    int x = *px;
    *px = *py;
    *py = x;
}

void bubble_sort(int values[], int n)
{
   int i, j;
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


void print_array(int values[], int size)
{
    int i;
	printf("[");
    for (i=0; i < size; i++)
	{
        printf("%d, ", values[i]);
	}
    printf("]\n");
}

void random_init()
{
	srand((unsigned int)time(NULL));
}

void randomize_array(int values[], int n)
{
	int i;
	for (i = 0; i < n; i++)
	{
		values[i] = (rand() % DIFF_VALUE) + MIN_VALUE;
	}
}



int main()
{
	int arr[ARR_SIZE];
	int n = sizeof(arr)/sizeof(arr[0]);
	random_init();
	randomize_array(arr, n);
    bubble_sort(arr, n);
    print_array(arr, n);
    return 0;
}
