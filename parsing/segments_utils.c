/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segments_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmummadi <kmummadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 14:51:49 by kmummadi          #+#    #+#             */
/*   Updated: 2025/02/10 15:02:05 by kmummadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
