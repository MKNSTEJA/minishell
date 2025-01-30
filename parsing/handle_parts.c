/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_parts.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmummadi <kmummadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 00:52:55 by mknsteja          #+#    #+#             */
/*   Updated: 2025/01/30 21:07:45 by kmummadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void		handle_redirections(t_split **input, int *i, char *string);

t_segment	*handle_space(t_split **input, t_segment **current_segments,
		t_type *token, int *i)
{
	if (*current_segments)
	{
		append_list(input, *current_segments, *token);
		*current_segments = NULL;
		*token = WORD;
	}
	(*i)++;
	return (NULL);
}

t_segment	*handle_quotes(t_quote_state *quote, t_segment **current_segments,
		int *i, char *string, t_segment **current_segment)
{
	t_quote_state	new_state;

	new_state = QUOTE_NONE;
	// printf("Quote: %d, i = %d, string[i] = %c\n", *quote, *i, string[*i]);
	if (string[*i] == '"')
		new_state = QUOTE_DOUBLE;
	else if(string[*i] == '\'')
		new_state = QUOTE_SINGLE;
	if (*quote == QUOTE_NONE)
	{
		*quote = new_state;
		*current_segment = create_segment("", *quote);
		append_segment(current_segments, *current_segment);
	}
	else if (*quote == new_state)
	{
		*quote = QUOTE_NONE;
		*current_segment = NULL;
	}
	else
		return (final_assign(string, i, current_segment, current_segments));
	return ((*i)++, *current_segment);
}

void	handle_others(t_split **input, t_segment **current_segments,
		t_type *token, int *i, char *string)
{
	if (*current_segments)
	{
		append_list(input, *current_segments, *token);
		*current_segments = NULL;
		*token = WORD;
	}
	if (string[*i] == '|')
	{
		append_list(input, create_segment("|", QUOTE_NONE), PIPES);
		(*i)++;
	}
	else
		handle_redirections(input, i, string);
}

void	handle_redirections(t_split **input, int *i, char *string)
{
	if (string[*i] == '<' && string[*i + 1] == '<')
	{
		append_list(input, create_segment("<<", QUOTE_NONE), HEREDOC);
		(*i) += 2;
	}
	else if (string[*i] == '<')
	{
		append_list(input, create_segment("<", QUOTE_NONE), IN);
		(*i) += 1;
	}
	else if (string[*i] == '>' && string[*i + 1] == '>')
	{
		append_list(input, create_segment(">>", QUOTE_NONE), APPEND);
		(*i) += 2;
	}
	else if (string[*i] == '>')
	{
		append_list(input, create_segment(">", QUOTE_NONE), OUT);
		(*i) += 1;
	}
}

t_segment	*handle_dollar(t_segment **current_segments,
		t_quote_state *quote_state, int *i)
{
	t_segment	*current_segment;

	*quote_state = QUOTE_DOUBLE;
	current_segment = create_segment("$\"", *quote_state);
	append_segment(current_segments, current_segment);
	(*i) += 2;
	return (current_segment);
}
