/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 22:38:44 by ykhattab          #+#    #+#             */
/*   Updated: 2025/02/09 00:21:55 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	count_commands(t_op *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

int	is_builtin(t_op *cmd)
{
	if (!cmd || !cmd->str || !cmd->str[0])
		return (0);
	return (strcmp(cmd->str[0], "exit") == 0
		|| strcmp(cmd->str[0], "cd") == 0
		|| strcmp(cmd->str[0], "export") == 0
		|| strcmp(cmd->str[0], "unset") == 0
		|| strcmp(cmd->str[0], "env") == 0
		|| strcmp(cmd->str[0], "pwd") == 0
		|| strcmp(cmd->str[0], "echo") == 0);
}

int	is_numeric(const char *s)
{
	int	i;

	if (!s || !*s)
		return (0);
	i = 0;
	if (s[i] == '-' || s[i] == '+')
		i++;
	while (s[i])
	{
		if (!isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

void	print_error_msg(const char *cmd, const char *arg, const char *err_msg)
{
	write(2, "Minishell: ", 11);
	write(2, cmd, ft_strlen(cmd));
	write(2, ": ", 2);
	if (arg)
	{
		write(2, arg, ft_strlen(arg));
		write(2, ": ", 2);
	}
	write(2, err_msg, ft_strlen(err_msg));
	write(2, "\n", 1);
}
