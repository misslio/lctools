

#include "wrapfcntl.h"

int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
	struct flock lock;

	lock.l_type = type;	/* F_RDLCK, F_WRLCK, F_UNLCK */
	lock.l_start = offset; /* byte offset, relative to l_whence */
	lock.l_whence = whence; /* SEEK_SET, SEEK_CUR, SEEK_END */
	lock.l_len = len; /* #bytes (0 means to EOF) */

	return (fcntl(fd, cmd, &lock));
}

int read_lock(int fd, off_t offset, int whence, off_t len)	
{
	return lock_reg(fd, F_SETLK, F_RDLCK, offset, whence, len);
}

int readw_lock(int fd, off_t offset, int whence, off_t len)	
{
	return lock_reg(fd, F_SETLKW, F_RDLCK, offset, whence, len);
}

int write_lock(int fd, off_t offset, int whence, off_t len)
{
	return lock_reg(fd, F_SETLK, F_WRLCK, offset, whence, len);
}

int writew_lock(int fd, off_t offset, int whence, off_t len)	
{
	return lock_reg(fd, F_SETLKW, F_WRLCK, offset, whence, len);
}

int un_lock(int fd, off_t offset, int whence, off_t len)
{
	return lock_reg(fd, F_SETLK, F_UNLCK, offset, whence, len);
}



