/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_char_node.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 12:05:24 by kmummadi          #+#    #+#             */
/*   Updated: 2025/02/14 02:40:29 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// int		check_escape(char *str, size_t *i, int *escaped, t_expand *exp);
void	skip_dollar(char *str, size_t *i, t_expand *exp);
void	expand_dollar(t_expand *exp, t_data *data, size_t *i, char *str);
void	expand_home(char **envp, size_t *i, t_expand *exp);

/**
 * @brief Processes a string for expansion by iterating over each character.
 *
 * Iterates through the segments' text and performs the appropriate
 * expansion operations based on the current character and its context (e.g.,
 * variable expansion, tilde expansion, or handling quotes). The resulting
 * characters are appended to the expansion's character list.
 *
 * @param str The input string (segments' text) to process.
 * @param exp Pointer to the t_expand structure holding the expansion state.
 * @param data Shell data containing environment variables and other info.
 * @param curr_segment The current segment being processed, containing quote info.
 */
void	loop_string(char *str, t_expand *exp, t_data *data,
		t_segment *curr_segment)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] == '"')
			skip_dollar(str, &i, exp);
		else if (str[i] == '"' && curr_segment->quote == DQ)
			expand_double_quote(str, data, &i, exp);
		else if (str[i] == '\'' && curr_segment->quote == SQ)
			expand_single_quote(str, &i, exp);
		else if (str[i] == '$' && curr_segment->quote != SQ)
			expand_dollar(exp, data, &i, str);
		else if (str[i] == '~' && (!i) && (str[i + 1] == '/' || !str[i + 1]))
			expand_home(data->env, &i, exp);
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

void	expand_dollar(t_expand *exp, t_data *data, size_t *i, char *str)
{
	char	*var;
	size_t	j;

	var = expand_var(&str[*i], data, i);
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