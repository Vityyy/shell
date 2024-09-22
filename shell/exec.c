#include "exec.h"
#include <stdnoreturn.h>

#define IN_FILE_FLAGS O_RDONLY | O_CLOEXEC
#define OUT_FILE_FLAGS O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC
#define ERR_FILE_FLAGS O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC

/**
 * * @param in_file
 * @param out_file * @param err_file
 */
void perform_redirections(char *in_file, char *out_file, char *err_file);

// sets "key" with the key part of "arg"
// and null-terminates it
//
// Example:
//  - KEY=value
//  arg = ['K', 'E', 'Y', '=', 'v', 'a', 'l', 'u', 'e', '\0']
//  key = "KEY"
//
static void
get_environ_key(char *arg, char *key)
{
	int i;
	for (i = 0; arg[i] != '='; i++)
		key[i] = arg[i];

	key[i] = END_STRING;
}

// sets "value" with the value part of "arg"
// and null-terminates it
// "idx" should be the index in "arg" where "=" char
// resides
//
// Example:
//  - KEY=value
//  arg = ['K', 'E', 'Y', '=', 'v', 'a', 'l', 'u', 'e', '\0']
//  value = "value"
//
static void
get_environ_value(char *arg, char *value, int idx)
{
	size_t i, j;
	for (i = (idx + 1), j = 0; i < strlen(arg); i++, j++)
		value[j] = arg[i];

	value[j] = END_STRING;
}

// sets the environment variables received
// in the command line
//
// Hints:
// - use 'block_contains()' to
// 	get the index where the '=' is
// - 'get_environ_*()' can be useful here
static void
set_environ_vars(char **eargv, int eargc)
{
	for (int i = 0; i < eargc; i++) {
		const int idx = block_contains(eargv[i], '=');
		char key[idx + 1];
		char value[strlen(eargv[i]) - idx];

		get_environ_key(eargv[i], key);
		get_environ_value(eargv[i], value, idx);

		setenv(key, value, 1);
	}
}

// opens the file in which the stdin/stdout/stderr
// flow will be redirected, and returns
// the file descriptor
//
// Find out what permissions it needs.
// Does it have to be closed after the execve(2) call?
//
// Hints:
// - if O_CREAT is used, add S_IWUSR and S_IRUSR
// 	to make it a readable normal file
static int
open_redir_fd(char *file, int flags)
{
	int fd;
	if (flags & O_CREAT)
		fd = open(file, flags, S_IRUSR | S_IWUSR);
	else
		fd = open(file, flags);

	if (fd < 0) {
		perror(NULL);
		exit(-1);
	}

	return fd;
}

void
perform_redirections(char *in_file, char *out_file, char *err_file)
{
	if (strlen(in_file) > 0)
		sdup2(open_redir_fd(in_file, IN_FILE_FLAGS), STDIN_FILENO);

	if (strlen(out_file) > 0)
		sdup2(open_redir_fd(out_file, OUT_FILE_FLAGS), STDOUT_FILENO);

	if (strlen(err_file) > 0) {
		if (strcmp(err_file, "&1") == 0)
			sdup2(STDOUT_FILENO, STDERR_FILENO);
		else
			sdup2(open_redir_fd(err_file, ERR_FILE_FLAGS),
			      STDERR_FILENO);
	}
}

// executes a command - does not return
//
// Hint:
// - check how the 'cmd' structs are defined
// 	in types.h
// - casting could be a good option
void
exec_cmd(struct cmd *cmd)
{
	// To be used in the different cases
	struct execcmd *e;
	struct backcmd *b;
	struct execcmd *r;
	struct pipecmd *p;

	switch (cmd->type) {
	case EXEC: {
		e = (struct execcmd *) cmd;

		set_environ_vars(e->eargv, e->eargc);

		sexecvp(e->argv[0], e->argv);

		break;
	}

	case BACK: {
		// runs a command in background
		//
		// Your code here
		printf("Background process are not yet implemented\n");
		_exit(-1);
		break;
	}

	case REDIR: {
		// changes the input/output/stderr flow
		//
		// To check if a redirection has to be performed
		// verify if file name's length (in the execcmd struct)
		// is greater than zero
		r = (struct execcmd *) cmd;

		set_environ_vars(r->eargv, r->eargc);

		perform_redirections(r->in_file, r->out_file, r->err_file);

		sexecvp(r->argv[0], r->argv);

		break;
	}

	case PIPE: {
		p = (struct pipecmd *) cmd;
		int prfd = p->prev_read_fd;
		int fds[2];
		spipe(fds);

		if (sfork() == 0) {
			if (prfd >= 0) {
				sdup2(prfd, STDIN_FILENO);
				sclose(prfd);
			}
			sdup2(fds[WRITE], STDOUT_FILENO);
			sclose(fds[WRITE]);
			sclose(fds[READ]);
			exec_cmd(p->leftcmd);
		}

		if (prfd >= 0)
			sclose(prfd);
		sclose(fds[WRITE]);

		if (sfork() == 0) {
			if (p->rightcmd->type != PIPE) {
				sdup2(fds[READ], STDIN_FILENO);
				sclose(fds[READ]);
			} else
				((struct pipecmd *) p->rightcmd)->prev_read_fd =
				        fds[READ];
			exec_cmd(p->rightcmd);
		}

		sclose(fds[READ]);

		swait(NULL);
		swait(NULL);
		_exit(EXIT_SUCCESS);
	}
	}
}
