#include "../include/minishell.h"

char *find_executable()
{
	char *path = getenv("PATH");
	if (!path)
		
}

int is_builtin(char **argv)
{
	if (!argv || !argv[0])
        return 0;
    return (strcmp(argv[0], "exit") == 0 ||
            strcmp(argv[0], "cd") == 0 ||
            strcmp(argv[0], "export") == 0 ||
            strcmp(argv[0], "unset") == 0 ||
            strcmp(argv[0], "env") == 0 ||
            strcmp(argv[0], "pwd") == 0 ||
            strcmp(argv[0], "echo") == 0);
}
}

void execute_builtin(char **argv)
{
	if (strcmp(argv[0], "exit") == 0)
		handle_exit(argv);
	else if (strcmp(argv[0], "cd") == 0)
		handle_cd(argv);
	else if (strcmp(argv[0], "unset") == 0)
		handle_unset(argv);
	else if (strcmp(argv[0], "env") == 0)
		handle_env(argv);
	else if (strcmp(argv[0], "pwd") == 0)
		handle_pwd(argv);
	else if (strcmp(argv[0], "echo") == 0)
		handle_echo(argv);
}


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
void execute_pipeline(command_t *cmd)
{
	int pipe_fds[2];
	pid_t pid;
	int prev_fd = -1; // I don't get this one..

	while (cmd)
	{
		if (is_builtin(cmd->argv[0])) //does this check only the first command or what?? this is important. 
			execute_builtin(cmd->argv);

		if (cmd->next)
		{
			if (pipe(pipe_fds) == -1)
			{
				perror("pipe");
				exit(1);
			}
		}

		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(1);
		}
		if (pid == 0)
		{
			if (prev_fd != -1) // i don't know how that means checking for a previous pipe...
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}

			if (cmd->next)
			{
				close(pipe_fds[0]);
				dup2(pipe_fds[1], STDOUT_FILENO);
				close(pipe_fds[1]);
			}
			execvp(cmd->argv[0], cmd->argv); //execve or ends with p? check the difference...
			perror("execvp");
			exit(1);
		}
		else { // Parent process
            if (prev_fd != -1) {
                close(prev_fd); // Close the previous read-end
            }
            if (cmd->next) {
                close(pipe_fds[1]); // Close write-end of the current pipe
                prev_fd = pipe_fds[0]; // Save read-end for the next command
            }

            // Wait for the child process (optional, depending on desired behavior)
            waitpid(pid, NULL, 0);
        }

        cmd = cmd->next; // Move to the next command in the pipeline

	}
}
}
void execute_commands(command_t *cmd)
{
	if (!cmd)
		return;

	// 1) detect if pipeline or simple command
	// 2) execute accordingly

	if (cmd->type == CMD_PIPE || cmd->next != NULL)
	{
		execute_pipeline(cmd);
	}
	else if (cmd->type == CMD_SIMPLE)
	{
		execute_simple_command(cmd);
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
