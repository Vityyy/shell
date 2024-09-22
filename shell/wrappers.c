#include "wrappers.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

void print_error_and_exit(const char *message);

void
print_error_and_exit(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

int
spipe(int *pipefd)
{
	int res = pipe(pipefd);
	if (res < 0)
		print_error_and_exit("Error in pipe().\n");
	return res;
}

pid_t
sfork(void)
{
	pid_t res = fork();
	if (res < 0)
		print_error_and_exit("Error in fork().\n");
	return res;
}

int
sclose(int fd)
{
	int res = close(fd);
	if (res < 0)
		print_error_and_exit("Error in close().\n");
	return res;
}

pid_t
swait(int *wstatus)
{
	int res = wait(wstatus);
	if (res < 0)
		print_error_and_exit("Error in wait().\n");
	return res;
}

pid_t
swaitpid(pid_t pid, int *wstatus, int options)
{
	int res = waitpid(pid, wstatus, options);
	if (res < 0)
		print_error_and_exit("Error in waitpid().\n");
	return res;
}

int
sexecvp(const char *file, char *const argv[])
{
	int res = execvp(file, argv);
	if (res < 0)
		print_error_and_exit("Error in execvp().\n");
	return res;  // It never gets here
}

int
sdup2(int oldfd, int newfd)
{
	int res = dup2(oldfd, newfd);
	if (res < 0)
		print_error_and_exit("Error in dup2().\n");
	return res;
}
