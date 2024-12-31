/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segregate_info.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknsteja <mknsteja@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 13:12:30 by mknsteja          #+#    #+#             */
/*   Updated: 2024/12/31 15:24:51 by mknsteja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	append_cmd(t_op *cmd, char *string);
void	split_cmds(t_split *input, t_op *cmd);

t_op	*initialise_cmd(t_split *input)
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
	split_cmds(input, cmd);
	return (cmd);
}

void	append_str(t_op *cmd, char *string)
{
	int		i;
	char	**new;

	i = 0;
	// if(!string)
	// {
	// 	cmd->str = NULL;
	// 	return ;
	// }
	while (cmd->str != NULL && cmd->str[i] != NULL)
	{
		// printf("0 ");
		i++;
	}
	new = malloc(sizeof(char *) * (i + 2));
	if (!new)
		exit(-1);
	i = 0;
	while (cmd->str && cmd->str[i])
	{
		// printf("1 ");
		new[i] = ft_strdup(cmd->str[i]);
		// printf("%s \n", new[i]);
		i++;
	}
	new[i] = ft_strdup(string);
	// printf("2 ");
	// printf("%s \n", new[i]);
	if (!new[i])
		exit(-1);
	new[i + 1] = NULL;
	i = 0;
	while (cmd->str && cmd->str[i])
		free(cmd->str[i++]);
	if (cmd->str)
		free(cmd->str);
	cmd->str = new;
	// printf("3 ");
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
				// printf("goes here \n");
				ptr = ptr->next;
				append_cmd(c_ptr, ptr->str);
				// printf("\ndone appending: %s", c_ptr->str[0]);
				c_ptr = c_ptr->next;
			}
			else
			{
				// printf("in\n");
				append_str(c_ptr, ptr->str);
				// printf("out\n");
			}
			// printf("\ndone appending 2\n");
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
	// printf("\n goes into append_str ");
	append_str(new, string);
	// printf("\n done appending_str ");
	new->next = NULL;
	new->append = 0;
	new->fd_in = STDIN_FILENO;
	new->fd_out = STDOUT_FILENO;
	// printf("staart ");
	ptr->next = new;
	// printf("end ");
}
