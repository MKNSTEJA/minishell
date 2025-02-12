/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 22:20:07 by ykhattab          #+#    #+#             */
/*   Updated: 2025/02/12 23:28:38 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <errno.h>

static void	execute_in_child(t_op *cmd, t_data *data)
{
	char	*exec_path;

	signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
	exec_path = find_executable(cmd->str, data->env);
	if (!exec_path)
	{
		fprintf(stderr, "%s: command not found\n", cmd->str[0]);
		_exit(127);
	}
	execve(exec_path, cmd->str, data->env);
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
		else if (WIFSIGNALED(status))
		{
			int sig = WTERMSIG(status);
			if (sig == SIGQUIT)
				fprintf(stderr, "Quit: %d\n", sig);
			g_exit_code = 128 + sig;
		}
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
