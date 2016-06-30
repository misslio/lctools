/******************************************************************
 * filename : wrapfcntl.h
 * des : packing of "fcntl" function
 *
 *
 * date	: 2016-05-06
 ******************************************************************/



#ifndef WRAPFCNTL_H
#define WRAPFCNTL_H

#include <stdio.h>
#include <string.h>

#include <sys/types.h> /* about files */
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

int read_lock(int, off_t, int, off_t);
int readw_lock(int, off_t, int, off_t);
int write_lock(int, off_t, int, off_t);
int writew_lock(int, off_t, int, off_t);
int un_lock(int, off_t, int, off_t);

int lock_reg_special(int fd, int cmd, int type, off_t offset, int whence, off_t len);

#endif
