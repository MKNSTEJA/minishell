/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknsteja <mknsteja@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 22:20:07 by ykhattab          #+#    #+#             */
/*   Updated: 2025/02/09 10:46:30 by mknsteja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <errno.h>

// static void	execute_in_child(t_op *cmd, t_data *data)
// {
// 	char	*exec_path;

// 	exec_path = find_executable(cmd->str, data->env);
// 	if (!exec_path)
// 	{
// 		fprintf(stderr, "%s: command not found\n", cmd->str[0]);
// 		_exit(127);
// 	}
// 	execve(exec_path, cmd->str, data->env);
// 	perror("execve");
// 	_exit(1);
// }

static void	execute_in_child(t_op *cmd, t_data *data)
{
	char	*exec_path;
	char	**new_argv;

	exec_path = find_executable(cmd->str, data->env);
	if (!exec_path)
	{
		print_error_msg(cmd->str[0], NULL, "command not found");
		_exit(127);
	}
	execve(exec_path, cmd->str, data->env);
	if (errno == ENOEXEC) // ENOEXEC means "Exec format error"
	{
		// Fallback: run the file with /bin/sh.
		// Build a new argv array:
		// new_argv[0] = "sh" (or the name you want)
		// new_argv[1] = exec_path (the script filename)
		// new_argv[2...] = the rest of cmd->str (if any)
		int i, argc = 0;
		while (cmd->str[argc])
			argc++;
		new_argv = malloc(sizeof(char *) * (argc + 2));
		if (!new_argv)
		{
			perror("malloc");
			_exit(1);
		}
		new_argv[0] = "sh"; // or "bash" if you prefer
		new_argv[1] = exec_path;
		for (i = 1; i < argc; i++)
			new_argv[i + 1] = cmd->str[i];
		new_argv[argc + 1] = NULL;
		execve("/bin/sh", new_argv, data->env);
		free(new_argv); // In case execve fails again.
	}
	perror("execve");
	_exit(1);
}

static void	handle_fork_and_wait(t_op *cmd, t_data *data)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		perror("fork");
	else if (pid == 0)
		execute_in_child(cmd, data);
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_exit_code = WEXITSTATUS(status);
		else
			g_exit_code = 1;
	}
}

static void	cleanup(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

void	execute_simple_command(t_op *cmd, t_data *data)
{
	int	saved_stdin;
	int	saved_stdout;

	if (!cmd)
		return ;
	if ((!cmd->str || !cmd->str[0]) && !cmd->redirections)
		return ;
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin < 0 || saved_stdout < 0)
	{
		perror("dup");
		return ;
	}
	if (apply_redirections(cmd, data->env) < 0)
	{
		cleanup(saved_stdin, saved_stdout);
		return ;
	}
	if (cmd->str && cmd->str[0])
	{
		if (is_builtin(cmd))
			execute_builtin(cmd, data);
		else
			handle_fork_and_wait(cmd, data);
	}
	cleanup(saved_stdin, saved_stdout);
}
