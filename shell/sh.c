#include "defs.h"
#include "types.h"
#include "readline.h"
#include "runcmd.h"

void sigchild_handler(int signum);

char prompt[PRMTLEN] = { 0 };

pid_t shell_pid;

// runs a shell command
static void
run_shell()
{
	char *cmd;

	while ((cmd = read_line(prompt)) != NULL)
		if (run_cmd(cmd) == EXIT_SHELL)
			return;
}

void
sigchild_handler(const int signum)
{
	int local_status;
	const pid_t pid = waitpid(0, &local_status, WNOHANG);

	if (pid <= 0)
		return;

	fprintf(stdout, "PID: %d STATUS: %d finished\n", pid, status);
}


// initializes the shell
// with the "HOME" directory
static void
init_shell()
{
	char buf[BUFLEN] = { 0 };
	char *home = getenv("HOME");

	if (chdir(home) < 0) {
		snprintf(buf, sizeof buf, "cannot cd to %s ", home);
		perror(buf);
	} else {
		snprintf(prompt, sizeof prompt, "(%s)", home);
	}


	sigaction(SIGCHLD,
	          &(struct sigaction){ .sa_handler = sigchild_handler,
	                               .sa_flags = SA_RESTART },
	          NULL);
}

int
main(void)
{
	shell_pid = getpid();

	init_shell();

	run_shell();

	return 0;
}
