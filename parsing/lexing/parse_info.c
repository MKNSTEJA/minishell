/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_info.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 13:11:31 by mknsteja          #+#    #+#             */
/*   Updated: 2025/02/14 02:15:42 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	assemble_token_string(t_split **input);
size_t	calculate_segments_length(t_segment *seg);

t_split	*split_inputs(char *string)
{
	t_split	*input;

	input = NULL;
	process_input_segments(string, &input);
	assemble_token_string(&input);
	return (input);
}

/**
 * @brief Assembles the full input strings from individual segments.
 *
 * This function iterates over the linked list of t_split structures, calculates
 * the total length of their segment texts, allocates memory accordingly, and
 * concatenates the segment texts into a single string.
 *
 * @param input A pointer to the head of the linked list containing the input segments.
 */
void	assemble_token_string(t_split **input)
{
	size_t		total_length;
	t_split		*ptr;
	t_segment	*seg;

	if (!input || !*input)
		return ;
	total_length = 0;
	ptr = *input;
	seg = ptr->segments;
	while (ptr)
	{
		seg = ptr->segments;
		total_length = calculate_segments_length(seg);
		ptr->str = malloc(total_length + 1);
		if (!ptr->str)
			exit(1);
		ptr->str[0] = '\0';
		while (seg)
		{
			ft_strlcat(ptr->str, seg->text, total_length + 1);
			seg = seg->next;
		}
		ptr = ptr->next;
	}
}

/**
 * @brief Calculates the total length of all segment texts in a linked list.
 *
 * This function traverses the linked list of segments and sums up the lengths
 * of their text strings.
 *
 * @param segment The head of the linked list of segments.
 * @return The total length of all segment texts.
 */
size_t	calculate_segments_length(t_segment *segment)
{
	t_segment	*seg;
	size_t		total_length;

	seg = segment;
	if (!seg)
		return (0);
	total_length = 0;
	while (seg)
	{
		total_length += ft_strlen(seg->text);
		seg = seg->next;
	}
	return (total_length);
}

void append_list(t_split **head, t_segment *segments, t_type type, int *token_quoted)
{
	t_split	*new_node;
	t_split	*current;
	
	new_node = malloc(sizeof(t_split));
	current = *head;
	if (!new_node)
	{
		perror("malloc");
		exit(1);
	}
	new_node->str = NULL;
	new_node->segments = segments;
	new_node->type = type;
	new_node->prev = NULL;
	new_node->next = NULL;
	new_node->token_has_quotes = *token_quoted;
	*token_quoted = 0;
	if (!*head)
		*head = new_node;
	else
	{
		while (current->next)
			current = current->next;
		current->next = new_node;
		new_node->prev = current;
	}
}
