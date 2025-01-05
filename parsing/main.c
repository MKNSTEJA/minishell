/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 02:42:19 by mknsteja          #+#    #+#             */
/*   Updated: 2025/01/05 07:31:19 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_cmd(t_op *cmd);
void	free_split(t_split *list);
void	print_split(t_split *input);
void	free_op(t_op *cmd);
t_split *split_inputs(char *string);
int      split_errors(t_split *input);
t_op    *initialise_cmd(t_split *input);

/*
** expand_tokens
**    Scans through the linked list 'tokens', performs variable expansion
**    ($VAR, $?, etc.), tilde expansion (~), optional wildcard expansion.
**   
**    If expansions introduce new whitespace, we may need to split a single
**    token into multiple tokens. Similarly, an empty expansion might remove
**    the token.
**
**    'envp' or a custom env structure can be used to fetch environment values.
**    For special parameters ($?, $#, etc.) we hardcode them (they're not in env).
*/
void expand_tokens(t_split *head, char **envp)
{
	t_split *curr = head;
	while (curr)
	{
		// if single_quoted, skip
		if (curr->quote_state == QUOTE_SINGLE)
		{
			curr = curr->next;
			continue ;
		}
		// if double_quoted, expand variables
		char *expanded_str = expand_one_token(curr->str, envp, curr->quote_state);
		// if expansion yields no text, remove the token
		if (!expanded_str || expanded_str[0] == '\0')
		{
			free(curr->str);
			curr->str = NULL;
			// remove the token
			curr = remove_token(&head, curr);
			free(expanded_str);
			continue ;
		}
		// if expansion introduces whitespace, split into multiple tokens
		if (curr->quote_state == QUOTE_NONE)
			handle_field_splitting(&head, &curr, expanded_str);
		else
		{
			//double-quoted => keep as one token
			free(curr->str);
			curr->str = expanded_str;
		}
		
		curr = curr->next;
	}
}
/*
 * Expands shell-like variables within a string.
 * Specifically, it handles the following cases:
 *
 * 1. Special Variable $?:
 *    - Represents the exit status of the last executed command.
 *
 * 2. Standard Variables $VAR:
 *    - Represents environment variables.
 *
 * 3. Edge Cases:
 *    - Handles scenarios like '$' without a valid variable name.
 */

char *expand_var(const char *str, char **envp, size_t *i)
{
	// str[0] == '$'
	size_t start = 1;
	if (str[1] == '?')
	{
		(*i)+= 2;
		return ft_itoa(g_exit_code);
	}
	// parse the variable name
	size_t var_len = 0;
	while (ft_isalnum(str[start + var_len]) || str[start + var_len] == '_')
		var_len++;
}

t_split *remove_token(t_split **head, t_split *token)
{
	t_split *prev = token->prev;
	t_split *next = token->next;
	if (prev)
		prev->next = next;
	if (next)
		next->prev = prev;
	if (token == *head)
		*head = next;
	free(token->str);
	free(token);
	return next;
}

char *expand_one_token(char *token, char **envp, t_quote_state quote_state)
{
	size_t i = 0;
    char   *expanded = ft_strdup("");  // start empty
    char   *tmp = NULL;

    while (token && token[i])
	{
		if (token[i] == '$' && quote_state != QUOTE_SINGLE)
		{
			// expand the variable
			char *var_value = expand_var(&token[i], envp, &i);
			// append the expansion to the expanded string
			tmp = ft_strjoin(expanded, var_value);
			free(expanded);
			expanded = tmp;
			free(var_value);
		}
		else if (i == 0 && token[i] == '~' && (token[i+1] == '/' || token[i+1] == '\0'))
		{
			// expand the tilde
			char *home = get_env_value("HOME", envp);
			if (!home)
				home = "~"; //fallback if HOME not set
			tmp = ft_strjoin(expanded, home);
			free(expanded);
			expanded = tmp;
			i++;
		}
		else
		{
			char onechar[2] = {token[i], '\0'};
			tmp = ft_strjoin(expanded, onechar);
			free(expanded);
			expanded = tmp;
			i++;
		}
	}
	return expanded;
}

int	main(void)
{
	t_split	*input;
	t_op	*cmd;
	char	*str;

	input = NULL;
	cmd = NULL;
	while (1)
	{
		str = readline("\nMinishell: ");
		if (!str) // user pressed Ctrl+D perhaps
			break;
		add_history(str);
		input = split_inputs(str);
		// print input
		print_split(input);
		
		// expand_tokens(input);

		
		if (split_errors(input) == 1)
		{
			free_split(input);
			input = NULL;
			free(str);
			continue ;
		}
		
		// convert t_split -> t_op
		cmd = initialise_cmd(input);
		// print_cmd(cmd);
		execute_commands(cmd);
		//clean up
		free_split(input);
		free_op(cmd);
		free(str);
		input = NULL;
		cmd = NULL;
	}
	return 0;
	// system("leaks minishell");
}

void	free_split(t_split *list)
{
	t_split	*current;
	t_split	*next_node;

	if (!list)
		return ;
	current = list;
	while (current)
	{
		next_node = current->next;
		if (current->str)
		{
			free(current->str);
			current->str = NULL;
		}
		if (current)
		{
			free(current);
			current = NULL;
		}
		current = next_node;
	}
}

void	print_split(t_split *input)
{
	t_split	*ptr;

	ptr = input;
	while (ptr)
	{
		printf("string = %s |-> token = %d\n", ptr->str, ptr->type);
		ptr = ptr->next;
	}
}

void	print_cmd(t_op *cmd)
{
	t_op	*ptr;
	int		i;
	int 	counter;

	ptr = cmd;
	i = 0;
	counter = 0;
	while (ptr)
	{
		i = 0;
		printf("String inside %d: \n", counter);
		while (ptr->str && ptr->str[i])
		{
			printf("%s ", ptr->str[i]);
			i++;
		}
		printf("\n");
		ptr = ptr->next;
		counter++;
	}
	printf("\n");
}

void	free_op(t_op *cmd)
{
	t_op	*ptr;
	t_op	*next_ptr;
	int		i;

	ptr = cmd;
	while (ptr)
	{
		next_ptr = ptr->next;
		// 1. free the array of strings
		if (ptr->str)
		{
			i = 0;
			while (ptr->str[i])
			{
				free(ptr->str[i]);
				ptr->str[i] = NULL;
				i++;
			}
			free(ptr->str);
			ptr->str = NULL;
		}
		// 2. free any redirections
       
        t_redir *redir = ptr->redirections;
        while (redir)
        {
            t_redir *temp = redir->next;
            free(redir->filename);
			redir->filename = NULL;
            free(redir);
            redir = temp;
        }

		// 3. free the node itself
		free(ptr);
		ptr = NULL;
		ptr = next_ptr;
	}
}
