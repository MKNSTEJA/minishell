/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segregate_info.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknsteja <mknsteja@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 13:12:30 by mknsteja          #+#    #+#             */
/*   Updated: 2024/12/25 16:51:11 by mknsteja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/minishell.h"

void append_str (t_op *cmd, char *string)
{
	int i = 0;
	char **new;
	while(cmd->str && cmd->str[i])
		i++;
	new = malloc(sizeof(char *) * (i + 2));
	if(!new)
		exit(-1);
	i = 0;
	while(cmd->str && cmd->str[i++])
		ft_strlcpy(new[i], cmd->str[i], ft_strlen(cmd->str[i]));
	new[i] = ft_strdup(string);
	if(!new[i])
		exit(-1);
	new[i + 1] = NULL;
	i = 0;
	while(cmd->str && cmd->str[i])
		free(cmd->str[i]);
	if(cmd->str)
		free(cmd->str);
	cmd->str = new;
}

void split_cmds(t_split *input)
{
	t_split *ptr;
	ptr = input;

}
