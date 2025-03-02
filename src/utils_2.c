/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 22:57:56 by ykhattab          #+#    #+#             */
/*   Updated: 2025/03/02 01:12:29 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	expand_escape_helper(const char *str, char *result, int *j)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1] != '\0')
		{
			i++;
			if (str[i] == 'n')
				result[(*j)++] = '\n';
			else if (str[i] == 't')
				result[(*j)++] = '\t';
			else
			{
				result[(*j)++] = '\\';
				result[(*j)++] = str[i];
			}
		}
		else
		{
			result[(*j)++] = str[i];
		}
		i++;
	}
}

char	*expand_escape(const char *str)
{
	char	*result;
	int		j;

	j = 0;
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	expand_escape_helper(str, result, &j);
	result[j] = '\0';
	return (result);
}

void	init_child_context(t_child_context *ctx, t_pipe_state *state,
		t_heredoc_state *heredoc_state, int pipe_fds[2])
{
	ctx->prev_fd = state->prev_fd;
	ctx->pipe_fds[0] = pipe_fds[0];
	ctx->pipe_fds[1] = pipe_fds[1];
	ctx->heredoc_state = heredoc_state;
	ctx->cmd_index = state->index;
}
