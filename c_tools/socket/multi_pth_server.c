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


  
//#define PORT 54324  
#define PORT 54929
#define BACKLOG 3  

#define MAX_BUF_SIZE	1024

#define PRINT_ERR(format, args...)		printf("Func: %s, Line : %d [\033[31mERR\033[0m] : "format, __func__, __LINE__, ##args)

#ifdef DEBUG
#define PRINT_DEBUG(format, args...)		printf("Func: %s, Line : %d [DEBUG] : "format, __func__, __LINE__, ##args)
#else
#define PRINT_DEBUG(format, args...) do{}while(0)
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
	
	daemonize();
	
	already_running();
	
	pthread_t pth[MAX_PTHREAD_SIZE];
  	bzero(&srv_addr, sizeof(struct sockaddr_in));
	
    srv_addr.sin_family = AF_INET;  
    srv_addr.sin_port = htons(PORT);  
	srv_addr.sin_addr.s_addr=INADDR_ANY;

#if 0
	if(inet_pton(AF_INET, SRV_IP, &srv_addr.sin_addr) != 1){
		return -1;
	}
#endif
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd < 0){
		return -1;
	}
	
	int opt = 1;
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	
    if(bind(listen_fd, (struct sockaddr *)&srv_addr, sizeof(struct sockaddr)) < 0){
		close(listen_fd);
		return -1;
	}
	
    listen(listen_fd, BACKLOG); 

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
	char rcv_buf[MAX_BUF_SIZE], snd_buf[MAX_BUF_SIZE], act_en_buf[64];
	int conn_fd, ret = 0;
	
	struct sockaddr_in cli_addr;  
	int addr_size, i, err_flag = 0;
	lic_info_t info;

	while(1){
		memset(&info, 0, sizeof(info));
		bzero(&cli_addr, sizeof(struct sockaddr_in));
		addr_size = sizeof(struct sockaddr);
		/* accept 阻塞需要一个互斥锁 */
		pthread_mutex_lock(&pth_mutex);
		conn_fd = accept(listen_fd, (struct sockaddr *)&cli_addr, &addr_size);
		pthread_mutex_unlock(&pth_mutex);
		
		if(conn_fd < 0){
			continue;
		}
	
		if((ret = read(conn_fd, rcv_buf, MAX_BUF_SIZE)) <= 0){
			PRINT_ERR("Read info from socket error!\n");
			close(conn_fd);
			continue;
		
		}		
		if(write(conn_fd, encypt_snd_buf, ret) < 0){
			PRINT_ERR("Send info into socket error!\n");
			close(conn_fd);
			continue;
		}
		
		close(conn_fd);
	}
	return 0;
}
