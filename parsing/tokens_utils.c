/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 16:09:04 by kmummadi          #+#    #+#             */
/*   Updated: 2025/03/01 23:44:31 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_split	*create_new_token(char *str, t_type type)
{
	t_split	*new_node;

	new_node = malloc(sizeof(t_split));
	if (!new_node)
	{
		perror("malloc");
		exit(1);
	}
	new_node->str = ft_strdup(str);
	new_node->type = type;
	new_node->prev = NULL;
	new_node->next = NULL;
	return (new_node);
}

int	is_token_unquoted(t_split *token)
{
	t_segment	*seg;

	seg = token->segments;
	while (seg)
	{
		if (seg->quote != QUOTE_NONE)
			return (0);
		seg = seg->next;
	}
	return (1);
}

t_split	*remove_token(t_split **head, t_split *token)
{
	t_split	*prev;
	t_split	*next;

	prev = token->prev;
	next = token->next;
	if (prev)
		prev->next = next;
	if (next)
		next->prev = prev;
	if (token == *head)
		*head = next;
	if (token->segments)
		free_segments(&token->segments);
	free(token->str);
	free(token);
	return (next);
}
