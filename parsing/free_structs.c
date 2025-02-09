/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknsteja <mknsteja@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 16:22:03 by kmummadi          #+#    #+#             */
/*   Updated: 2025/02/09 10:54:59 by mknsteja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_redirections(t_redir *redir, t_op *ptr);

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

	ptr = cmd;
	i = 0;
	next_ptr = NULL;
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
		free_redirections(ptr->redirections, ptr);
		ptr = next_ptr;
	}
}

void	free_redirections(t_redir *redir, t_op *ptr)
{
	t_redir	*temp;

	temp = NULL;
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
}
