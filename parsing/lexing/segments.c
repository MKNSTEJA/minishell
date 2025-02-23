/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segments.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 06:38:32 by mknsteja          #+#    #+#             */
/*   Updated: 2025/02/23 19:12:50 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void		process_input_segments(char *input_string, t_split **input);
t_segment	*new_segment(const char *text, t_quote_state state);
void		add_segment(t_segment **head, t_segment *new_seg);
void		parse_segments(t_split **input, t_segment **current_segments,
				t_parts *parts);
void		append_char_to_segment(t_parts **parts,
				t_segment **current_segments, int *i);

/**
 * process_input_segments
 * Takes an input string and a pointer to the token list.
 * It initializes a parts structure and starts parsing the string into segments.
 */
void	process_input_segments(char *input_string, t_split **input)
{
	t_segment	*current_segments;
	t_parts		parts;

	parts.string = input_string;
	parts.i = 0;
	parts.current_segment = NULL;
	parts.quote = QUOTE_NONE;
	parts.token = WORD;
	parts.token_quoted = 0;
	current_segments = NULL;
	parse_segments(input, &current_segments, &parts);
}

/**
 * new_segment
 * Allocates and returns a new segment node with a duplicate of the input text.
 * It also sets the given quote state.
 */
t_segment	*new_segment(const char *text, t_quote_state state)
{
	t_segment	*seg;

	seg = malloc(sizeof(t_segment));
	if (!seg)
	{
		perror("malloc");
		exit(1);
	}
	seg->text = ft_strdup(text);
	seg->quote = state;
	seg->next = NULL;
	return (seg);
}

/**
 * parse_segments
 * Walks through the input string character by character,
 * and creates or appends segments based on the character encountered.
 */
void	parse_segments(t_split **input, t_segment **current_segments,
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
			append_char_to_segment(&parts, current_segments, &i);
	}
	if (*current_segments)
		append_list(input, *current_segments, parts->token,
			&parts->token_quoted);
}

/**
 * add_segment
 * Adds a new segment node at the end of the segment list.
 */
void	add_segment(t_segment **head, t_segment *new_seg)
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

/**
 * append_char_to_segment
 * Appends a single character from the input's string to the current segment.
 * If there's no current segment, it creates one.
 */
void	append_char_to_segment(t_parts **parts, t_segment **current_segments,
		int *i)
{
	char	temp_char[2];
	char	*updated_text;

	temp_char[0] = (*parts)->string[(*i)];
	temp_char[1] = '\0';
	updated_text = NULL;
	if (!((*parts)->current_segment))
	{
		(*parts)->current_segment = new_segment("", QUOTE_NONE);
		add_segment(current_segments, (*parts)->current_segment);
	}
	updated_text = ft_strjoin(((*parts)->current_segment)->text, temp_char);
	if ((*parts)->current_segment->text)
		free((*parts)->current_segment->text);
	// free((*parts)->current_segment->text);
	((*parts)->current_segment)->text = updated_text;
	(*i)++;
}
