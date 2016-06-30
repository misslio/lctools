#include <stdio.h>
#include <string.h>

/**
 * desc - Check ip format
 *
 * @param - ip 
 *
 * @ret - 1 valid
 * 		  0 invalid or error
 * author - lc at 2016-06-30
 * */
int is_valid_ipv4(char *ip)
{	
	if(NULL == ip)
		return 0;
	
	char tail[16] = {0};
	int  i, count;
	unsigned int d[4];
	
	/*16 + 15 = 32 */
	if(strlen(ip) > 30)
		return 0;

	tail[0] = 0;

	count  = sscanf(ip, "%3u.%3u.%3u.%3u%s", &d[0], &d[1], &d[2], &d[3], tail);
	if (count != 4 || tail[0])
		return 0;

	for (i = 0; i < 4; i++)
		if (d[i] > 255)
			return 0;

	return 1;
}

/**
 * usage: ./xxx 192.16.22.12
 * */
int main(int argc, char **argv)
{
	if(argc != 2)
		return -1;

	if(is_valid_ipv4(argv[1]))
		printf("OK!\n");
	else
		printf("NO!\n");

	return 0;
}
