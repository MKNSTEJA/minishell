/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_expand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmummadi <kmummadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 17:58:57 by kmummadi          #+#    #+#             */
/*   Updated: 2025/02/05 21:16:17 by kmummadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	free_expanded_str(t_expand *exp, t_split **head)
{
	if (!exp->expanded_str || exp->expanded_str[0] == '\0')
	{
		free(exp->split->str);
		exp->split->str = NULL;
		exp->to_remove = exp->split;
		exp->split = exp->split->next;
		remove_token(head, exp->to_remove);
		free(exp->expanded_str);
		return (1);
	}
	return (0);
}

void	free_char_list(t_char_node *head)
{
	t_char_node	*current;
	t_char_node	*next;

	current = head;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
}
