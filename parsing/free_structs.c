/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmummadi <kmummadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 16:22:03 by kmummadi          #+#    #+#             */
/*   Updated: 2025/02/05 18:06:20 by kmummadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_data(t_data *data)
{
	int	i;

	if (data->env)
	{
		i = 0;
		while (data->env[i])
		{
			free(data->env[i]);
			i++;
		}
		free(data->env);
	}
}

void	free_split(t_split *list)
{
	t_split	*current;
	t_split	*next_node;

	if (!list)
		return ;
	current = list;
	while (current)
	{
		next_node = current->next;
		free_segment(current);
		if (current->str)
		{
			free(current->str);
			current->str = NULL;
		}
		if (current)
			free(current);
		current = next_node;
	}
}

void	free_segment(t_split *list)
{
	t_segment	*current_seg;
	t_segment	*next_seg;

	current_seg = list->segments;
	next_seg = NULL;
	while (current_seg)
	{
		next_seg = current_seg->next;
		if (current_seg->text)
		{
			free(current_seg->text);
			current_seg->text = NULL;
		}
		if (current_seg)
			free(current_seg);
		current_seg = next_seg;
	}
}

void	free_op(t_op *cmd)
{
	t_op	*ptr;
	t_op	*next_ptr;
	int		i;
	t_redir	*redir;
	t_redir	*temp;

	ptr = cmd;
	i = 0;
	while (ptr)
	{
		next_ptr = ptr->next;
		if (ptr->str)
		{
			i = 0;
			while (ptr->str[i])
			{
				free(ptr->str[i]);
				ptr->str[i] = NULL;
				i++;
			}
			free(ptr->str);
			ptr->str = NULL;
		}
		redir = ptr->redirections;
		while (redir)
		{
			temp = redir->next;
			free(redir->filename);
			redir->filename = NULL;
			free(redir);
			redir = temp;
		}
		free(ptr);
		ptr = NULL;
		ptr = next_ptr;
	}
}
