#include <stdio.h>
#include <stdarg.h>


void no_args()
{
   return;
}

int no_args2()
{
	return 1;
}

int sum_of_3(int a, int b, int c)
{
	return a+b+c;
}

int sum_and_print(short a, long b, int c)
{
	int sum = a + b + c;
	printf("Sum of %hd, %ld and %d is %d\n", a, b, c, sum);
	return sum;
}

int var_args(unsigned int a, char* s, int num, ...)
{
	va_list list;
	int sum = 0;
	int cur_arg = 0;
	va_start(list, num);

	printf("The first fixed arg is %u\n", a);
	printf("The second fixed arg is \"%s\"\n", s);
	printf("Expecting %d variable-length args:\n", num);
	for (int i=0; i<num; i++)
	{
		cur_arg = va_arg(list, int);
		printf("  Variable arg %d is %d\n", i, cur_arg);
		sum += cur_arg;
	}
	va_end(list);
	return a*sum;
}

int main()
{
	int x = 0, y = 0, z = 0;
	no_args();
	x = no_args2();
	y = sum_of_3(x, 7, 945);
	z = sum_and_print(383, x, y);
	x = var_args(z, "Hello.", 6, x, y, z, x, y, z);
	printf("Final result is %d", x);

	return 0;
}
