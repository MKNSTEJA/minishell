/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 19:55:28 by ykhattab          #+#    #+#             */
/*   Updated: 2025/02/06 20:42:54 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	update_pwd_and_oldpwd(char *old_pwd, char ***envp)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		set_env_variable("OLDPWD", old_pwd, envp);
		set_env_variable("PWD", cwd, envp);
		free(cwd);
	}
}

static void	handle_home_directory(char ***envp, char *old_pwd)
{
	char	*home;

	home = my_getenv("HOME", *envp);
	if (home)
	{
		if (!chdir(home))
			update_pwd_and_oldpwd(old_pwd, envp);
	}
	else
	{
		fprintf(stderr, "cd: HOME environment variable is not set\n");
	}
}

static void	handle_oldpwd_directory(char ***envp, char *old_pwd)
{
	char	*oldpwd_env;
	char	*cwd;

	oldpwd_env = my_getenv("OLDPWD", *envp);
	if (!oldpwd_env)
	{
		fprintf(stderr, "cd: OLDPWD not set\n");
		return ;
	}
	if (chdir(oldpwd_env) == 0)
	{
		cwd = getcwd(NULL, 0);
		if (cwd)
		{
			set_env_variable("OLDPWD", old_pwd, envp);
			set_env_variable("PWD", cwd, envp);
			printf("%s\n", cwd);
			free(cwd);
		}
	}
	else
		perror("cd");
	free(old_pwd);
	return ;
}

void	handle_cd(char **argv, char ***envp)
{
	char	*old_pwd;

	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
	{
		perror("getcwd");
		g_exit_code = 1;
		return ;
	}
	if (!argv[1] || (argv[1][0] == '~' && argv[1][1] == '\0'))
		handle_home_directory(envp, old_pwd);
	else if (argv[1][0] == '-' && argv[1][1] == '\0')
		handle_oldpwd_directory(envp, old_pwd);
	else
	{
		if (chdir(argv[1]) == 0)
			update_pwd_and_oldpwd(old_pwd, envp);
		else
			fprintf(stderr, "cd: no such file or directory: %s\n", argv[1]);
		free(old_pwd);
	}
	g_exit_code = 0;
}
