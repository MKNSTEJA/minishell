#include "../include/minishell.h"


void free_split(char **split)
{
    int i = 0;
	while (split[i])
		free(split[i++]);
    free(split);
}

int is_builtin(command_t *cmd)
{
	if (cmd->argv == 'cd' || cmd->arg == )
	{

	}
}
char *find_executable()
{
	char *path = getenv("PATH");
	if (!path)
		
}
// THIS IS WHERE YOU ARE WORKING
void execute_simple_command(command_t *cmd)
{
	extern char **environ;
	if (!cmd || !cmd->argv || !cmd->argv[0])
        return;

	if (is_builtin(cmd->argv)) // implement the function to know if it is built in
	{
		execute_builtin(cmd); // implement this too (will not use execve.. but how will it execute?)
		return;
	}

	// handle non-built in functions
		// search for it in the non-executable.. in the path
	find_executable(cmd->argv)
	
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
        
		execute_commands(input);
        free(input);
    }
    return 0;
}
