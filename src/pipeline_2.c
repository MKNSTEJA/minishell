/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmummadi <kmummadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 22:14:08 by ykhattab          #+#    #+#             */
/*   Updated: 2025/02/16 17:49:41 by kmummadi         ###   ########.fr       */
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

void	wait_for_children(t_pipe_state *state, t_data *data)
{
	int	status;
	int	i;
	int	child_status;
	int	sig;

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
		data->last_exit = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT)
			fprintf(stderr, "Quit: %d\n", sig);
		data->last_exit = 128 + sig;
	}
	else
		data->last_exit = 1;
}
