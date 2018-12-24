/**
 * @file - single_instance.c
 * @author - lc
 **/
 
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "single_instance.h"

static int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
	struct flock lock;

	lock.l_type = type;	/* F_RDLCK, F_WRLCK, F_UNLCK */
	lock.l_start = offset; /* byte offset, relative to l_whence */
	lock.l_whence = whence; /* SEEK_SET, SEEK_CUR, SEEK_END */
	lock.l_len = len; /* #bytes (0 means to EOF) */

	return (fcntl(fd, cmd, &lock));
}


/* 单实例进程, 调用该函数，重复运行会直接退出 */
int already_running(void)
{
	int fd;
	char buf[32];
	fd = open(LOCKFILE, O_RDWR|O_CREAT, 0644); /* 该文件不能关闭，否则锁会失效 */
	if (fd < 0) {
		exit(1);
	}
	/* 加建议性写锁,进程运行期间一直持有，不可释放 */
	if (lock_reg(fd, F_SETLK, F_WRLCK, 0, SEEK_SET, 0) < 0) {		
		close(fd);
		fprintf(stderr, "process has been running!\n");
		exit(1);
	}
	
	ftruncate(fd, 0);
	memset(buf, 0, 32);
	snprintf(buf, 32, "%ld\n", (long)getpid());
	write(fd, buf, strlen(buf));
	
	return(0);
}

/* 单实例进程, 调用该函数，重复运行会直接退出 */
int already_running_with_name(char *lock_file_name)
{
	int fd;
	char buf[32];
	char file_buf[64];
	snprintf(file_buf, 64, "/var/run/%s.pid", lock_file_name);

	fd = open(file_buf, O_RDWR|O_CREAT, 0644); /* 该文件不能关闭，否则锁会失效 */
	if (fd < 0) {
		exit(1);
	}
	/* 加建议性写锁,进程运行期间一直持有，不可释放 */
	if (lock_reg(fd, F_SETLK, F_WRLCK, 0, SEEK_SET, 0) < 0) {		
		close(fd);
		fprintf(stderr, "process has been running!\n");
		exit(1);
	}
	
	ftruncate(fd, 0);
	memset(buf, 0, 32);
	snprintf(buf, 32, "%ld\n", (long)getpid());
	write(fd, buf, strlen(buf));
	
	return(0);
}


