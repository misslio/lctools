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


  
#define PORT 54929
#define BACKLOG 3  

#define MAX_BUF_SIZE	7000
#define SRV_IP			"192.168.77.183"

#define SIMPLE_DEBUG
#ifdef SIMPLE_DEBUG
#define PRINT_DEBUG(format, args...)	printf("[\033[33mDEBUG INFO\033[0m]\033[33mfunc\033[0m: %s \033[33mline\033[0m: %d :"format, __func__, __LINE__, ##args)
#else
#define PRINT_DEBUG(format, args...)	do{;}while(0)
#endif

#define PRINT_ERR(format, args...) printf("[\033[31mERR INFO\033[0m]\033[31mfunc\033[0m: %s \033[31mline\033[0m: %d :"format, __func__, __LINE__, ##args)


#ifndef LC_FREE_MALLOC
#define LC_FREE_MALLOC(p) do	{\
		if((p) != NULL)\
			{\
				free(p);\
				(p) = NULL;\
			}\
	}while(0)
#endif


#define MAX_PTHREAD_SIZE		50

static int count = 0;

struct sockaddr_in srv_addr;  
int listen_fd;

pthread_mutex_t pth_mutex = PTHREAD_MUTEX_INITIALIZER;

void * pth_handle_conn_func(void *args);

/*
 * daemon, run as single process
 **/
int main(void)  
{   
		
	pthread_t pth[MAX_PTHREAD_SIZE];
  	bzero(&srv_addr, sizeof(struct sockaddr_in));
	
    srv_addr.sin_family = AF_INET;  
    srv_addr.sin_port = htons(PORT);  
	srv_addr.sin_addr.s_addr=INADDR_ANY;

#if 0
	if(inet_pton(AF_INET, SRV_IP, &srv_addr.sin_addr) != 1){
		iridium_write_log_v(LIC_LOG_FILE_V, IRILOG_ERR_V, "Network address was converted fail.\n");
		return -1;
	}
#endif
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd < 0){
		PRINT_ERR("Open socket error\n");
		return -1;
	}
	
	int opt = 1;
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	
    if(bind(listen_fd, (struct sockaddr *)&srv_addr, sizeof(struct sockaddr)) < 0){
		PRINT_ERR("Bind error\n");
		close(listen_fd);
		return -1;
	}
	
    listen(listen_fd, BACKLOG); 
    PRINT_DEBUG("Now we are listenning client\n");

	/* 创建连接处理线程 */
	int i = 0;
	for(; i < MAX_PTHREAD_SIZE; i++){
		pthread_create(&pth[i], NULL, pth_handle_conn_func, NULL);
	}
	
    while(1);

server_err_exit:
	close(listen_fd);
	pthread_mutex_destroy(&pth_mutex);
	return 0;
} 

void * pth_handle_conn_func(void *args)
{
	char rcv_buf[MAX_BUF_SIZE], snd_buf[MAX_BUF_SIZE];
	int conn_fd, ret = 0;
	
	struct sockaddr_in cli_addr;  
	int addr_size, i, err_flag = 0;

	while(1){
		bzero(&cli_addr, sizeof(struct sockaddr_in));
		addr_size = sizeof(struct sockaddr);
		/* accept 阻塞需要一个互斥锁 */
		pthread_mutex_lock(&pth_mutex);
		conn_fd = accept(listen_fd, (struct sockaddr *)&cli_addr, &addr_size);
		pthread_mutex_unlock(&pth_mutex);
		
		if(conn_fd < 0){
			PRINT_ERR("Accept error.\n");
			continue;
		}
#if 1
		i = 0;
		while(1){
			i++;
		//	sleep(1);
			if(i == 20)
				break;
			memset(rcv_buf, 0, MAX_BUF_SIZE);
			if((ret = read(conn_fd, rcv_buf, MAX_BUF_SIZE)) <= 0){
				PRINT_ERR("Recv info error!\n");
				//			return ;
			}
			PRINT_DEBUG("recv info, len = %d\n", ret);
			if(strcmp(rcv_buf, "OVER") == 0)
				break;
			
		}
#if 0
		memset(snd_buf, 0, MAX_BUF_SIZE);
		strcpy(snd_buf, "Hey! You got me, kid!");
		if((ret = write(conn_fd, snd_buf, strlen(snd_buf))) <= 0){
			PRINT_ERR("Send info error!\n");
//			return ;
		}
		PRINT_DEBUG("Info have been send!\n");
#endif
#endif
//		sleep(1);
		close(conn_fd);
	}
	return 0;
}
