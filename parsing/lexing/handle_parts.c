/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_parts.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:52:55 by mknsteja          #+#    #+#             */
/*   Updated: 2025/02/12 21:09:03 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_redirections(t_split **input, t_parts **parts, int *i)
{
	if ((*parts)->string[*i] == '<' && (*parts)->string[*i + 1] == '<')
	{
		append_list(input, create_segment("<<", QUOTE_NONE), HEREDOC, &((*parts)->token_quoted));
		(*i) += 2;
	}
	else if ((*parts)->string[*i] == '<')
	{
		append_list(input, create_segment("<", QUOTE_NONE), IN, &((*parts)->token_quoted));
		(*i) += 1;
	}
	else if ((*parts)->string[*i] == '>' && (*parts)->string[*i + 1] == '>')
	{
		append_list(input, create_segment(">>", QUOTE_NONE), APPEND, &((*parts)->token_quoted));
		(*i) += 2;
	}
	else if ((*parts)->string[*i] == '>')
	{
		append_list(input, create_segment(">", QUOTE_NONE), OUT, &((*parts)->token_quoted));
		(*i) += 1;
	}
}

void	handle_space(t_split **input, t_segment **current_segments,
		t_parts **parts, int *i)
{
	if (*current_segments)
	{
		append_list(input, *current_segments, (*parts)->token, &((*parts)->token_quoted));
		*current_segments = NULL;
		(*parts)->token = WORD;
	}
	(*i)++;
	(*parts)->current_segment = NULL;
}

void	handle_quotes(t_segment **current_segments, t_parts **parts, int *i)
{
	t_quote_state	new_state;

	new_state = QUOTE_NONE;
	if ((*parts)->string[*i] == '"')
		new_state = DQ;
	else if ((*parts)->string[*i] == '\'')
		new_state = SQ;
	(*parts)->token_quoted = 1;
	if ((*parts)->quote == QUOTE_NONE)
	{
		(*parts)->quote = new_state;
		(*parts)->current_segment = create_segment("", (*parts)->quote);
		append_segment(current_segments, (*parts)->current_segment);
	}
	else if ((*parts)->quote == new_state)
	{
		(*parts)->quote = QUOTE_NONE;
		if (is_segment_empty((*parts)->current_segment))
			remove_last_segment(current_segments);
		(*parts)->current_segment = NULL;
	}
	else
		return (final_assign(parts, current_segments, i));
	(*i)++;
}

void	handle_others(t_split **input, t_segment **current_segments,
		t_parts **parts, int *i)
{
	if (*current_segments)
	{
		append_list(input, *current_segments, (*parts)->token, &((*parts)->token_quoted));
		*current_segments = NULL;
		(*parts)->token = WORD;
	}
	if ((*parts)->string[*i] == '|')
	{
		append_list(input, create_segment("|", QUOTE_NONE), PIPES, &((*parts)->token_quoted));
		(*i)++;
	}
	else
		handle_redirections(input, parts, i);
}

void	handle_dollar(t_segment **current_segments, t_parts **parts, int *i)
{
	(*parts)->quote = DQ;
	(*parts)->current_segment = create_segment("$\"", (*parts)->quote);
	(*i) += 2;
	while (((*parts)->string[*i] && (*parts)->string[*i] != '"'))
	{
		final_assign(parts, current_segments, i);
	}
	if ((*parts)->string[*i] == '"')
		final_assign(parts, current_segments, i);
	append_segment(current_segments, (*parts)->current_segment);
}
