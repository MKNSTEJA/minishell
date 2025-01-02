

#include "../include/minishell.h"

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

void execute_simple_command(command_t *cmd) {
    if (!cmd || !cmd->args || !cmd->args[0])
        return;

    if (cmd->type == CMD_BUILTIN && is_builtin(cmd->args[0])) {
        // Save original FDs
        int saved_stdin = dup(STDIN_FILENO);
        int saved_stdout = dup(STDOUT_FILENO);
        if (saved_stdin < 0 || saved_stdout < 0) {
            perror("dup");
            return;
        }

        // Handle redirections
        redirection_t *redir = cmd->redirections;
        while (redir) {
            if (redir->type == REDIRECT_IN) {
                int fd_in = open(redir->filename, O_RDONLY);
                if (fd_in < 0) {
                    perror(redir->filename);
                    // Restore FDs and return
                    dup2(saved_stdin, STDIN_FILENO);
                    dup2(saved_stdout, STDOUT_FILENO);
                    close(saved_stdin);
                    close(saved_stdout);
                    return;
                }
                dup2(fd_in, STDIN_FILENO);
                close(fd_in);
            }
            else if (redir->type == REDIRECT_OUT || redir->type == APPEND) {
                int flags = O_WRONLY | O_CREAT;
                if (redir->type == REDIRECT_OUT)
                    flags |= O_TRUNC;
                else if (redir->type == APPEND)
                    flags |= O_APPEND;
                
                int fd_out = open(redir->filename, flags, 0644);
                if (fd_out < 0) {
                    perror(redir->filename);
                    // Restore FDs and return
                    dup2(saved_stdin, STDIN_FILENO);
                    dup2(saved_stdout, STDOUT_FILENO);
                    close(saved_stdin);
                    close(saved_stdout);
                    return;
                }
                dup2(fd_out, STDOUT_FILENO);
                close(fd_out);
            }
            // Handle other redirection types (e.g., HEREDOC) here
            redir = redir->next;
        }

        // Execute the built-in command
        execute_builtin(cmd);

        // Restore original FDs
        dup2(saved_stdin, STDIN_FILENO);
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdin);
        close(saved_stdout);
    }
    else {
        // Handle external commands, possibly with forking and execution
        execute_external_command(cmd);
    }
}

void execute_pipeline(command_t *cmd)
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
    command_t *current = cmd;

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
            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            if (current->next)
            {
                close(pipe_fds[0]);
                dup2(pipe_fds[1], STDOUT_FILENO);
                close(pipe_fds[1]);
            }
            if (is_builtin(current->argv))
            {
                execute_builtin(current->argv);
                _exit(0);
            }
            else
            {
                execvp(current->argv[0], current->argv);
                perror("execvp");
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