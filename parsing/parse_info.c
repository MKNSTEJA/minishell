/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_info.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yousef <yousef@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 13:11:31 by mknsteja          #+#    #+#             */
/*   Updated: 2025/02/02 15:45:57 by yousef           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*handle_delimiter(char *string, char c, int *i);
void	fill_inputs(t_split **input);
size_t	seg_len(t_segment *seg);

t_split	*split_inputs(char *string)
{
	t_split	*input;

	input = NULL;
	handle_segments(string, &input);
	fill_inputs(&input);
	return (input);
}

char	*handle_delimiter(char *string, char c, int *i)
{
	char			*result;
	unsigned int	start;

	start = (unsigned int)(*i);
	if (c == '\"' || c == '\'')
	{
		(*i)++;
		while (string[*i] && string[*i] != c)
			(*i)++;
		result = ft_substr(string, start + 1, (*i) - (start + 1));
		if (string[*i])
			(*i)++;
	}
	else
	{
		while (string[*i] && string[*i] != ' ' && string[*i] != '\t'
			&& string[*i] != '\"' && string[*i] != '\'' && string[*i] != '|'
			&& string[*i] != '>' && string[*i] != '<')
		{
			(*i)++;
		}
		result = ft_substr(string, start, (*i) - start);
	}
	return (result);
}

void	fill_inputs(t_split **input)
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
		total_length = seg_len(seg);
		ptr->str = malloc(total_length + 1);
		if (!ptr->str)
			exit(1);
		ptr->str[0] = '\0';
		seg = ptr->segments;
		while (seg)
		{
			ft_strlcat(ptr->str, seg->text, total_length + 1);
			seg = seg->next;
		}
		ptr = ptr->next;
	}
}

size_t	seg_len(t_segment *seg)
{
	size_t	total_length;

	total_length = 0;
	while (seg)
	{
		total_length += ft_strlen(seg->text);
		seg = seg->next;
	}
	return (total_length);
}

void	append_list(t_split **head, t_segment *segments, t_type type)
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