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
        char *home = getenv("HOME");
        if (home) 
		{
            if (chdir(home) != 0)
                perror("cd"); // Print error if chdir fails
        } 
		else
            fprintf(stderr, "cd: HOME environment variable is not set\n");
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
		const char *error_msg = "unset: Missing argument\n";
		write(STDERR_FILENO, error_msg, strlen(error_msg));
		return;
	}
	char **env = environ;
	while (*env)
	{
		if (strncmp(*env, argv[1], ft_strlen(argv[1])) == 0 && (*env)[strlen(argv[1])] == '=')
		{
			char **next = env;
			while (*next)
            {
                *next = *(next + 1);
                next++;
            }
            break;
        }
        env++;
		}
}

void handle_env(char **argv)
{
	extern char **environ;
	char **env = environ;

	while (*env)
	{
		printf("%s\n", *env);
		env++;
	}
}
void handle_pwd(char **argv)
{
	char *pwd = getenv("PWD");
    if (pwd)
        printf("%s\n", pwd);
    else
	{
        const char *error_msg = "pwd: Unable to retrieve current directory\n";
    	write(STDERR_FILENO, error_msg, strlen(error_msg));
	}
}

void handle_echo(char **argv)
{
	int newline = 1;
	int i = 1;
	if (argv[1] && strcmp(argv[1], "-n") == 0)
	{
		newline = 0;
		i = 2;
	}
	while (argv[i])
	{
		if (i > 2 || (i > 1 && newline))
			printf(" ");
		printf("%s", argv[i]);
		i++;
	}
	if (newline)
		printf("\n");
}


