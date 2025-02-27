/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env_unset.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 23:20:25 by ykhattab          #+#    #+#             */
/*   Updated: 2025/02/27 23:47:33 by ykhattab         ###   ########.fr       */
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
		handle_cd(argv, data);
	else if (strcmp(argv[0], "unset") == 0)
		handle_unset(argv, data);
	else if (strcmp(argv[0], "env") == 0)
		handle_env(argv, data);
	else if (strcmp(argv[0], "pwd") == 0)
		handle_pwd(argv, data);
	else if (strcmp(argv[0], "echo") == 0)
		handle_echo(argv, data);
	else if (strcmp(argv[0], "export") == 0)
		handle_export(argv, data);
}

void	handle_unset(char **argv, t_data *data)
{
	int	i;

	i = 1;
	if (!argv[1] || (argv[1][0] == '?' && !argv[1][1]))
		return ;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			data->last_exit = 1;
			print_error_msg("unset", argv[i], "not a valid identifier");
		}
		else
			remove_env_variable(&data->env, argv[i]);
		i++;
	}
	if (data->last_exit != 1)
		data->last_exit = 0;
}

void	handle_env(char **argv, t_data *data)
{
	char	**env;

	if (argv && argv[1])
	{
		print_error_msg("env", NULL, "too many arguments");
		data->last_exit = 1;
		return ;
	}
	env = data->env;
	if (!env)
	{
		return ;
	}
	while (*env)
	{
		printf("%s\n", *env);
		env++;
	}
	data->last_exit = 0;
}
