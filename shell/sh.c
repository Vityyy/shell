#include "defs.h"
#include "types.h"
#include "readline.h"
#include "runcmd.h"

void sigchild_handler(int signum);

char prompt[PRMTLEN] = { 0 };

stack_t ss = { 0 };

// runs a shell command
static void
run_shell()
{
	char *cmd;

	while ((cmd = read_line(prompt)) != NULL)
		if (run_cmd(cmd) == EXIT_SHELL)
			return;
}

char *
itoa(int num)
{
	int i = 0;

	if (num == 0)
		i = 1;
	else
		for (int a = num; a != 0; i++)
			a /= 10;

	char *s = malloc(i + 1);
	s[i] = 0;

	for (int a = num; i != 0; i--) {
		s[i - 1] = (char) ((a % 10) + 48);
		a /= 10;
	}

	return s;
}

void
print_status(pid_t pid, int local_status)
{
	char *str_num = itoa(pid);
	write(STDOUT_FILENO, "PID: ", 5);
	write(STDOUT_FILENO, str_num, strlen(str_num));
	free(str_num);
	write(STDOUT_FILENO, " STATUS: ", 9);
	str_num = itoa(local_status);
	write(STDOUT_FILENO, str_num, strlen(str_num));
	free(str_num);
	write(STDOUT_FILENO, " finished\n", 10);
}

void
sigchild_handler(const int signum)
{
	int local_status;
	const pid_t pid = waitpid(0, &local_status, WNOHANG);

	if (pid <= 0)
		return;

	print_status(pid, local_status);
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

	ss.ss_sp = malloc(SIGSTKSZ);
	ss.ss_size = SIGSTKSZ;
	sigaltstack(&ss, NULL);

	sigaction(SIGCHLD,
	          &(struct sigaction){ .sa_handler = sigchild_handler,
	                               .sa_flags = SA_RESTART | SA_ONSTACK },
	          NULL);
}

int
main(void)
{
	init_shell();

	run_shell();

	free(ss.ss_sp);

	return 0;
}
