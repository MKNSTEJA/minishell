/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_char_node.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmummadi <kmummadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 12:05:24 by kmummadi          #+#    #+#             */
/*   Updated: 2025/02/01 15:11:02 by kmummadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		check_escape(char *str, size_t *i, int *escaped, t_expand *exp);
void	skip_dollar(char *str, size_t *i, t_expand *exp);
void	expand_dollar(t_expand *exp, char **envp, size_t *i, char *str);
void	expand_home(char **envp, size_t *i, t_expand *exp);

void	loop_string(char *str, t_expand **exp, char **envp,
		t_segment *curr_segment)
{
	size_t	i;
	int		escaped;

	i = 0;
	escaped = 0;
	while (str[i])
	{
		if (check_escape(str, &i, &escaped, *exp) != 0)
			continue ;
		if (str[i] == '$' && str[i + 1] == '"' && !escaped)
			skip_dollar(str, &i, *exp);
		else if (str[i] == '"' && curr_segment->quote == DQ && !escaped)
			expand_double_quote(str, envp, &i, &((*exp)->expanded_head),
				&((*exp)->expanded_tail));
		else if (str[i] == '\'' && curr_segment->quote == SQ && !escaped)
			expand_single_quote(str, &i, &((*exp)->expanded_head),
				&((*exp)->expanded_tail));
		else if (str[i] == '$' && curr_segment->quote != SQ && !escaped)
			expand_dollar(*exp, envp, &i, str);
		else if (str[i] == '~' && (!i) && (str[i + 1] == '/' || !str[i + 1]))
			expand_home(envp, &i, *exp);
        else
        {
            append_char_node(&((*exp)->expanded_head), &((*exp)->expanded_tail), str[i]);
            i++;
        }
	}
}

int	check_escape(char *str, size_t *i, int *escaped, t_expand *exp)
{
	if (str[*i] == '\\' && !escaped)
	{
		*escaped = 1;
		(*i)++;
		return (1);
	}
	else if (escaped)
	{
		append_char_node(&(exp->expanded_head), &(exp->expanded_tail), str[*i]);
		*escaped = 0;
		(*i)++;
		return (2);
	}
	return (0);
}

void	skip_dollar(char *str, size_t *i, t_expand *exp)
{
	*i += 2;
	while (str[*i] && str[*i] != '"')
	{
		append_char_node(&(exp->expanded_head), &(exp->expanded_tail), str[*i]);
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
			append_char_node(&(exp->expanded_head), &(exp->expanded_tail),
				var[j]);
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
			append_char_node(&(exp->expanded_head), &(exp->expanded_tail),
				home[j]);
			j++;
		}
	}
	else
		append_char_node(&(exp->expanded_head), &(exp->expanded_tail), '~');
	(*i)++;
}
