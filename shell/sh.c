#include "defs.h"
#include "types.h"
#include "readline.h"
#include "runcmd.h"

char *itoa_safe(int num, char *buf, size_t buf_size);
void print_status(pid_t pid, int local_status);
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
itoa_safe(int num, char *buf, size_t buf_size)
{
	int i = buf_size - 1;
	buf[i] = 0;

	if (num == 0) {
		buf[i - 1] = '0';
		return buf + i - 1;
	}

	i--;

	for (int a = abs(num); a != 0; i--) {
		buf[i] = (char) ((a % 10) + 48);
		a /= 10;
	}

	if (num < 0) {
		buf[i] = '-';
		return buf + i;
	}

	return buf + i + 1;
}

void
print_status(pid_t pid, int local_status)
{
	char pid_buf[13];     // Enough for 32-bit int
	char status_buf[13];  // Enough for 32-bit int
	char *pid_str;
	char *status_str;

	write(STDOUT_FILENO, "PID: ", 5);
	pid_str = itoa_safe(pid, pid_buf, 13);
	write(STDOUT_FILENO, pid_str, strlen(pid_str));
	write(STDOUT_FILENO, " STATUS: ", 9);
	status_str = itoa_safe(local_status, status_buf, 13);
	write(STDOUT_FILENO, status_str, strlen(status_str));
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
	          &(struct sigaction) { .sa_handler = sigchild_handler,
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
