/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 09:04:14 by mknsteja          #+#    #+#             */
/*   Updated: 2025/02/10 21:35:22 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*expand_one_token(char *token, char **envp, t_quote_state quote_state);
char	*expand_var(const char *str, char **envp, size_t *i);
char	*get_env_value(const char *var_name, char **envp);
void	handle_field_splitting(t_split **head, t_split **curr_ptr,
			char *expanded_str);
void	splitting_spaces(t_expand *exp, t_split **head);
void	execute_field_splitting(char **fields, t_split *curr);

char	*convert_char_list_to_string(t_char_node *head)
{
	size_t		len;
	size_t		i;
	t_char_node	*current;
	char		*str;

	len = 0;
	i = 0;
	current = head;
	while (current)
	{
		len++;
		current = current->next;
	}
	str = malloc(len + 1);
	if (!str)
		exit(EXIT_FAILURE);
	current = head;
	while (i < len)
	{
		str[i] = current->c;
		current = current->next;
		i++;
	}
	str[len] = '\0';
	return (str);
}

void	expand_tokens(t_split **head, char **envp)
{
	t_expand	exp;

	if (!head || !*head)
		return ;
	exp.split = *head;
	exp.seg = NULL;
	while (exp.split)
	{
		exp.expanded_head = NULL;
		exp.expanded_tail = NULL;
		exp.seg = exp.split->segments;
		if (exp.split->prev && exp.split->prev->type == HEREDOC)
        {
            exp.split = exp.split->next;
            continue;
        }
		while (exp.seg)
		{
			loop_string(exp.seg->text, &exp, envp, exp.seg);
			exp.seg = exp.seg->next;
		}
		exp.expanded_str = convert_char_list_to_string(exp.expanded_head);
		free_char_list(exp.expanded_head);
		if (free_expanded_str(&exp, head) != 0)
			continue ;
		exp.token_unquoted = is_token_unquoted(exp.split);
		splitting_spaces(&exp, head);
	}
}

void	splitting_spaces(t_expand *exp, t_split **head)
{
	if (exp->token_unquoted)
	{
		handle_field_splitting(head, &exp->split, exp->expanded_str);
		if (exp->expanded_str)
			free(exp->expanded_str);
		if (exp->split)
			exp->split = exp->split->next;
	}
	else
	{
		free(exp->split->str);
		exp->split->str = exp->expanded_str;
		exp->split = exp->split->next;
	}
}

void	handle_field_splitting(t_split **head, t_split **curr_ptr,
		char *expanded_str)
{
	t_split	*curr;
	char	**fields;
	int		i;

	i = 0;
	curr = *curr_ptr;
	fields = ft_split(expanded_str, ' ');
	if (!fields)
	{
		free(curr->str);
		curr->str = NULL;
		curr = remove_token(head, curr);
		*curr_ptr = curr;
		free(expanded_str);
		return ;
	}
	free(curr->str);
	curr->str = ft_strdup(fields[0]);
	execute_field_splitting(fields, curr);
	*curr_ptr = curr;
	while (fields[i])
		free(fields[i++]);
	free(fields);
}

void	execute_field_splitting(char **fields, t_split *curr)
{
	t_split	*new_node;
	int		i;

	new_node = NULL;
	i = 1;
	while (fields[i])
	{
		new_node = create_new_token(fields[i], WORD);
		new_node->next = curr->next;
		if (curr->next)
			curr->next->prev = new_node;
		curr->next = new_node;
		new_node->prev = curr;
		curr = new_node;
		i++;
	}
}
