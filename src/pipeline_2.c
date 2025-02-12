/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 22:14:08 by ykhattab          #+#    #+#             */
/*   Updated: 2025/02/12 23:28:56 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	setup_child_io(t_op *current, int prev_fd, int pipe_fds[2])
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
}

void	wait_for_children(t_pipe_state *state)
{
	int	status;
	int	i;
	int	child_status;

	status = 0;
	i = 0;
	while (i < state->pipeline_length)
	{
		waitpid(state->pids[i], &child_status, 0);
		if (state->pids[i] == state->last_pid)
			status = child_status;
		i++;
	}
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
