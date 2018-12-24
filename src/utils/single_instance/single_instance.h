/**
 * @file - single_instance.h
 * @author - lc
 **/
#ifndef SINGLE_INSTANCE_H
#define SINGLE_INSTANCE_H

#include <stdio.h>

#define LOCKFILE 	"/var/run/default_proc.pid"


int already_running(void);

int already_running_with_name(char *lock_file_name);

#endif
