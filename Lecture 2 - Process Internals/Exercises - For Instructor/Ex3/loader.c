#include <stdio.h>




int main(int argc, char ** argv)
{
	unsigned int i=0;
	printf("This is an executable\nIt has been called with:\n");
	if (1>=argc)
		printf("no argruments\n");
	else
		for(i=0;i<argc-1;++i)
		{
			printf("%s\n",argv[i+1]);
		}
	
	return 0;
}




