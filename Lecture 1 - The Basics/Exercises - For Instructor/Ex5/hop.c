#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define WIDTH (13)
#define HEIGHT (13)



char  arr[] = 	"#############"
				"#     #     #"
				"# ### # ### #"
				"#   # # #$# #"
				"### # # # # #"
				"#   #   # # #"
				"# ####### # #"
				"#  #      # #"
				"## # ###### #"
				"#  #  #  @# #"
				"# ### # ### #"
				"#     #     #"
				"#############";
				
// answer = "wwsseeeennnnnnnnnnwwwwsssswwnnnnwwwwsseesswwssesswsseeeennwnneeeeennnn"

int parse_char(char chr)
{
	switch(chr)
	{
		case 'n':
			return (-1)*WIDTH; 
		case 's':
			return WIDTH;
		case 'e':
			return 1;
		case 'w':
			return -1;
		default :
			return 0;
	}
}


int find_start()
{
	int off = 0;
	for (off = 0; off < sizeof(arr)/sizeof(arr[0]); ++off)
	{
		if ('@' == arr[off])
			return off;
	}
	return -1;
}


int check_input(char * input)
{
	int pos =  find_start();
	//printf("pos: %d,%d\n", pos%WIDTH, pos/WIDTH);
	if (-1 == pos)
		return -1;

	int off = 0, jmp = 0;

	char curr_chr = input[off];

	while('\0' != curr_chr)
	{
		//printf("curr char %c\n", curr_chr);
		jmp = parse_char(curr_chr);
		
		pos += jmp;
		//printf("pos: %d: %d,%d\n",pos, pos%WIDTH, pos/WIDTH);

		if (pos < 0 || pos >= sizeof(arr)/sizeof(arr[0]))
			return -1;
	
		switch(arr[pos])
		{
			case ' ':
				off++;
				break;
			case '#':
				return -1;
			case '$':
				return 0;
			case '@':
				off++;
				break;

		}

		curr_chr = input[off];		
	}

	return -1;

}


int main()
{	
	char input[0x100] = {0};
	FILE* fp;

	//printf("map:\n%s\n",arr);
	//printf("len of arr: %d\n", sizeof(arr));
	
	fp = fopen("input.txt", "r");
	if (NULL == fp)
	{
		printf("No input file!!!\n");
		return 0;
	} 
	fgets(input, sizeof(input), fp);

	if (0 == check_input(input))
		printf("SUCCESS!!!!\n");
	else
		printf("Wrong Password!!!!\n");

}