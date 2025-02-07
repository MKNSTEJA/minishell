/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 22:14:08 by ykhattab          #+#    #+#             */
/*   Updated: 2025/02/07 22:15:58 by ykhattab         ###   ########.fr       */
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
	int	status_last;
	int	i;
	int	child_status;

	status_last = 0;
	i = 0;
	while (i < state->pipeline_length)
	{
		waitpid(state->pids[i], &child_status, 0);
		if (state->pids[i] == state->last_pid)
			status_last = child_status;
		i++;
	}
	if (WIFEXITED(status_last))
		g_exit_code = WEXITSTATUS(status_last);
	else
		g_exit_code = 1;
}
