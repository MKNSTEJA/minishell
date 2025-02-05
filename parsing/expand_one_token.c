/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_one_token.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmummadi <kmummadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 20:41:13 by kmummadi          #+#    #+#             */
/*   Updated: 2025/02/05 21:14:38 by kmummadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*expand_dollar_token(char *token, char **envp, size_t *i,
			char *expanded);
char	*expand_home_env(char **envp, size_t *i, char *expanded);
char	*strjoin_expanded(char *token, size_t *i, char *expanded);

char	*expand_one_token(char *token, char **envp, t_quote_state quote_state)
{
	size_t	i;
	char	*expanded;

	i = 0;
	expanded = ft_strdup("");
	if (!expanded)
		return (NULL);
	while (token && token[i])
	{
		if (token[i] == '$' && quote_state != SQ)
			expanded = expand_dollar_token(token, envp, &i, expanded);
		else if (!i && token[i] == '~' && (token[i + 1] == 47 || !token[i + 1]))
			expanded = expand_home_env(envp, &i, expanded);
		else
			expanded = strjoin_expanded(token, &i, expanded);
		if (!expanded)
			return (NULL);
	}
	return (expanded);
}

char	*expand_dollar_token(char *token, char **envp, size_t *i,
		char *expanded)
{
	char	*var_value;
	char	*tmp;

	var_value = expand_var(&token[*i], envp, i);
	if (!var_value)
		var_value = ft_strdup("");
	tmp = ft_strjoin(expanded, var_value);
	free(var_value);
	free(expanded);
	if (!tmp)
		return (NULL);
	expanded = tmp;
	return (expanded);
}

char	*expand_home_env(char **envp, size_t *i, char *expanded)
{
	char	*env_home;
	char	*home;
	char	*tmp;

	home = NULL;
	env_home = get_env_value("HOME", envp);
	if (env_home)
	{
		home = ft_strdup(env_home);
		if (!home)
			home = ft_strdup("~");
	}
	else
		home = ft_strdup("~");
	tmp = ft_strjoin(expanded, home);
	free(home);
	free(expanded);
	if (!tmp)
		return (NULL);
	expanded = tmp;
	(*i)++;
	return (expanded);
}

char	*strjoin_expanded(char *token, size_t *i, char *expanded)
{
	char	*tmp;
	char	onechar[2];

	onechar[0] = token[*i];
	onechar[1] = '\0';
	tmp = ft_strjoin(expanded, onechar);
	free(expanded);
	if (!tmp)
		return (NULL);
	expanded = tmp;
	(*i)++;
	return (expanded);
}
