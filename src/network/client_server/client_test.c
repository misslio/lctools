#include <stdio.h>  
#include <sys/types.h>  
#include <netinet/in.h>  
#include <sys/socket.h>  
#include <netdb.h>  
#include <unistd.h>  
#include <string.h>  
#include <arpa/inet.h>  
#include <sys/wait.h>  
#include <errno.h>  
#include <pthread.h>


#define SRV_PORT 54929 

#define MAX_BUF_SIZE	8096

#define PRINT_ERR(format, args...)		printf("Func: %s, Line : %d [\033[31mERR\033[0m] : "format, __func__, __LINE__, ##args)
#define PRINT_DEBUG(format, args...)		printf("Func: %s, Line : %d [DEBUG] : "format, __func__, __LINE__, ##args)

#define THREAD_NUMS	1

#define BUF_LEN 	64

char date_buf[BUF_LEN];
char register_code[BUF_LEN];


void *pth_func(void *args)
{
	char rcv_buf[MAX_BUF_SIZE], snd_buf[MAX_BUF_SIZE];

	char tip_date[] = "DATE:", tip_code[] = "HDCODE:";

	int sock_fd, len;
	struct sockaddr_in	srv_addr;

	bzero(&srv_addr, sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(SRV_PORT);
//	srv_addr.sin_addr.s_addr = inet_addr("192.168.77.183");
	srv_addr.sin_addr.s_addr = INADDR_ANY;

	int i, j = 0;
	while(1){
		sock_fd = socket(AF_INET, SOCK_STREAM, 0);
		
		if(connect(sock_fd, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0){
			PRINT_ERR("Connect server fail\n");
			close(sock_fd);
			return NULL;
		}
#if 1
		while(j < 10){
			j++;
			memset(snd_buf, 0, MAX_BUF_SIZE);
			char *ptr = snd_buf;
			for(i = 0; i < 80; i++){
				memcpy(ptr + i, "G", 1);
			}
			snd_buf[i] = '\0';
			if(j == 9)
				strcpy(snd_buf, "OVER");
			if(write(sock_fd, snd_buf, strlen(snd_buf)) < 0){
				PRINT_ERR("Send info  fail\n");
				close(sock_fd);
				return NULL;
			} 

			PRINT_DEBUG("Send info success\n");
			sleep(4);
		}
#if 0
		memset(rcv_buf, 0, MAX_BUF_SIZE);
		int ret = 0;
		if((ret = read(sock_fd, rcv_buf, MAX_BUF_SIZE)) < 0){
			PRINT_ERR("Recv info from server fail\n");
		}else
			PRINT_DEBUG("Recv info success! %s\n", rcv_buf);
#endif
#endif
		while(1);
		close(sock_fd);
		break;
		sleep(1);
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t pth[THREAD_NUMS];
	int i;
	for(i = 0; i < THREAD_NUMS; i++){
		pthread_create(&pth[i], NULL, pth_func, NULL);
	}

	i = 0;
	for(; i < THREAD_NUMS; i++){
		pthread_join(pth[i], NULL);
		PRINT_DEBUG("-----------> Thread %d exit\n", i);
	}

	return 0;
	
}
