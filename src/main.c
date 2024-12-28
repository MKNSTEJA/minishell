#include "../include/minishell.h"


char *find_executable(char **argv) // 
{
	if (argv[0][0] == '/' || argv[0][0] == '.')
    {
        if (access(argv[0], X_OK) == 0)
            return strdup(argv[0]);
        else
            return NULL;
    }
	char *executable_path = NULL;
	char *path = getenv("PATH"); 
	if (!path)
	{
		write(2, "PATH not set\n", 13);
    	return NULL;
	}
	char **split_path = ft_split(path, ':');
	if (!split_path)
		return NULL;
	while (*split_path)
	{
		char *string_slash = ft_strjoin(split_path, "/");
		char *full_path = ft_strjoin(string_slash, argv[0]);
		free(string_slash);
		if (access(full_path, X_OK) == 0)
		{
			executable_path = full_path;
			break;
		}
		free(full_path);
		split_path++;
	}
	char **tmp = split_path;
	while (*tmp)
	{
		free(*tmp);
		tmp++;
	}
	free(split_path);
	return executable_path;
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
	if (!cmd || !cmd->argv || !cmd->argv[0])
        return;

	if (is_builtin(cmd->argv))
	{
		int saved_stdin = dup(STDIN_FILENO);
		int saved_stdout = dup(STDOUT_FILENO);
		if (saved_stdin < 0 || saved_stdout < 0) // important (handling errors, and knowing when to handle them)
        {
            perror("dup");
            return;
        }

		if (cmd->infile)
		{
			int fd_in = open(cmd->infile, O_RDONLY);
			if (fd_in < 0)
			{	
				perror(cmd->infile);
                // restore FDs and return
                dup2(saved_stdin, STDIN_FILENO);
                dup2(saved_stdout, STDOUT_FILENO);
                close(saved_stdin);
                close(saved_stdout);
                return;
			}
			dup2(fd_in, STDIN_FILENO);
			close(fd_in);
		}
		if (cmd->outfile)
		{
			int flags = O_WRONLY | O_CREAT | (cmd->append ? O_APPEND : O_TRUNC);
			int fd_out = open(cmd->outfile, flags, 0644);
			if (fd_out < 0)
			{
                perror(cmd->outfile);
                // restore FDs and return
                dup2(saved_stdin, STDIN_FILENO);
                dup2(saved_stdout, STDOUT_FILENO);
                close(saved_stdin);
                close(saved_stdout);
                return;
            }
			dup2(fd_out, STDOUT_FILENO);
			close(fd_out);
		}
			execute_builtin(cmd);
			dup2(saved_stdin, STDIN_FILENO);
			dup2(saved_stdout, STDOUT_FILENO);
			close(saved_stdin);
			close(saved_stdout);
			return;
	}
	else
	{
		char *executable_path = find_executable(cmd->argv);
		if (!executable_path)
		{
            fprintf(stderr, "%s: command not found\n", cmd->argv[0]);
            return;
        }

		pid_t pid = fork();
		if (pid < 0)
		{
			perror("fork");
			free(executable_path);
			return;
		}
		if (pid == 0)
		{
			if (cmd->infile)
			{ 
				int fd_in = open(cmd->infile, O_RDONLY);
				if (fd_in < 0)
				{	
					perror(cmd->infile);
					_exit(1);
				}
				dup2(fd_in, STDIN_FILENO);
				close(fd_in);
			}
		
		if (cmd->outfile)
		{
			int flags = O_WRONLY | O_CREAT | (cmd->append ? O_APPEND : O_TRUNC);
			int fd_out = open(cmd->outfile, flags, 0644);
			if (fd_out < 0)
			{
                perror(cmd->outfile);
                _exit(1);
            }
			dup2(fd_out, STDOUT_FILENO);
			close(fd_out);
		}
		execvp(executable_path, cmd->argv);
		perror("execvp");
		_exit(127);
		}
		else
		{
			//parent process
			free(executable_path);
			waitpid(pid, NULL, 0);
		}
	}
}
void execute_pipeline(command_t *cmd)
{
	int pipe_fds[2];
	pid_t pid;
	pid_t pids[PIPELINE_MAX]; // i don't think bash has a pipeline max
	int i = 0;
	int prev_fd = -1;

	while (cmd)
	{
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
			if (prev_fd != -1)
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
			if (is_builtin(cmd->argv[0]))
			{
				execute_builtin(cmd->argv);
				_exit(0); //terminate child without cleanup for parent
			}
			else
			{
				execvp(cmd->argv[0], cmd->argv);
				perror("execvp");
				_exit(1);
			}
		}
		else 
		{ // Parent process
			pids[i++] = pid;
            if (prev_fd != -1) 
                close(prev_fd);
            if (cmd->next) 
			{
                close(pipe_fds[1]);
                prev_fd = pipe_fds[0]; // Save read-end for the next command
            }
        }
        cmd = cmd->next; // Move to the next command in the pipeline
	}
	int j = 0;

	while (j < i)
		waitpid(pids[j++], NULL, 0);
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
    // Instead of real input, let's just test the execution code directly:
    command_t *cmd = NULL;

    // Option 1: Test a simple command (e.g., "ls -la")
    cmd = mock_simple_command();
    // or Option 2: Test a pipeline (e.g., "cat | wc")
    // cmd = mock_pipeline();

    // Now just call your execute_functions
    execute_commands(cmd);

    // Possibly free the allocated command structures
    // e.g. free_command_list(cmd); // if you have a function for that
    // but for a quick test, you can skip it or write it later.

    return 0;
}

// int main(void)
// {
//     char *input;

//     // Set up signal handling
//     set_signals_interactive();

//     while (1)
//     {
//         input = readline("minishell> ");
//         if (!input) // Handle EOF (Ctrl+D)
//         {
//             printf("exit\n");
//             break;
//         }
//         if (*input)
//             add_history(input); // Add non-empty input to history
        
// 		execute_commands(input);
//         free(input);
//     }
//     return 0;
// }
