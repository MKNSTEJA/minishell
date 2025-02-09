/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env_unset.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 23:20:25 by ykhattab          #+#    #+#             */
/*   Updated: 2025/02/08 23:58:22 by ykhattab         ###   ########.fr       */
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
void	handle_unset(char **argv, char **envp)
{
	const char	*error_msg;

	if (!argv[1])
	{
		error_msg = "unset: Missing argument\n";
		write(STDERR_FILENO, error_msg, ft_strlen(error_msg));
		g_exit_code = 1;
		return ;
	}
	if (!is_valid_identifier(argv[1]))
	{
		g_exit_code = 1;
		print_error_msg("unset", argv[1], "not a valid identifier");
		return ;
	}
	remove_env_variable(&envp, argv[1]);
	g_exit_code = 0;
}

void	handle_env(char **argv, t_data *data)
{
	char	**env;

	if (argv && argv[1])
	{
		print_error_msg("env", NULL, "too many arguments");
		g_exit_code = 1;
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
	g_exit_code = 0;
}
