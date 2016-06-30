#include "../../public/include/apue.h"
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int val;

	if(argc != 2)
		err_quit("Usage: a.out <descriptor>");

	if((val = fcntl(atoi(argv[1]), F_GETFL, 0)) < 0)
		err_sys("fcntl error for fd %d", atoi(argv[1]));

	switch(val & O_ACCMODE){
		case O_RDONLY:
			printf("read only");
			break;
		case O_WRONLY:
			printf("write only");
			break;
		case O_RDWR:
			printf("read write");
			break;
		default:
			err_dump("unknown access mode");
	}

#if !defined(_POSIX_C_SOURCE) && defined(O_FSYNC) && (O_FSYNX != O_SYNC)
	if(val & O_FSYNC)
		printf(", sysnchronous write.");
#endif

	putchar('\n');
	return 0;	
}



