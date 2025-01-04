/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segregate_info.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 13:12:30 by mknsteja          #+#    #+#             */
/*   Updated: 2025/01/04 04:05:08 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	append_cmd(t_op *cmd, char *string);
void	split_cmds(t_split *input, t_op *cmd);

void add_redirection(t_op *cmd, t_type type, char *filename)
{
	t_redir *new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
	{
		perror("malloc");
		exit(1); // handle later on gracefully
	}
	new_redir->type = type;
	new_redir->filename = ft_strdup(filename);
	new_redir->next = NULL;

	// If this command has no redirections yet, new_redir is the first
	if (!cmd->redirections)
		cmd->redirections = new_redir;
	else {
		// else, append at the end of the linked list
		t_redir *temp = cmd->redirections;
		while (temp->next)
			temp = temp->next;
		temp->next = new_redir;
	}
}

t_op	*initialise_cmd(t_split *input)
{
	t_op	*cmd = calloc(1, sizeof(t_op));
	if (!cmd)
		exit(-1);
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
	// printf("Appended string: %s\n", string);
}

void	split_cmds(t_split *input, t_op *cmd)
{
	t_split *ptr = input;
    t_op    *c_ptr = cmd;

	while (ptr)
	{
		if (ptr->type == PIPES) // how would that be known? 
		{
				ptr = ptr->next; // move pointer to next token after pipe (start of next cmd)
				if (ptr && ptr->str)
					append_cmd(c_ptr, ptr->str);
				else
					append_cmd(c_ptr, NULL);
				c_ptr = c_ptr->next;
			}
			else if (ptr->type == IN || ptr->type == OUT || ptr->type == APPEND || ptr->type == HEREDOC)
			{
				//then we expect the next token to be the filename.
				t_split *filename_token = ptr->next;
				if (!filename_token || filename_token->type != WORD)
					fprintf(stderr, "syntax error near token %s\n", ptr->str);
				else
				{
					add_redirection(c_ptr, ptr->type, filename_token->str);
					// skip the filename token as well
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

	new = calloc(1, sizeof(t_op));
	if (!new)
		exit(-1);
	// printf("\n goes into append_str ");
	if (string)
		append_str(new, string);
	cmd->next = new;
	// printf("end ");
}
