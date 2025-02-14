/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_expand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 17:58:57 by kmummadi          #+#    #+#             */
/*   Updated: 2025/02/14 03:12:49 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Checks and handles an empty expanded string for a token.
 *
 * If the expanded string is NULL or empty, this function handles the token based on
 * whether it originally contained quotes. If the token had quotes, its string is set
 * to an empty string. Otherwise, the token is removed from the list.
 *
 * @param exp Pointer to the expansion state structure.
 * @param head Pointer to the head of the token linked list.
 * @return 0 if the token is kept (with an empty string), 1 if the token was removed.
 */
int	handle_empty_expanded_string(t_expand *exp, t_split **head)
{
	if (!exp->expanded_str || exp->expanded_str[0] == '\0')
	{
		if (exp->split->token_has_quotes) //New block here
		{
			free(exp->split->str);
			exp->split->str = ft_strdup("");
			free(exp->expanded_str);
			return (0);
		}
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
