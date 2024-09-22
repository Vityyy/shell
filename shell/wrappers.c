#include "wrappers.h"

void
print_error_and_exit(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

int
_pipe(int *pipefd)
{
	int res = pipe(pipefd);
	if (res < 0)
		print_error_and_exit("Error in pipe().\n");
	return res;
}

pid_t
_fork(void)
{
	pid_t res = fork();
	if (res < 0)
		print_error_and_exit("Error in fork().\n");
	return res;
}

int
_close(int fd)
{
	int res = close(fd);
	if (res < 0)
		print_error_and_exit("Error in close().\n");
	return res;
}

pid_t
_wait(int *wstatus)
{
	int res = wait(wstatus);
	if (res < 0)
		print_error_and_exit("Error in wait().\n");
	return res;
}

pid_t
_waitpid(pid_t pid, int *wstatus, int options)
{
	int res = waitpid(pid, wstatus, options);
	if (res < 0)
		print_error_and_exit("Error in waitpid().\n");
	return res;
}

int
_execvp(const char *file, char *const argv[])
{
	int res = execvp(file, argv);
	if (res < 0)
		print_error_and_exit("Error in execvp().\n");
	return res;  // It never gets here
}

int
_dup2(int oldfd, int newfd)
{
	int res = dup2(oldfd, newfd);
	if (res < 0)
		print_error_and_exit("Error in dup2().\n");
	return res;
}
