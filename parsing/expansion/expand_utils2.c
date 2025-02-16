/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmummadi <kmummadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 17:16:18 by kmummadi          #+#    #+#             */
/*   Updated: 2025/02/16 17:27:08 by kmummadi         ###   ########.fr       */
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
