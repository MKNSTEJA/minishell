/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmummadi <kmummadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 20:19:28 by kmummadi          #+#    #+#             */
/*   Updated: 2025/02/10 17:59:03 by kmummadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*expand_var(const char *str, char **envp, size_t *i)
{
	size_t	start;
	size_t	var_len;
	char	*var_name;
	char	*value;

	start = 1;
	var_len = 0;
	if (str[1] == '?')
		return ((*i) += 2, ft_itoa(g_exit_code));
	while (ft_isalnum(str[start + var_len]) || str[start + var_len] == '_')
		var_len++;
	if (var_len == 0)
		return ((*i)++, ft_strdup("$"));
	var_name = ft_substr(str, start, var_len);
	if (!var_name)
		return (ft_strdup(""));
	(*i) += var_len + 1;
	value = my_getenv(var_name, envp);
	free(var_name);
	if (value)
		return (ft_strdup(value));
	else
		return (ft_strdup(""));
}

char	*get_env_value(const char *var_name, char **envp)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(var_name);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, len) == 0 && envp[i][len] == '=')
		{
			return (envp[i] + len + 1);
		}
		i++;
	}
	return (NULL);
}
