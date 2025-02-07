/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 23:20:25 by ykhattab          #+#    #+#             */
/*   Updated: 2025/02/07 23:58:49 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	execute_builtin(t_op *cmd, t_data *data)
{
	char	**argv;

	argv = cmd->str;
	if (!argv || !argv[0])
		return ;
	if (strcmp(argv[0], "exit") == 0)
		handle_exit(argv);
	else if (strcmp(argv[0], "cd") == 0)
		handle_cd(argv, &(data->env));
	else if (strcmp(argv[0], "unset") == 0)
		handle_unset(argv, data->env);
	else if (strcmp(argv[0], "env") == 0)
		handle_env(argv, data);
	else if (strcmp(argv[0], "pwd") == 0)
		handle_pwd(argv, data->env);
	else if (strcmp(argv[0], "echo") == 0)
		handle_echo(argv);
	else if (strcmp(argv[0], "export") == 0)
		handle_export(argv, data);
}
