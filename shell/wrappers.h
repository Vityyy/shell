#ifndef WRAPPERS_H
#define WRAPPERS_H

#include <unistd.h>

int spipe(int *pipefd);

pid_t sfork(void);

int sclose(int fd);

pid_t swait(int *wstatus);

pid_t swaitpid(pid_t pid, int *wstatus, int options);

int sexecvp(const char *file, char *const argv[]);

int sdup2(int oldfd, int newfd);

#endif  // WRAPPERS_H
