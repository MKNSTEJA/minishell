/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segregate_info.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknsteja <mknsteja@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 13:12:30 by mknsteja          #+#    #+#             */
/*   Updated: 2025/02/09 11:14:35 by mknsteja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	append_cmd(t_op *cmd, char *string);
void	split_cmds(t_split *input, t_op *cmd);

void	add_redirection(t_op *cmd, t_type type, char *filename)
{
	t_redir	*new_redir;
	t_redir	*temp;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		exit(1);
	new_redir->type = type;
	new_redir->filename = ft_strdup(filename);
	new_redir->next = NULL;
	temp = NULL;
	if (type == HEREDOC)
	{
		if (ft_strchr(filename, '\'') != NULL)
			new_redir->quoted = 1;
		else
			new_redir->quoted = 0;
	}
	else
		new_redir->quoted = 0;
	if (!cmd->redirections)
		cmd->redirections = new_redir;
	else
	{
		temp = cmd->redirections;
		while (temp->next)
			temp = temp->next;
		temp->next = new_redir;
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

void	split_cmds(t_split *input, t_op *cmd)
{
	t_split	*ptr;
	t_op	*c_ptr;
	t_split	*filename_token;

	ptr = input;
	c_ptr = cmd;
	while (ptr)
	{
		if (ptr->type == PIPES)
		{
			ptr = ptr->next;
			if (ptr && ptr->str)
				append_cmd(c_ptr, ptr->str);
			else
				append_cmd(c_ptr, NULL);
			c_ptr = c_ptr->next;
		}
		else if (ptr->type == IN || ptr->type == OUT || ptr->type == APPEND
			|| ptr->type == HEREDOC)
		{
			filename_token = ptr->next;
			if (!filename_token || filename_token->type != WORD)
			{
				ft_putstr_fd("minishell: syntax error near token ",
					STDERR_FILENO);
				ft_putstr_fd(ptr->str, STDERR_FILENO);
				ft_putstr_fd("\n", STDERR_FILENO);
				return ;
			}
			else
			{
				add_redirection(c_ptr, ptr->type, filename_token->str);
				ptr = filename_token;
			}
		}
		else
			append_str(c_ptr, ptr->str);
		if (ptr)
			ptr = ptr->next;
	}
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
