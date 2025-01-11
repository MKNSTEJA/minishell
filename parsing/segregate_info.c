/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segregate_info.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknsteja <mknsteja@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 13:12:30 by mknsteja          #+#    #+#             */
/*   Updated: 2025/01/07 14:07:04 by mknsteja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	append_cmd(t_op *cmd, char *string);
void	split_cmds(t_split *input, t_op *cmd);

t_op	*initialise_cmd(t_split *input, char **envp)
{
	t_op	*cmd;

	cmd = malloc(sizeof(t_op));
	if (!cmd)
		exit(-1);
	cmd->str = NULL;
	cmd->next = NULL;
	cmd->append = 0;
	cmd->fd_in = STDIN_FILENO;
	cmd->fd_out = STDOUT_FILENO;
	cmd->prev = NULL;
	cmd->squote = 0;
	split_cmds(input, cmd);
	expand_dollar(cmd, envp);
	return (cmd);
}

void	append_str(t_op *cmd, char *string)
{
	int		i;
	char	**new;

	i = 0;
	while (cmd->str != NULL && cmd->str[i] != NULL)	
		i++;
	new = malloc(sizeof(char *) * (i + 2));
	if (!new)
		exit(-1);
	i = 0;
	while (cmd->str && cmd->str[i])
	{
		new[i] = ft_strdup(cmd->str[i]);
		if(!new[i])
			exit(-1);
		i++;
	}
	new[i] = ft_strdup(string);
	if (!new[i])
		exit(-1);
	new[i + 1] = NULL;
	i = 0;
	while (cmd->str && cmd->str[i])
		free(cmd->str[i++]);
	if (cmd->str)
		free(cmd->str);
	cmd->str = new;
}

void	split_cmds(t_split *input, t_op *cmd)
{
	t_split	*ptr;
	t_op	*c_ptr;

	c_ptr = cmd;
	ptr = input;
	while (ptr)
	{
		if (ptr->str)
		{
			if (ptr->str[0] == '|')
			{
				ptr = ptr->next;
				append_cmd(c_ptr, ptr->str);
				c_ptr = c_ptr->next;
			}
			else
				append_str(c_ptr, ptr->str);
		}
		if (ptr)
			ptr = ptr->next;
	}
}

void	append_cmd(t_op *cmd, char *string)
{
	t_op	*new;
	t_op	*ptr;

	ptr = cmd;
	new = malloc(sizeof(t_op));
	if (!new)
		exit(-1);
	new->str = NULL;
	append_str(new, string);
	new->next = NULL;
	new->append = 0;
	new->fd_in = STDIN_FILENO;
	new->fd_out = STDOUT_FILENO;
	new->prev = NULL;
	new->squote = 0;
	ptr->next = new;
}

void	backtrack_op(t_op *cmd)
{
	t_op	*back;
	t_op	*current;

	current = cmd;
	back = NULL;
	while (current)
	{
		current->prev = back;
		back = current;
		current = current->next;
	}
}
