#include <stdio.h>
#include <signal.h>
#include <pwd.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define SERVER_TO_KILL_PATH		"/var/run/demo_signal_server.pid"

/* 单实例进程, 调用该函数，重复运行会直接退出 */
int already_running(void)
{
	int fd;
	char buf[32];
	fd = open(SERVER_TO_KILL_PATH,	 O_RDWR|O_CREAT, 0644); /* 该文件不能关闭，否则锁会失效 */
	if (fd < 0) {
		exit(1);
	}
	
	ftruncate(fd, 0);
	memset(buf, 0, 32);
	snprintf(buf, 32, "%ld\n", (long)getpid());
	write(fd, buf, strlen(buf));

	return(0);
}

static void license_signal_handle(int signo)
{
	printf("%s, Good bye! I am dying...\n", __func__);
	exit(0);
}

void * pth_fuc(void *arg)
{
	signal(SIGUSR1, license_signal_handle);
	printf("I am ready\n");
	pause();
}

int main()
{
	already_running();
	pthread_t pth;
	pthread_create(&pth, NULL, &pth_fuc , NULL);
	pthread_detach(pth);
	while(1);
	return 0;
}


