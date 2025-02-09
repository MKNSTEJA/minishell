/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_exit_pwd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:47:58 by ykhattab          #+#    #+#             */
/*   Updated: 2025/02/08 23:52:41 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	handle_exit_args(char **argv, int count)
{
	int	status;

	status = 0;
	if (!is_numeric(argv[1]))
	{
		print_error_msg("exit", argv[1], "numeric argument required");
		exit(255);
	}
	else if (count > 2)
	{
		print_error_msg("exit", NULL, "too many arguments");
		exit(1);
	}
	else
	{
		status = ft_atoi(argv[1]) % 256;
		if (status < 0)
			status += 256;
	}
	return (status);
}

void	handle_exit(char **argv)
{
	int	status;
	int	count;

	status = 0;
	count = 0;
	while (argv[count])
		count++;
	if (count > 1)
		status = handle_exit_args(argv, count);
	exit(status);
}

int	is_n_flag(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] != '-')
	{
		return (0);
	}
	i++;
	while (arg[i] == 'n')
	{
		i++;
	}
	return (arg[i] == '\0');
}

void	handle_echo(char **argv)
{
	int	first_arg;
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (argv[i] && is_n_flag(argv[i]))
	{
		newline = 0;
		i++;
	}
	first_arg = 1;
	while (argv[i])
	{
		if (!first_arg)
			printf(" ");
		else
			first_arg = 0;
		printf("%s", argv[i]);
		i++;
	}
	g_exit_code = 0;
	if (newline)
		printf("\n");
}

void	handle_pwd(char **argv, char **envp)
{
	char		*pwd;
	const char	*error_msg;

	(void)argv;
	pwd = my_getenv("PWD", envp);
	if (pwd)
	{
		printf("%s\n", pwd);
		g_exit_code = 0;
	}
	else
	{
		error_msg = "pwd: Unable to retrieve current directory\n";
		write(STDERR_FILENO, error_msg, ft_strlen(error_msg));
	}
}
