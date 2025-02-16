/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 22:35:39 by ykhattab          #+#    #+#             */
/*   Updated: 2025/02/16 21:31:16 by ykhattab         ###   ########.fr       */
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
	char *cwd_cmd = ft_strjoin("./", cmd);
	if (!cwd_cmd)
		return (NULL);
	
	if (access(cwd_cmd, X_OK) == 0)
		return (cwd_cmd);
	
	free(cwd_cmd);
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
	// if (!path)
	// 	path = "/bin:/usr/bin";
	if (!path || ft_strlen(path) == 0)  // If PATH is unset or empty
	{
		executable_path = check_relative_path(argv[0]); // Check in cwd
		if (executable_path)
			return (executable_path);
		// print_error_msg("find_executable", "PATH is not set or empty", "");
		print_error_msg(argv[0], NULL, "No such file or directory");
		return (NULL);  // Prevent trying to find executables without PATH
	}
	split_path = ft_split(path, ':');
	if (!split_path)
		return (NULL);
	executable_path = try_paths(split_path, argv[0]);
	free(split_path);
	return (executable_path);
}
