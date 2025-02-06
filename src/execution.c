

#include "../include/minishell.h"


void execute_commands(t_op *cmd, t_data *data)
{
    if (cmd && cmd->next != NULL)
        execute_pipeline(cmd, data);
    else
        execute_simple_command(cmd, data);
}

char *expand_line(char *line, char **envp)
{
	return expand_one_token(line, envp, DQ);
}


char *find_executable(char **argv, char **envp)
{
	if (argv[0][0] == '/' || argv[0][0] == '.')
    {
        if (access(argv[0], X_OK) == 0)
            return strdup(argv[0]);
        else
            return NULL;
    }
	char *executable_path = NULL;
	// char *path = getenv("PATH"); 
	char *path = get_env_value("PATH", envp);
	if (!path)
	{
		// write(2, "PATH not set\n", 13);
    	// return NULL;
		path = "/bin:/usr/bin";
	}
	char **split_path = ft_split(path, ':');
	// printf("split_path: %s\n", *split_path);
	if (!split_path)
		return NULL;
	char **sp = split_path;
	while (*sp)
	{
		// printf("sp: %s\n", *sp);
		char *string_slash = ft_strjoin(*sp, "/");
		char *full_path = ft_strjoin(string_slash, argv[0]);
		free(string_slash);
		if (access(full_path, X_OK) == 0)
		{
			executable_path = full_path;
			break;
		}
		free(full_path);
		sp++;
	}
	char **tmp = split_path;
	while (tmp && *tmp)
	{
		free(*tmp);
		*tmp = NULL;
		tmp++;
	}
	free(split_path);
	// tmp = NULL;
	return executable_path;
}

void execute_builtin(t_op *cmd, t_data *data)
{
	char **argv = cmd->str;
	if (!argv || !argv[0])
        return;
	if (strcmp(argv[0], "exit") == 0)
		handle_exit(argv);
	else if (strcmp(argv[0], "cd") == 0)
		handle_cd(argv, &(data->env));
	else if (strcmp(argv[0], "unset") == 0)
		handle_unset(argv, data->env);
	else if (strcmp(argv[0], "env") == 0)
		handle_env(argv, data);
	else if (strcmp(argv[0], "pwd") == 0)
		handle_pwd(argv, data->env);
	else if (strcmp(argv[0], "echo") == 0)
		handle_echo(argv);
	else if (strcmp(argv[0], "export") == 0)
		handle_export(argv, data);
}

void execute_simple_command(t_op *cmd, t_data *data) 
{
    if (!cmd || !cmd->str || !cmd->str[0])
	{
		return;
	}
        // Save original FDs
	int saved_stdin = dup(STDIN_FILENO);
	int saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin < 0 || saved_stdout < 0) 
	{
		perror("dup");
		return;
    }

	if (apply_redirections(cmd, data->env) < 0)
	{
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return;
	}
	if (is_builtin(cmd))
		execute_builtin(cmd, data);
	else
	{
		pid_t pid = fork();
		if (pid < 0)
			perror("fork");
		else if (pid == 0)
		{
			char *exec_path = find_executable(cmd->str, data->env);
			if (!exec_path)
			{
				fprintf(stderr, "%s: command not found\n", cmd->str[0]);
				_exit(127);
			}
			// fprintf(stderr, "Executing: %s\n", exec_path);
			execve(exec_path, cmd->str, data->env);
			perror("execve");
			_exit(1);
		}
		else //parent
		{
			int status;
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				g_exit_code = WEXITSTATUS(status);
			else
				g_exit_code = 1;
		}
	}
	// Restore FDs
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}


void execute_pipeline(t_op *cmd, t_data *data)
{
    int pipeline_length = count_commands(cmd);
    pid_t *pids = malloc(sizeof(pid_t) * pipeline_length);
    if (!pids)
    {
        perror("malloc");
        exit(1);
    }

    int pipe_fds[2];
    pid_t pid;
    int i = 0;
    int prev_fd = -1;
    t_op *current = cmd;
	pid_t last_pid = 0;
    while (current)
    {
        if (current->next)
        {
            if (pipe(pipe_fds) == -1)
            {
                perror("pipe");
                free(pids);
                exit(1);
            }
        }

        pid = fork();
        if (pid == -1) 
        {
            perror("fork");
            free(pids);
            exit(1);
        }

        if (pid == 0) // Child
        {
            if (apply_redirections(current, data->env) < 0)
				_exit(1);
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}
			// if there's a next command, set up our stdout to the pipe
            if (current->next)
            {
                close(pipe_fds[0]);
                dup2(pipe_fds[1], STDOUT_FILENO);
                close(pipe_fds[1]);
            }
            if (is_builtin(current))
			{
				execute_builtin(current, data);
				_exit(0);
			}
            else
            {
				// external
				char *exec_path = find_executable(current->str, data->env);
				if (!exec_path)
				{
                    fprintf(stderr, "%s: command not found\n", current->str[0]);
                    _exit(127);
                }
                execve(exec_path, current->str, data->env);
                perror("execve");
                _exit(1);
            }
        }
        else // Parent
        {
            pids[i++] = pid; // Store child PID
			last_pid = pid;

            if (prev_fd != -1)
                close(prev_fd);

            if (current->next)
            {
                close(pipe_fds[1]);
                prev_fd = pipe_fds[0];
            }
        }
        current = current->next;
    }

    // Wait for all children
	int status = 0;
    for (int j = 0; j < pipeline_length; j++)
	{
		int child_status;
		waitpid(pids[j], &child_status, 0);
		if (pids[j] == last_pid)
		{
			status = child_status;
		}
		if (WIFEXITED(status))
			g_exit_code = WEXITSTATUS(status);
		else
			g_exit_code = 1;
	}	
	free(pids);

}