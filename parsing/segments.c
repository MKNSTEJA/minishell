/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segments.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmummadi <kmummadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 06:38:32 by mknsteja          #+#    #+#             */
/*   Updated: 2025/01/31 20:12:46 by kmummadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void		assign_segments(t_split **input, t_segment **current_segments,
				t_parts *parts);
t_segment	*create_segment(const char *text, t_quote_state state);
void		append_segment(t_segment **head, t_segment *new_seg);
void		final_assign(t_parts **parts, t_segment **current_segments, int *i);

void	handle_segments(char *string, t_split **input)
{
	t_segment	*current_segments;
	t_parts		parts;

	parts.string = string;
	parts.i = 0;
	parts.current_segment = NULL;
	parts.quote = QUOTE_NONE;
	parts.token = WORD;
	current_segments = NULL;
	assign_segments(input, &current_segments, &parts);
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
		t_parts *parts)
{
	int	i;

	i = 0;
	while (parts->string[i])
	{
		if (parts->quote == QUOTE_NONE && (parts->string[i] == 32
				|| parts->string[i] == 9))
			handle_space(input, current_segments, &parts, &i);
		else if (parts->string[i] == '"' || parts->string[i] == '\'')
			handle_quotes(current_segments, &parts, &i);
		else if (parts->quote == QUOTE_NONE && (parts->string[i] == '|'
				|| parts->string[i] == '<' || parts->string[i] == '>'))
			handle_others(input, current_segments, &parts, &i);
		else if (parts->quote == QUOTE_NONE && parts->string[i] == '$'
			&& parts->string[i + 1] == '"')
			handle_dollar(current_segments, &parts, &i);
		else
			final_assign(&parts, current_segments, &i);
	}
	if (*current_segments)
		append_list(input, *current_segments, parts->token);
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

void	final_assign(t_parts **parts, t_segment **current_segments, int *i)
{
	char	temp_char[2];
	char	*updated_text;

	temp_char[0] = (*parts)->string[(*i)];
	temp_char[1] = '\0';
	updated_text = NULL;
	if (!((*parts)->current_segment))
	{
		(*parts)->current_segment = create_segment("", QUOTE_NONE);
		append_segment(current_segments, (*parts)->current_segment);
	}
	updated_text = ft_strjoin(((*parts)->current_segment)->text, temp_char);
	free((*parts)->current_segment->text);
	((*parts)->current_segment)->text = updated_text;
	(*i)++;
}
