#include "builtin.h"

// returns true if the 'exit' call
// should be performed
//
// (It must not be called from here)
int
exit_shell(char *cmd)
{
	if (strncmp(cmd, "exit", 4) == 0)
		return 1;
	return 0;
}

// returns true if "chdir" was performed
//  this means that if 'cmd' contains:
// 	1. $ cd directory (change to 'directory')
// 	2. $ cd (change to $HOME)
//  it has to be executed and then return true
//
//  Remember to update the 'prompt' with the
//  	new directory.
//
// Examples:
//  1. cmd = ['c','d', ' ', '/', 'b', 'i', 'n', '\0']
//  2. cmd = ['c','d', '\0']
int
cd(char *cmd)
{
	if (strncmp(cmd, "cd", 2) != 0) {
		return 0;
	}

	strtok(cmd, " ");
	char *dir = strtok(NULL, " ");
	if (dir == NULL || *dir == 0) {
		dir = getenv("HOME");
	}

	if (strcmp(dir, ".") == 0)
		return 1;


	if (strcmp(dir, "..") == 0) {
		char *cwd = getcwd(NULL, 0);

		char *aux = NULL;
		for (char *c = strtok(cwd, "/"); c != NULL; c = strtok(NULL, "/")) {
			aux = c;
		}

		if (aux != NULL)
			*aux = '\0';

		strncpy(prompt, aux, strlen(cwd) > 1024 ? 1024 : strlen(cwd));
		return 1;
	}

	if (chdir(dir) == 0) {
		char *cwd = getcwd(NULL, 0);
		if (cwd != NULL) {
			strncpy(prompt,
			        cwd,
			        strlen(cwd) > 1024 ? 1024 : strlen(cwd));
			free(cwd);
		} else {
			perror("Error in getcwd");
		}
		return 1;
	}

	return 1;
}

// returns true if 'pwd' was invoked
// in the command line
//
// (It has to be executed here and then
// 	return true)
int
pwd(char *cmd)
{
	if (strncmp(cmd, "pwd", 3) == 0) {
		char *cwd = getcwd(NULL, 0);
		printf("%s\n", cwd);
		free(cwd);
		return 1;
	}

	return 0;
}

// returns true if `history` was invoked
// in the command line
//
// (It has to be executed here and then
// 	return true)
int
history(char *cmd)
{
	// Your code here

	return 0;
}
