

#include "../include/minishell.h"


void execute_commands(t_op *cmd)
{
    // if we see multiple commands, we assume pipeline
    if (cmd && cmd->next != NULL)
        execute_pipeline(cmd);
    else
        execute_simple_command(cmd);
}
int apply_redirections(t_op *cmd)
{
    t_redir *redir = cmd->redirections;
    while (redir)
    {
        if (redir->type == IN)  // "<"
        {
            int fd_in = open(redir->filename, O_RDONLY);
            if (fd_in < 0)
            {
                perror(redir->filename);
                return (-1);
            }
            dup2(fd_in, STDIN_FILENO);
            close(fd_in);
        }
        else if (redir->type == OUT) // ">"
        {
            int fd_out = open(redir->filename,
                              O_WRONLY | O_CREAT | O_TRUNC,
                              0644);
            if (fd_out < 0)
            {
                perror(redir->filename);
                return (-1);
            }
            dup2(fd_out, STDOUT_FILENO);
            close(fd_out);
        }
        else if (redir->type == APPEND) // ">>"
        {
            int fd_out = open(redir->filename,
                              O_WRONLY | O_CREAT | O_APPEND,
                              0644);
            if (fd_out < 0)
            {
                perror(redir->filename);
                return (-1);
            }
            dup2(fd_out, STDOUT_FILENO);
            close(fd_out);
        }
        else if (redir->type == HEREDOC) // "<<"
        {
			// create a pipe to store heredoc content
			int heredoc_pipe[2];
			if (pipe(heredoc_pipe) < 0)
			{
				perror("pipe");
				return (-1);
			}
			char *limiter = redir->filename;
			char *line = NULL;
			while (1)
			{
				ft_putstr_fd("> ", 1);
				line = get_next_line(0);
				if (!line)
					break;
				size_t len = ft_strlen(line);
				if (len > 0 && line[len - 1] == '\n')
					line[len -1] = '\0';

				if (ft_strcmp(line, limiter) == 0)
				{
					free(line);
					break;
				}
				write(heredoc_pipe[1], line, ft_strlen(line));
				write(heredoc_pipe[1], "\n", 1);
				free(line);
			}
			close(heredoc_pipe[1]); 
			dup2(heredoc_pipe[0], STDIN_FILENO);
			close(heredoc_pipe[0]);
        }
        redir = redir->next;
    }
    return (0);
}


char *find_executable(char **argv)
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
	while (*tmp)
	{
		free(*tmp);
		tmp++;
	}
	free(split_path);
	return executable_path;
}

void execute_builtin(t_op *cmd)
{
	char **argv = cmd->str;
	if (!argv || !argv[0])
        return;
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
	else if (strcmp(argv[0], "export") == 0)
		handle_export(argv);
}

void execute_simple_command(t_op *cmd) {
	extern char **environ;
    if (!cmd || !cmd->str || !cmd->str[0])
	{
		return;
	}
        // Save original FDs
	int saved_stdin = dup(STDIN_FILENO);
	int saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin < 0 || saved_stdout < 0) {
		perror("dup");
		return;
        }

        if (apply_redirections(cmd) < 0)
		{
			dup2(saved_stdin, STDIN_FILENO);
			dup2(saved_stdout, STDOUT_FILENO);
			close(saved_stdin);
			close(saved_stdout);
        	return;
		}
		if (is_builtin(cmd))
			execute_builtin(cmd);
		else
		{
			pid_t pid = fork();
			if (pid < 0)
				perror("fork");
			else if (pid == 0)
			{
				char *exec_path = find_executable(cmd->str);
				if (!exec_path)
				{
					fprintf(stderr, "%s: command not found\n", cmd->str[0]);
					_exit(127);
				}
				execve(exec_path, cmd->str, environ); // or use your env array
				perror("execve");
				_exit(1);
			}
			else //parent
			{
				// wait
				int status;
				waitpid(pid, &status, 0);
				// Optional: handle exit status if needed
			}
		}
		// Restore FDs
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
	}


void execute_pipeline(t_op *cmd)
{
	extern char **environ;
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
            if (apply_redirections(current) < 0)
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
                execute_builtin(current);
                _exit(0);
            }
            else
            {
				// external
				char *exec_path = find_executable(current->str);
				if (!exec_path)
				{
                    fprintf(stderr, "%s: command not found\n", current->str[0]);
                    _exit(127);
                }
                execve(exec_path, current->str, environ);
                perror("execve");
                _exit(1);
            }
        }
        else // Parent
        {
            pids[i++] = pid; // Store child PID

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
    for (int j = 0; j < pipeline_length; j++)
        waitpid(pids[j], NULL, 0);

    free(pids);
}