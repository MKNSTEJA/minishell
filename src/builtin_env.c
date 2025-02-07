/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:52:53 by ykhattab          #+#    #+#             */
/*   Updated: 2025/02/07 23:33:42 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	export_with_value(const char *arg, t_data *data)
{
	int		key_len;
	char	*key;
	char	*value;
	char	*equal_sign;

	equal_sign = ft_strchr(arg, '=');
	key_len = equal_sign - arg;
	key = ft_substr(arg, 0, key_len);
	if (!is_valid_identifier(key))
	{
		g_exit_code = 1;
		print_error_msg("export", key, "not a valid identifier");
		free(key);
		return ;
	}
	value = ft_substr(arg, key_len + 1, ft_strlen(arg) - (key_len + 1));
	set_env_variable(key, value, &(data->env));
	free(key);
	free(value);
}

static void	export_without_value(const char *arg, t_data *data)
{
	if (!is_valid_identifier(arg))
	{
		g_exit_code = 1;
		print_error_msg("export", arg, "not a valid identifier");
		return ;
	}
	set_env_variable(arg, "", &(data->env));
}

void	handle_export(char **argv, t_data *data)
{
	int	i;

	i = 1;
	if (!argv[1])
	{
		print_exported_environ(data->env);
		return ;
	}
	while (argv[i])
	{
		if (ft_strchr(argv[i], '='))
			export_with_value(argv[i], data);
		else
			export_without_value(argv[i], data);
		i++;
	}
}

void	handle_unset(char **argv, char **envp)
{
	const char	*error_msg;

	if (!argv[1])
	{
		error_msg = "unset: Missing argument\n";
		write(STDERR_FILENO, error_msg, strlen(error_msg));
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
