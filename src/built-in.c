#include "../include/minishell.h"


void handle_exit(char **argv)
{
    int status = 0;
    if (argv[1])
        status = atoi(argv[1]);
    exit(status);
}
void handle_cd(char **argv)
{
	if (!argv[1])
	{
		printf("cd: Missing argument\n"); // change this to home dir.. 
		return;
	}
	if (chdir(argv[1]) != 0)
		printf("cd: no such file or directory: %s", argv[1]); // will come back to it to change to stderror
}

void handle_unset(char **argv)
{
	extern char **environ;
	if (!argv[1])
	{
		printf("unset: Missing argument\n"); // can we use perror or shall we use some other one here?
		return;
	}
	char **env = environ;
	while (*env)
	{
		if (strncmp(*env, argv[1], ft_strlen(argv[1])) == 0 && (*env))
	}
}
void handle_env(char **argv);
void handle_pwd(char **argv);
void handle_echo(char **argv);