#include <stdio.h>






int __attribute__((__constructor__))  callable()
{
	int pid = getpid();
	char cmdpth[0x400];
	char args[0x400];
	snprintf(cmdpth, sizeof(cmdpth), "/proc/%d/cmdline", pid);
	FILE * fp = fopen(cmdpth,"r");
	fgets(args, sizeof(args), fp);
	printf("--------\n(you have) loaded loadme.so into %s (process %d)!\nWell Done.\n--------\n", args, pid);
	return 0;
}

