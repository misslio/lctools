#include "../../public/include/apue.h"
#include <pwd.h>
static void my_alarm(int signo)
{
	printf("%s, i got a SIGALRM\n", __func__);
	signal(SIGALRM, my_alarm);
	alarm(1);
}


int main(void)
{
	struct passwd *ptr;
//	signal(SIGALRM, SIG_IGN);
	signal(SIGALRM, my_alarm);

	alarm(1); /* send message to self */
	for ( ; ; ) {
		sleep(2);
		printf("i am the main\n");
//		alarm(1);
	}
}
