/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_3_heredocs.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 23:27:47 by ykhattab          #+#    #+#             */
/*   Updated: 2025/02/27 23:38:53 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	process_pipeline_heredocs(t_op *cmd, t_data *data,
		t_heredoc_state *state)
{
	t_op	*current;
	int		heredoc_index;
	int		cmd_index;
	t_redir	*redir;

	current = cmd;
	heredoc_index = 0;
	cmd_index = 0;
	while (current)
	{
		redir = current->redirections;
		while (redir)
		{
			if (redir->type == HEREDOC)
				handle_heredoc_pipe(state, heredoc_index++, redir, data);
			redir = redir->next;
		}
		current = current->next;
		cmd_index++;
	}
}

void	init_heredoc_state(t_heredoc_state *state, t_op *cmd, int len)
{
	len = count_commands(cmd);
	state->heredoc_count = count_heredocs(cmd);
	state->has_heredocs = (state->heredoc_count > 0);
	state->heredoc_pipes = malloc(sizeof(int) * 2 * state->heredoc_count);
	state->cmd_indices = malloc(sizeof(int) * state->heredoc_count);
	if (!state->heredoc_pipes || !state->cmd_indices)
	{
		perror("malloc");
		exit(1);
	}
	assign_heredoc_indices(state, cmd);
}

void	apply_heredoc_pipes(t_op *cmd, t_heredoc_state *state,
		int cmd_index)
{
	t_redir	*redir;
	int		i;

	i = 0;
	while (i < state->heredoc_count)
	{
		if (state->cmd_indices[i] == cmd_index)
		{
			redir = cmd->redirections;
			while (redir)
			{
				if (redir->type == HEREDOC)
				{
					dup2(state->heredoc_pipes[i * 2], STDIN_FILENO);
					close(state->heredoc_pipes[i * 2]);
					break ;
				}
				redir = redir->next;
			}
		}
		i++;
	}
}
