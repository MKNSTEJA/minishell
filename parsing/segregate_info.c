/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segregate_info.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 13:12:30 by mknsteja          #+#    #+#             */
/*   Updated: 2025/02/26 22:01:33 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	append_cmd(t_op *cmd, char *string);
t_op	*initialise_cmd(t_split *input);
void	append_str(t_op *cmd, char *string);
void	check_redir_quotes(t_type type, t_redir **new_redir, t_op **cmd,
			char *filename);

void	add_redirection(t_op *cmd, t_type type, char *filename,
		int token_has_quotes)
{
	t_redir	*new_redir;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		exit(1);
	new_redir->type = type;
	new_redir->filename = ft_strdup(filename);
	new_redir->next = NULL;
	new_redir->quoted = token_has_quotes;
	check_redir_quotes(type, &new_redir, &cmd, filename);
}

void	check_redir_quotes(t_type type, t_redir **new_redir, t_op **cmd,
		char *filename)
{
	t_redir	*temp;

	(void)type;
	(void)filename;
	if (!(*cmd)->redirections)
		(*cmd)->redirections = (*new_redir);
	else
	{
		temp = (*cmd)->redirections;
		while (temp->next)
			temp = temp->next;
		temp->next = (*new_redir);
	}
}

t_op	*initialise_cmd(t_split *input)
{
	t_op	*cmd;

	cmd = ft_calloc(1, sizeof(t_op));
	if (!cmd)
		exit(1);
	split_cmds(input, cmd);
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
		exit(1);
	i = 0;
	while (cmd->str && cmd->str[i])
	{
		new[i] = ft_strdup(cmd->str[i]);
		i++;
	}
	new[i] = ft_strdup(string);
	if (!new[i])
		exit(1);
	new[i + 1] = NULL;
	i = 0;
	while (cmd->str && cmd->str[i])
		free(cmd->str[i++]);
	if (cmd->str)
		free(cmd->str);
	cmd->str = new;
}

void	append_cmd(t_op *cmd, char *string)
{
	t_op	*new;

	new = ft_calloc(1, sizeof(t_op));
	if (!new)
		exit(1);
	if (string)
		append_str(new, string);
	cmd->next = new;
}
