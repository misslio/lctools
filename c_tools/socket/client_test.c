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

#include "../common/include/iri_license.h"

#define SRV_PORT 54929 

#define MAX_BUF_SIZE	1024

#define PRINT_ERR(format, args...)		printf("Func: %s, Line : %d [\033[31mERR\033[0m] : "format, __func__, __LINE__, ##args)
#define PRINT_DEBUG(format, args...)		printf("Func: %s, Line : %d [DEBUG] : "format, __func__, __LINE__, ##args)

#define THREAD_NUMS	1

int handle_send_data(int conn_fd, char *act_code)
{
	int ret = -1;
	if(NULL == act_code)
		return ret;

	char *encypt_snd_buf = NULL;

	ret = rsa_base64(act_code, strlen(act_code), &encypt_snd_buf, PUB_KEY);	/*????*/
	if(ret > 0){
		if(write(conn_fd, encypt_snd_buf, ret) < 0){
			iridium_write_log_v(LIC_LOG_FILE_V, IRILOG_ERR_V, "Send message fail.\n");
			store_code(act_code);
		}else{
			iridium_write_log_v(LIC_LOG_FILE_V, IRILOG_DEBUG_V, "Send success.\n");
		}
	}

	LC_FREE_MALLOC(encypt_snd_buf);

	return ret;
}


void *pth_func(void *args)
{
	char rcv_buf[MAX_BUF_SIZE], snd_buf[MAX_BUF_SIZE];

	int sock_fd;
	struct sockaddr_in	srv_addr;

	bzero(&srv_addr, sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(SRV_PORT);
	srv_addr.sin_addr.s_addr = inet_addr("192.168.77.183");

	while(1){
		sock_fd = socket(AF_INET, SOCK_STREAM, 0);
		
		if(connect(sock_fd, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0){
			PRINT_ERR("Connect server fail\n");
			close(sock_fd);
			return NULL;
		}
	
		
		memset(snd_buf, 0, MAX_BUF_SIZE);
		if(write(sock_fd, snd_buf, ret) < 0){
			PRINT_ERR("Send info into socket error!\n");
			close(conn_fd);
			continue;
		}
			
		memset(rcv_buf, 0, MAX_BUF_SIZE);
		if((ret = read(sock_fd, rcv_buf, MAX_BUF_SIZE)) <= 0){
			PRINT_ERR("Read info from socket error!\n");
			close(conn_fd);
			continue;
		
		}		

		close(sock_fd);

		sleep(1);
	}

	return NULL;
}
int main()
{
	pthread_t pth[THREAD_NUMS];

	int i = 0;
	for(; i < THREAD_NUMS; i++){
		pthread_create(&pth[i], NULL, pth_func, NULL);
	}

	i = 0;
	for(; i < THREAD_NUMS; i++){
		pthread_join(pth[i], NULL);
		PRINT_DEBUG("-----------> Thread %d exit\n", i);
	}

	return 0;
	
	
}
