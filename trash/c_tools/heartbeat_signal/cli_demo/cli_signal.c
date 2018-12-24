#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

/* Heart pthread func. Send signal to others to keep self alive */
void *pth_func(void *arg)
{
	pid_t pid;
	
	int tmp = atoi(arg);
	pid = (pid_t)tmp;
	
	while(1){
		sleep(2);
		kill(pid , SIGUSR1);
		printf("I am alive have send\n");
	}

}


/* Main pthread func. Do main things */
int main(int argc, char *argv[])
{
	pid_t pid;
	pthread_t pth;

	

	pthread_create(&pth, NULL, &pth_func, (void*)argv[1]);
	pthread_detach(pth);

	while(1);

	return 0;
}
