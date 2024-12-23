#include "../include/minishell.h"

// going anywhere for christmas? 
// 
void free_split(char **split)
{
    int i = 0;
	while (split[i])
		free(split[i++]);
    free(split);
}

void execute_simple_command(command_t *cmd)
{
	extern char **environ; // why is it a 2darray
	if (!cmd || !cmd->argv || !cmd->argv[0])
        return;
}
void execute_commands(command_t *cmd)
{
	if (!cmd)
		return;

	if (cmd->type == CMD_SIMPLE)
	{
		execute_simple_command(cmd);
	}
	else if (cmd->type == CMD_PIPE)
	{
		execute_pipeline(cmd);
	}
}

	
	char **argv = ft_split(input, ' ');
	if (!argv || !argv[0])
	{
		free_split(argv);
		return;
	}
	if (strcmp(argv[0], "exit") == 0)
	{
		free_split(argv);
		exit(0);
	}
	//fork and execute
	pid_t pid = fork();
	if (pid < 0)
	{
		perror("fork");
	}
	else if (pid == 0)
	{
		if (execve(argv[0], argv, environ) == -1)
		{
			perror("execvp");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		int status;
		waitpid(pid, &status, 0);
	}

}

int main(void)
{
    char *input;

    // Set up signal handling
    set_signals_interactive();

    while (1)
    {
        input = readline("minishell> ");
        if (!input) // Handle EOF (Ctrl+D)
        {
            printf("exit\n");
            break;
        }
        if (*input)
            add_history(input); // Add non-empty input to history
        
		execute_command(input);
        free(input);
    }
    return 0;
}
