/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segments_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 14:51:49 by kmummadi          #+#    #+#             */
/*   Updated: 2025/02/27 22:34:01 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_segment_empty(t_segment *seg)
{
	return (seg && seg->text && seg->text[0] == '\0');
}

void	remove_last_segment(t_segment **head)
{
	t_segment	*current;
	t_segment	*prev;

	if (!head || !*head)
		return ;
	current = *head;
	prev = NULL;
	while (current->next)
	{
		prev = current;
		current = current->next;
	}
	if (prev)
		prev->next = NULL;
	else
		*head = NULL;
	if (current->text)
		free(current->text);
	free(current);
}

void	free_segments(t_segment **current_segments)
{
	t_segment	*seg;
	t_segment	*tmp;

	seg = *current_segments;
	while (seg)
	{
		tmp = seg;
		seg = seg->next;
		free(tmp->text);
		free(tmp);
	}
	*current_segments = NULL;
}

void	clear_empty_segments(t_split *temp)
{
	while (temp)
	{
		if (!temp->segments)
		{
			temp->segments = NULL;
			temp->token_has_quotes = 0;
		}
		temp = temp->next;
	}
}
