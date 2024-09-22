#ifndef __WRAPPERS_H__
#define __WRAPPERS_H__

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void print_error_and_exit(const char *message);

int _pipe(int *pipefd);

pid_t _fork(void);

int _close(int fd);

pid_t _wait(int *wstatus);

pid_t _waitpid(pid_t pid, int *wstatus, int options);

int _execvp(const char *file, char *const argv[]);

int _dup2(int oldfd, int newfd);

#endif /* __WRAPPERS_H__ */
