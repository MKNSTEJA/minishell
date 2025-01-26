/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segments.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknsteja <mknsteja@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 06:38:32 by mknsteja          #+#    #+#             */
/*   Updated: 2025/01/26 07:26:43 by mknsteja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void		handle_space(t_split *input, t_segment *current_segments,
				t_type token, int *i);
void		handle_quotes(t_quote_state quote_state,
				t_segment *current_segments, int *i, char *string);
void		handle_others(t_split *input, t_segment *current_segments,
				t_type *token, int *i, char *string);

t_segment	*handle_segments(char *string, t_split *input)
{
	t_segment		*current_segments;
	t_quote_state	quote;
	int				i;
	t_type			token;

	token = WORD;
	quote = QUOTE_NONE;
	current_segments = NULL;
	i = 0;
	while (string[i])
	{
		if (quote == QUOTE_NONE && (string[i] == 32 || string[i] == 9))
		{
			handle_space(input, current_segments, token, &i);
			continue ;
		}
		if (string[i] == '"' || string[i] == '\'')
		{
			handle_quotes(quote, current_segments, &i, string);
			continue ;
		}
	}
}

void	handle_space(t_split *input, t_segment *current_segments, t_type token,
		int *i)
{
	if (current_segments)
	{
		append_list(&input, current_segments, token);
		current_segments = NULL;
		token = WORD;
	}
	(*i)++;
}

void	handle_quotes(t_quote_state quote, t_segment *current_segments, int *i,
		char *string)
{
	t_quote_state	new_state;
	t_segment		*current_segment;

	new_state = QUOTE_NONE;
	current_segment = NULL;
	if (string[*i] == '"' || string[*i] == '\'')
	{
		if (string[*i] == '"')
			new_state = QUOTE_DOUBLE;
		else
			new_state = QUOTE_SINGLE;
		if (quote == QUOTE_NONE)
		{
			quote = new_state;
			current_segment = create_segment("", quote);
			append_segment(&current_segments, current_segment);
			(*i)++;
		}
		else if (quote == new_state)
		{
			quote = QUOTE_NONE;
			current_segment = NULL;
			(*i)++;
		}
	}
}

void	handle_others(t_split *input, t_segment *current_segments,
		t_type *token, int *i, char *string)
{
	if (current_segments)
	{
		append_list(&input, current_segments, token);
		current_segments = NULL;
		*token = WORD;
	}
	if (string[*i] == '|')
	{
		append_list(&input, create_segment("|", QUOTE_NONE), PIPES);
		(*i)++;
	}
	else if (string[*i] == '<')
	{
		if (string[*i + 1] == '<')
		{
			append_list(&input, create_segment("<<", QUOTE_NONE), HEREDOC);
			*i += 2;
		}
		else
		{
			append_list(&input, create_segment("<", QUOTE_NONE), IN);
			*i += 1;
		}
	}
	else if (string[*i] == '>')
	{
		if (string[*i + 1] == '>')
		{
			append_list(&input, create_segment(">>", QUOTE_NONE), APPEND);
			*i += 2;
		}
		else
		{
			append_list(&input, create_segment(">", QUOTE_NONE), OUT);
			*i += 1;
		}
	}
}
