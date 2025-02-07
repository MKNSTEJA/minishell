/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 22:35:39 by ykhattab          #+#    #+#             */
/*   Updated: 2025/02/07 23:46:41 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	execute_commands(t_op *cmd, t_data *data)
{
	if (cmd && cmd->next != NULL)
		execute_pipeline(cmd, data);
	else
		execute_simple_command(cmd, data);
}

static char	*check_relative_path(char *cmd)
{
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (access(cmd, X_OK) == 0)
			return (strdup(cmd));
		else
			return (NULL);
	}
	return (NULL);
}

static char	*join_path_with_cmd(char *path, char *cmd)
{
	char	*slash;
	char	*full_path;

	slash = ft_strjoin(path, "/");
	full_path = ft_strjoin(slash, cmd);
	free(slash);
	return (full_path);
}

static char	*try_paths(char **paths, char *cmd)
{
	char	*full_path;

	while (*paths)
	{
		full_path = join_path_with_cmd(*paths, cmd);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		paths++;
	}
	return (NULL);
}

char	*find_executable(char **argv, char **envp)
{
	char	*executable_path;
	char	*path;
	char	**split_path;

	if (argv[0][0] == '/' || argv[0][0] == '.')
		return (check_relative_path(argv[0]));
	path = get_env_value("PATH", envp);
	if (!path)
		path = "/bin:/usr/bin";
	split_path = ft_split(path, ':');
	if (!split_path)
		return (NULL);
	executable_path = try_paths(split_path, argv[0]);
	free(split_path);
	return (executable_path);
}
