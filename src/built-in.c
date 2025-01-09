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
	(void)argv;
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
	(void)argv;
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
	(void)argv;
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
	int i = 1; //start from 1 bec. 0 is the command itself
	int newline = 1; //default is to print newline
	int enable_escape = 0; //default is to disable escape

	// 1) parse flags
	while (argv[i] && argv[i][0] == '-' && argv[i][1] != '\0')
	{
		int j = 1;
		int valid_flag = 1; //default is to assume the flag is valid
		while (argv[i][j])
		{
			if (argv[i][j] == 'n')
				newline = 0;
			else if (argv[i][j] == 'e')
				enable_escape = 1;
			else if (argv[i][j] == 'E')
				enable_escape = 0;
			else
			{
				valid_flag = 0;
				break;
			}
			j++;
		}
		if (!valid_flag)
			break;
		i++;
	}

	// 2) print arguments
	int first_arg = 1;
	while (argv[i])
	{
		if (!first_arg)
			printf(" ");
		else
			first_arg = 0;
		
		if (enable_escape)
		{
			char *expanded = expand_escape(argv[i]);
			if (expanded)
			{
				printf("%s", expanded);
				free(expanded);
			}
			else
				printf("%s", argv[i]);
		}
		else
			printf("%s", argv[i]);
		i++;
	}
	if (newline)
		printf("\n");
}

// that we pass

char *expand_escape(const char *str)
{
	char *expanded = ft_strdup("");
	char *tmp = NULL;
	size_t i = 0;

	while (str && str[i])
	{
		if (str[i] == '\\' && str[i + 1])
		{
			i++;
			if (str[i] == 'n')
				tmp = ft_strjoin(expanded, "\n");
			else if (str[i] == 't')
				tmp = ft_strjoin(expanded, "\t");
			else
			{
				char onechar[2] = {str[i], '\0'};
				tmp = ft_strjoin(expanded, onechar);
			}
			free(expanded);
			expanded = tmp;
		}
		else
		{
			char onechar[2] = {str[i], '\0'};
			tmp = ft_strjoin(expanded, onechar);
			free(expanded);
			expanded = tmp;
		}
		i++;
	}
	return expanded;
}


