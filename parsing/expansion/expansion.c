/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 09:04:14 by mknsteja          #+#    #+#             */
/*   Updated: 2025/02/17 00:57:35 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_env_value(const char *var_name, char **envp);
void	handle_field_splitting(t_split **head, t_split **curr_ptr,
			char *expanded_str);
void	splitting_spaces(t_expand *exp, t_split **head);
void	execute_field_splitting(char **fields, t_split *curr);
// void debug_print_token_chain(t_split *head, const char *location);

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

// void debug_print_token_chain(t_split *head, const char *location) {
//     fprintf(stderr, "\n[DEBUG] Token chain at %s:\n", location);
//     t_split *curr = head;
//     while (curr) {
//         fprintf(stderr, "Token: '%s', segments: %p, next: %p, prev: %p\n",
//                 curr->str, curr->segments, curr->next, curr->prev);
//         curr = curr->next;
//     }
//     fprintf(stderr, "\n");
// }

/**
 * @brief Expands all tokens by processing each token's segments for variable
 * and tilde expansion.
 *
 * For each token in the linked list, this function processes its segments by
 * iterating over the string and performing the appropriate expansion based on
 * the context (e.g., within quotes or unquoted).
 *
 * @param head Pointer to the head of the token linked list.
 * @param data Shell data containing environment variables and other info.
 */
void	expand_tokens(t_split **head, t_data *data)
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
			loop_string(exp.seg->text, &exp, data, exp.seg);
			exp.seg = exp.seg->next;
		}
		exp.expanded_str = convert_char_list_to_string(exp.expanded_head);
		// fprintf(stderr, "[expand_tokens] Expanded string: \"%s\"\n", exp.expanded_str);
		free_char_list(exp.expanded_head);
		if (handle_empty_expanded_string(&exp, head) != 0)
			continue ;
		exp.token_unquoted = is_token_unquoted(exp.split);
		splitting_spaces(&exp, head);
	}
}

void	splitting_spaces(t_expand *exp, t_split **head)
{
    // fprintf(stderr, "[splitting_spaces] Before splitting, token: \"%s\"\n",
            // exp->split ? exp->split->str : "NULL");

    if (exp->split) {
        // fprintf(stderr, "[splitting_spaces] Current token: \"%s\", next token: \"%s\"\n",
        //         exp->split->str,
        //         exp->split->next ? exp->split->next->str : "NULL");
    }

    if (exp->token_unquoted)
    {
		// debug_print_token_chain(*head, "before field splitting");
        handle_field_splitting(head, &exp->split, exp->expanded_str);
		// debug_print_token_chain(*head, "after field splitting");
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

    // fprintf(stderr, "[splitting_spaces] After splitting, next token: \"%s\"\n",
    //         exp->split ? exp->split->str : "NULL");
}



void	handle_field_splitting(t_split **head, t_split **curr_ptr,
		char *expanded_str)
{
	t_split	*curr;
	char	**fields;
	int		i;

	i = 0;
	curr = *curr_ptr;
	// fprintf(stderr, "[handle_field_splitting] Starting with segments: %p\n", 
    //         (*curr_ptr)->segments);
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
	// fprintf(stderr, "[handle_field_splitting] First field: %s\n", fields[0]);
	execute_field_splitting(fields, curr);
	t_split *temp = curr;
    while (temp) {
        if (!temp->segments) {
            // fprintf(stderr, "[handle_field_splitting] Initializing segments for token: %s\n", 
                    // temp->str);
            temp->segments = NULL;  // or initialize with proper segment data
            temp->token_has_quotes = 0;  // initialize other fields as needed
        }
        temp = temp->next;
    }
	// fprintf(stderr, "[handle_field_splitting] After splitting - curr str: %s\n", 
    //         curr->str);
	// if (curr->next)
	// 	fprintf(stderr, "[handle_field_splitting] Next token: %s\n", curr->next->str);
	*curr_ptr = curr;
	while (fields[i])
		free(fields[i++]);
	free(fields);
}

void	execute_field_splitting(char **fields, t_split *curr)
{
	t_split	*new_node;
	int		i;

	i = 1;
	while (fields[i])
	{
		new_node = create_new_token(fields[i], WORD);
		new_node->segments = NULL;
		new_node->token_has_quotes = 0;
		
		new_node->next = curr->next;
		if (curr->next)
			curr->next->prev = new_node;
		curr->next = new_node;
		new_node->prev = curr;
		curr = new_node;
		i++;
	}
}
