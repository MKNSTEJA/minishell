/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segregate_info.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknsteja <mknsteja@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 13:12:30 by mknsteja          #+#    #+#             */
/*   Updated: 2024/12/31 06:24:00 by mknsteja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	append_str(t_op *cmd, char *string)
{
	int		i;
	char	**new;

	i = 0;
	while (cmd->str && cmd->str[i])
		i++;
	new = malloc(sizeof(char *) * (i + 2));
	if (!new)
		exit(-1);
	i = 0;
	while (cmd->str && cmd->str[i++])
		ft_strlcpy(new[i], cmd->str[i], ft_strlen(cmd->str[i]));
	new[i] = ft_strdup(string);
	if (!new[i])
		exit(-1);
	new[i + 1] = NULL;
	i = 0;
	while (cmd->str && cmd->str[i])
		free(cmd->str[i]);
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
		append_str(c_ptr, ptr->str);
		ptr = ptr->next;
		if(input->str[0] == '|')
			append_cmd(cmd, input->next->str);
	}
}

void	*append_cmd(t_op *cmd, char *string)
{
	t_op	*new;
	t_op	*ptr;
	
	ptr = cmd;
	new = malloc(sizeof(t_op));
	if (!new)
		exit(-1);
	new->str = ft_strdup(string);
	new->next = NULL;
	while(ptr->next)
		ptr = ptr->next;
	ptr->next = new;
}
