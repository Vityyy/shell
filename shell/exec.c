#include "exec.h"
#include <stdnoreturn.h>

#define IN_FILE_FLAGS O_RDONLY | O_CLOEXEC
#define OUT_FILE_FLAGS O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC
#define ERR_FILE_FLAGS O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC

/**
 * * @param p * @param previous_read_fd
 */
noreturn void generate_pipeline(struct pipecmd *p, int previous_read_fd);

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
		char key[idx];
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
		_dup2(open_redir_fd(in_file, IN_FILE_FLAGS), STDIN_FILENO);

	if (strlen(out_file) > 0)
		_dup2(open_redir_fd(out_file, OUT_FILE_FLAGS), STDOUT_FILENO);

	if (strlen(err_file) > 0) {
		if (strcmp(err_file, "&1") == 0)
			_dup2(STDOUT_FILENO, STDERR_FILENO);
		else
			_dup2(open_redir_fd(err_file, ERR_FILE_FLAGS),
			      STDERR_FILENO);
	}
}

noreturn void
generate_pipeline(struct pipecmd *p, int previous_read_fd)
{
	struct execcmd *el = (struct execcmd *) p->leftcmd;
	struct execcmd *er = (struct execcmd *) p->rightcmd;
	pid_t left_child_pid;
	pid_t right_child_pid;

	int fds[2];
	_pipe(fds);

	left_child_pid = _fork();
	if (left_child_pid == 0) {
		set_environ_vars(el->eargv, el->eargc);

		_close(fds[READ]);
		_dup2(previous_read_fd, STDIN_FILENO);
		_close(previous_read_fd);
		_dup2(fds[WRITE], STDOUT_FILENO);
		_close(fds[WRITE]);
		_execvp(el->argv[0], el->argv);
	}

	_close(previous_read_fd);
	_close(fds[WRITE]);

	if (p->rightcmd->type == PIPE) {
		right_child_pid = _fork();
		if (right_child_pid == 0)
			generate_pipeline((struct pipecmd *) p->rightcmd,
			                  fds[READ]);
	} else {
		right_child_pid = _fork();
		if (right_child_pid == 0) {
			set_environ_vars(er->eargv, er->eargc);
			_dup2(fds[READ], STDIN_FILENO);
			_close(fds[READ]);
			_execvp(er->argv[0], er->argv);
		}
	}

	_close(fds[READ]);

	_wait(NULL);
	_wait(NULL);
	_exit(0);
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

		// const pid_t pid = _fork();
		// if (pid == 0)
		// 	_execvp(e->argv[0], e->argv);
		// _wait(NULL);

		_execvp(e->argv[0], e->argv);

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

		// const pid_t pid = _fork();
		// if (pid == 0) {
		// 	perform_redirections(r->in_file, r->out_file,
		// r->err_file); 	_execvp(r->argv[0], r->argv);
		// }
		// _wait(NULL);

		perform_redirections(r->in_file, r->out_file, r->err_file);
		_execvp(r->argv[0], r->argv);

		break;
	}

	case PIPE: {
		p = (struct pipecmd *) cmd;
		struct execcmd *left = (struct execcmd *) p->leftcmd;
		struct cmd *right = p->rightcmd;

		pid_t left_child_pid;
		pid_t right_child_pid;
		int fds[2];
		_pipe(fds);

		left_child_pid = _fork();
		if (left_child_pid == 0) {
			set_environ_vars(left->eargv, left->eargc);

			_close(fds[READ]);
			_dup2(fds[WRITE], STDOUT_FILENO);
			_close(fds[WRITE]);
			_execvp(left->argv[0], left->argv);
		}

		_close(fds[WRITE]);

		if (right->type == PIPE) {
			right_child_pid = _fork();
			if (right_child_pid == 0)
				generate_pipeline((struct pipecmd *) right,
				                  fds[READ]);
		} else {
			right_child_pid = _fork();
			if (right_child_pid == 0) {
				set_environ_vars(((struct execcmd *) right)->eargv,
				                 ((struct execcmd *) right)->eargc);
				_dup2(fds[READ], STDIN_FILENO);
				_close(fds[READ]);
				_execvp(((struct execcmd *) right)->argv[0],
				        ((struct execcmd *) right)->argv);
			}
		}

		_close(fds[READ]);
		_wait(NULL);
		_wait(NULL);
	}
	}
}
