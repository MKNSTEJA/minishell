/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 22:48:58 by ykhattab          #+#    #+#             */
/*   Updated: 2025/02/27 23:45:46 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	count_heredocs(t_op *cmd)
{
	int		count;
	t_redir	*redir;

	count = 0;
	while (cmd)
	{
		redir = cmd->redirections;
		while (redir)
		{
			if (redir->type == HEREDOC)
				count++;
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (count);
}

void	assign_heredoc_indices(t_heredoc_state *state, t_op *cmd)
{
	int		cmd_idx;
	int		heredoc_idx;
	t_redir	*redir;

	cmd_idx = 0;
	heredoc_idx = 0;
	while (cmd)
	{
		redir = cmd->redirections;
		while (redir)
		{
			if (redir->type == HEREDOC)
				state->cmd_indices[heredoc_idx++] = cmd_idx;
			redir = redir->next;
		}
		cmd = cmd->next;
		cmd_idx++;
	}
}

void	handle_heredoc_pipe(t_heredoc_state *state, int heredoc_index,
		t_redir *redir, t_data *data)
{
	if (pipe(state->heredoc_pipes + heredoc_index * 2) < 0)
	{
		perror("pipe");
		free(state->cmd_indices);
		free(state->heredoc_pipes);
		exit(1);
	}
	process_heredoc_input(state->heredoc_pipes[heredoc_index * 2 + 1], redir,
		data);
	close(state->heredoc_pipes[heredoc_index * 2 + 1]);
}

void	cleanup_heredoc_state(t_heredoc_state *state)
{
	int	i;

	i = 0;
	if (state->heredoc_pipes)
	{
		while (i < state->heredoc_count * 2)
		{
			close(state->heredoc_pipes[i]);
			i++;
		}
		free(state->heredoc_pipes);
		free(state->cmd_indices);
	}
}
