#include "runcmd.h"

int status = 0;
struct cmd *parsed;

// runs the command in 'cmd'
int
run_cmd(char *cmd)
{
	pid_t p;

	// if the "enter" key is pressed
	// just print the prompt again
	if (cmd[0] == END_STRING)
		return 0;

	// "history" built-in call
	if (history(cmd))
		return 0;

	// "cd" built-in call
	if (cd(cmd))
		return 0;

	// "exit" built-in call
	if (exit_shell(cmd))
		return EXIT_SHELL;

	// "pwd" built-in call
	if (pwd(cmd))
		return 0;

	// parses the command line
	parsed = parse_line(cmd);

	// forks and run the command
	if ((p = fork()) == 0) {
		if (parsed->type != BACK)
			setpgid(0, 0);

		exec_cmd(parsed);
	}

	// stores the pid of the process
	parsed->pid = p;

	// background process special treatment
	// Hint:
	// - check if the process is
	//		going to be run in the 'back'
	// - print info about it with
	// 	'print_back_info()'
	if (parsed->type == BACK)
		print_back_info(parsed);
	else {
		waitpid(p, &status, 0);  // waits for the process to finish
		print_status_info(parsed);
	}

	free_command(parsed);

	return 0;
}
