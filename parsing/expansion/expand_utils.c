/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 15:58:26 by kmummadi          #+#    #+#             */
/*   Updated: 2025/02/19 18:25:45 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_dquote_expansion(const char *str, size_t *i, t_expand *exp,
			t_data *data);

void	expand_double_quote(const char *str, t_data *data, size_t *i,
		t_expand *exp)
{
	(*i)++;
	while (str[*i] && str[*i] != '"')
	{
		handle_dquote_expansion(str, i, exp, data);
	}
	if (str[*i] == '"')
		(*i)++;
}

void	handle_dquote_expansion(const char *str, size_t *i, t_expand *exp, t_data *data)
{
    char	*var;
    size_t	j;

    if (str[*i] == '$')
    {
        var = expand_var(&str[*i], data, i);
        if (!var) {
            return; // or decide how to handle a missing variable
        }
        j = 0;
        while (var[j])
            append_char_node(exp, var[j++]);
        free(var);
    }
    else if (str[*i] == '\\' && str[*i + 1] != '\0')
    {
        append_char_node(exp, str[*i + 1]);
        (*i) += 2;
    }
    else
    {
        append_char_node(exp, str[*i]);
        (*i)++;
    }
}


void	expand_single_quote(const char *str, size_t *i, t_expand *exp)
{
	(*i)++;
	while (str[*i] && str[*i] != '\'')
	{
		append_char_node(exp, str[*i]);
		(*i)++;
	}
	if (str[*i] == '\'')
		(*i)++;
}

void	append_char_node(t_expand *exp, char c)
{
	t_char_node	*new_node;

	new_node = malloc(sizeof(t_char_node));
	if (!new_node)
		exit(1);
	new_node->c = c;
	new_node->next = NULL;
	if (exp->expanded_tail)
	{
		(exp->expanded_tail)->next = new_node;
		exp->expanded_tail = new_node;
	}
	else
	{
		exp->expanded_head = new_node;
		exp->expanded_tail = new_node;
	}
}
