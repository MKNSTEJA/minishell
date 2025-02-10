/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_char_node.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmummadi <kmummadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 12:05:24 by kmummadi          #+#    #+#             */
/*   Updated: 2025/02/10 17:35:17 by kmummadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// int		check_escape(char *str, size_t *i, int *escaped, t_expand *exp);
void	skip_dollar(char *str, size_t *i, t_expand *exp);
void	expand_dollar(t_expand *exp, char **envp, size_t *i, char *str);
void	expand_home(char **envp, size_t *i, t_expand *exp);

void	loop_string(char *str, t_expand *exp, char **envp,
		t_segment *curr_segment)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] == '"')
			skip_dollar(str, &i, exp);
		else if (str[i] == '"' && curr_segment->quote == DQ)
			expand_double_quote(str, envp, &i, exp);
		else if (str[i] == '\'' && curr_segment->quote == SQ)
			expand_single_quote(str, &i, exp);
		else if (str[i] == '$' && curr_segment->quote != SQ)
			expand_dollar(exp, envp, &i, str);
		else if (str[i] == '~' && (!i) && (str[i + 1] == '/' || !str[i + 1]))
			expand_home(envp, &i, exp);
		else
		{
			append_char_node(exp, str[i]);
			i++;
		}
	}
}

void	skip_dollar(char *str, size_t *i, t_expand *exp)
{
	(*i) += 2;
	while (str[*i] && str[*i] != '"')
	{
		append_char_node(exp, str[*i]);
		(*i)++;
	}
	if (str[*i] == '"')
		(*i)++;
}

void	expand_dollar(t_expand *exp, char **envp, size_t *i, char *str)
{
	char	*var;
	size_t	j;

	var = expand_var(&str[*i], envp, i);
	j = 0;
	if (var)
	{
		while (var[j])
		{
			append_char_node(exp, var[j]);
			j++;
		}
		free(var);
	}
}

void	expand_home(char **envp, size_t *i, t_expand *exp)
{
	char	*home;
	size_t	j;

	home = get_env_value("HOME", envp);
	j = 0;
	if (home)
	{
		j = 0;
		while (home[j])
		{
			append_char_node(exp, home[j]);
			j++;
		}
	}
	else
		append_char_node(exp, '~');
	(*i)++;
}

// int	check_escape(char *str, size_t *i, int *escaped, t_expand *exp)
// {
// 	if (str[*i] == '\\' && !(*escaped))
// 	{
// 		(*escaped) = 1;
// 		(*i)++;
// 		return (1);
// 	}
// 	if (*escaped)
// 	{
// 		append_char_node(exp, str[*i]);
// 		(*escaped) = 0;
// 		(*i)++;
// 		return (1);
// 	}
// 	return (0);
// }