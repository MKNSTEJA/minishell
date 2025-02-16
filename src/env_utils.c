/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 15:50:12 by ykhattab          #+#    #+#             */
/*   Updated: 2025/02/16 19:01:57 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_exported_environ(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		printf("declare -x %s\n", envp[i]);
		i++;
	}
}

void	set_env_variable(const char *key, const char *value, char ***envp)
{
	int	i;
	int	key_len;

	i = 0;
	key_len = ft_strlen(key);
	while ((*envp)[i])
	{
		if (ft_strncmp((*envp)[i], key, key_len) == 0
			&& (*envp)[i][key_len] == '=')
		{
			free((*envp)[i]);
			(*envp)[i] = create_env_string(key, value);
			return ;
		}
		i++;
	}
	add_env_variable(key, value, envp);
}

char	*create_env_string(const char *key, const char *value)
{
	int		key_len;
	int		val_len;
	char	*env_string;

	key_len = ft_strlen(key);
	if (value)
		val_len = ft_strlen(value);
	else
		val_len = 0;
	env_string = (char *)malloc(key_len + val_len + 2);
	if (!env_string)
		return (NULL);
	ft_strlcpy(env_string, key, key_len + 1);
	env_string[key_len] = '=';

	if (value && val_len > 0)
		ft_strlcpy(env_string + key_len + 1, value, val_len + 1);
	else
		env_string[key_len + 1] = '\0';
	return (env_string);
}

char	*my_getenv(const char *name, char **env)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(name);
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

void	add_env_variable(const char *key, const char *value, char ***envp)
{
	int		i;
	int		j;
	char	**new_env;

	i = 0;
	j = 0;
	while ((*envp)[i])
		i++;
	new_env = (char **)malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return ;
	while (j < i)
	{
		new_env[j] = (*envp)[j];
		j++;
	}
	new_env[i] = create_env_string(key, value);
	new_env[i + 1] = NULL;
	free(*envp);
	*envp = new_env;
}
