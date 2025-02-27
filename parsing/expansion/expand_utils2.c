/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 17:16:18 by kmummadi          #+#    #+#             */
/*   Updated: 2025/02/27 22:23:56 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	initialise_exp(t_expand *exp, t_split **head)
{
	if (!head || !*head)
		return (-1);
	exp->split = *head;
	exp->seg = NULL;
	return (0);
}

void	initialise_inside_loop(t_expand *exp, t_segment *segments)
{
	exp->expanded_head = NULL;
	exp->expanded_tail = NULL;
	exp->seg = segments;
}

void	expand_segments(t_expand *exp, t_data *data)
{
	while (exp->seg)
	{
		loop_string(exp->seg->text, exp, data, exp->seg);
		exp->seg = exp->seg->next;
	}
	exp->expanded_str = convert_char_list_to_string(exp->expanded_head);
	free_char_list(exp->expanded_head);
}
