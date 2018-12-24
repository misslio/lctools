/**
 * @file - daemon.c
 * @author - lc
 **/

#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

void daemonize()
{
	int fd0, fd1, fd2;
	u_int i;
	pid_t pid;
	struct rlimit rl;
	struct sigaction sa;
	/*
	 * Clear file creation mask.
	 */
	umask(0);
	/*
	 * Get maximum number of file descriptors.
	 */
	getrlimit(RLIMIT_NOFILE, &rl);
	/*
	 * Become a session leader to lose controlling TTY.
	 */
	if ((pid = fork()) < 0)
		return;
	else if (pid != 0) /* parent */
		exit(0);
	setsid();
	/*
	 * Ensure future opens won¡¯t allocate controlling TTYs.
	 */
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);

	sa.sa_flags = 0;
	sigaction(SIGHUP, &sa, NULL);
	if ((pid = fork()) < 0)
		return ;
	else if (pid != 0) /* parent */
		exit(0);
	/*
	 * Change the current working directory to the root so
	 * we won¡¯t prevent file systems from being unmounted.
	 */
	chdir("/");
	/*
	 * Close all open file descriptors.
	 */
	if (rl.rlim_max == RLIM_INFINITY)
		rl.rlim_max = 1024;
	for (i = 0; i < rl.rlim_max; i++)
		close(i);
	/*
	 * Attach file descriptors 0, 1, and 2 to /dev/null.
	 */
	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);
	/*
	 * Initialize the log file.
	 */
	if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
		exit(1);
	}
}


