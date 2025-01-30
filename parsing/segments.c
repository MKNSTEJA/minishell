/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segments.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmummadi <kmummadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 06:38:32 by mknsteja          #+#    #+#             */
/*   Updated: 2025/01/30 21:07:07 by kmummadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void		assign_segments(t_split **input, t_segment **current_segments,
				char *string, t_quote_state quote);
t_segment	*create_segment(const char *text, t_quote_state state);
void		append_segment(t_segment **head, t_segment *new_seg);
t_segment		*final_assign(char *string, int *i, t_segment **current_segment,
				t_segment **current_segments);

void	handle_segments(char *string, t_split **input)
{
	t_segment		*current_segments;
	t_quote_state	quote;

	quote = QUOTE_NONE;
	current_segments = NULL;
	assign_segments(input, &current_segments, string, quote);
}

t_segment	*create_segment(const char *text, t_quote_state state)
{
	t_segment	*seg;

	seg = malloc(sizeof(t_segment));
	if (!seg)
	{
		perror("malloc");
		exit(1);
	}
	seg->text = ft_strdup(text);
	seg->quote_state = state;
	seg->next = NULL;
	return (seg);
}

void	assign_segments(t_split **input, t_segment **current_segments,
		char *string, t_quote_state quote)
{
	int			i;
	t_type		token;
	t_segment	*current_segment;

	i = 0;
	token = WORD;
	current_segment = NULL;
	while (string[i])
	{
		if (quote == QUOTE_NONE && (string[i] == 32 || string[i] == 9))
			current_segment = handle_space(input, current_segments, &token, &i);
		else if (string[i] == '"' || string[i] == '\'')
			handle_quotes(&quote, current_segments, &i, string, &current_segment);
		else if (quote == QUOTE_NONE && (string[i] == '|' || string[i] == '<'
				|| string[i] == '>'))
			handle_others(input, current_segments, &token, &i, string);
		else if (quote == QUOTE_NONE && string[i] == '$'
			&& string[i + 1] == '"')
			current_segment = handle_dollar(current_segments, &quote, &i);
		else
			final_assign(string, &i, &current_segment, current_segments);
		// printf("Current segment: %s | Quote State: %d\n", current_segment->text, current_segment->quote_state);
	}
	if (*current_segments)
		append_list(input, *current_segments, token);
}

void	append_segment(t_segment **head, t_segment *new_seg)
{
	t_segment	*current;

	if (!*head)
	{
		*head = new_seg;
	}
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new_seg;
	}
}

t_segment	*final_assign(char *string, int *i, t_segment **current_segment,
		t_segment **current_segments)
{
	char	temp_char[2];
	char	*updated_text;

	temp_char[0] = string[*i];
	temp_char[1] = '\0';
	updated_text = NULL;
	if (!(*current_segment))
	{
		*current_segment = create_segment("", QUOTE_NONE);
		append_segment(current_segments, *current_segment);
	}
	updated_text = ft_strjoin((*current_segment)->text, temp_char);
	free((*current_segment)->text);
	(*current_segment)->text = updated_text;
	(*i)++;
	return(*current_segment);
}
