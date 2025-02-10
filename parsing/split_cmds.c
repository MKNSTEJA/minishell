/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmummadi <kmummadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 15:09:37 by kmummadi          #+#    #+#             */
/*   Updated: 2025/02/10 17:47:51 by kmummadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	split_cmd_pipes(t_split **ptr, t_op **c_ptr);
int		split_cmd_redirections(t_split **ptr, t_split **filename_token,
			t_op **c_ptr);

void	split_cmds(t_split *input, t_op *cmd)
{
	t_split	*ptr;
	t_op	*c_ptr;
	t_split	*filename_token;

	ptr = input;
	c_ptr = cmd;
	filename_token = (ptr);
	while (ptr)
	{
		if (ptr->type == PIPES)
			split_cmd_pipes(&ptr, &c_ptr);
		else if (ptr->type == IN || ptr->type == OUT || ptr->type == APPEND
			|| ptr->type == HEREDOC)
		{
			if (!split_cmd_redirections(&ptr, &filename_token, &c_ptr))
				return ;
		}
		else
			append_str(c_ptr, ptr->str);
		if (ptr)
			ptr = ptr->next;
	}
}

void	split_cmd_pipes(t_split **ptr, t_op **c_ptr)
{
	(*ptr) = (*ptr)->next;
	if ((*ptr) && (*ptr)->str)
		append_cmd(*c_ptr, (*ptr)->str);
	else
		append_cmd((*c_ptr), NULL);
	(*c_ptr) = (*c_ptr)->next;
}

int	split_cmd_redirections(t_split **ptr, t_split **filename_token,
		t_op **c_ptr)
{
	(*filename_token) = (*ptr)->next;
	// if (*filename_token)
	// 	printf("file name token type: %d", (*filename_token)->type);
	// else
	// 	printf("NULL!\n");
	if (!(*filename_token) || (*filename_token)->type != WORD)
	{
		ft_putstr_fd("minishell: syntax error near token ", STDERR_FILENO);
		ft_putstr_fd((*ptr)->str, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		return (0);
	}
	else
	{
		add_redirection(*c_ptr, (*ptr)->type, (*filename_token)->str);
		(*ptr) = (*filename_token);
	}
	return (1);
}
