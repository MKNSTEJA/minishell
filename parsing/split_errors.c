/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 16:52:14 by mknsteja          #+#    #+#             */
/*   Updated: 2025/02/27 22:33:42 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		check_pipe(t_split *input);
void	back_track(t_split *input);

int	split_errors(t_split *token_list, t_data *data)
{
	t_split	*ptr;

	back_track(token_list);
	ptr = token_list;
	while (ptr)
	{
		if (ptr->type == PIPES && check_pipe(ptr))
			return (1);
		else if ((ptr->type != PIPES && ptr->type != WORD)
			&& check_redirection(ptr, data))
		{
			return (1);
		}
		ptr = ptr->next;
	}
	return (0);
}

int	check_pipe(t_split *input)
{
	if (!input->next || !input->prev)
	{
		ft_putstr_fd("Error! Invalid input for pipe: `|'\n", 2);
		return (1);
	}
	return (0);
}

int	check_redirection(t_split *input, t_data *data)
{
	if (!input->next || input->next->type != WORD)
	{
		data->last_exit = 258;
		ft_putstr_fd("Error! Invalid input or token: ", 2);
		ft_putstr_fd(input->str, 2);
		ft_putstr_fd("'\n", 2);
		return (1);
	}
	return (0);
}

void	back_track(t_split *input)
{
	t_split	*back;
	t_split	*current;

	current = input;
	back = NULL;
	while (current)
	{
		current->prev = back;
		back = current;
		current = current->next;
	}
}
