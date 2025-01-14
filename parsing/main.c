/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 02:42:19 by mknsteja          #+#    #+#             */
/*   Updated: 2025/01/14 20:21:56 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int g_exit_code = 0;


// void	print_cmd(t_op *cmd);
void	free_split(t_split *list);
void	print_split(t_split *input);
void	free_op(t_op *cmd);
t_split *split_inputs(char *string);
int      split_errors(t_split *input);
t_op    *initialise_cmd(t_split *input);



// Function to retrieve the value of an environment variable
char *get_env_value(const char *var_name, char **envp)
{
    size_t len = ft_strlen(var_name);
    for (int i = 0; envp[i] != NULL; i++)
    {
        // Check if the current env string starts with var_name followed by '='
        if (ft_strncmp(envp[i], var_name, len) == 0 && envp[i][len] == '=')
        {
            return envp[i] + len + 1; // Return pointer to the value part
        }
    }
    return NULL; // Variable not found
}

t_split *create_new_token(char *str, t_type type, t_quote_state quote_state)
{
	t_split *new_node = malloc(sizeof(t_split));
	if (!new_node)
	{
		perror("malloc");
		exit(1);
	}
	new_node->str = ft_strdup(str);
	new_node->type = type;
	new_node->quote_state = quote_state;
	new_node->prev = NULL;
	new_node->next = NULL;
	return new_node;
}

void handle_field_splitting(t_split **head, t_split **curr_ptr, char *expanded_str)
{
    t_split *curr = *curr_ptr;

    // split expanded_str on whitespace
    // e.g. "Hello   World" => [ "Hello", "World" ]

    char **fields = ft_split(expanded_str, ' ');
    if (!fields)
    {
        // if ft_split_whitespace fails or returns NULL, remove the token entirely
        free(curr->str);
        curr->str = NULL;
        curr = remove_token(head, curr);
        *curr_ptr = curr;
		free(expanded_str);
        return;
    }

    // The first field becomes the current token’s string
    free(curr->str);
    curr->str = ft_strdup(fields[0]);

    // Any subsequent fields become new tokens inserted after `curr`
    int i = 1;
    while (fields[i])
    {
        t_split *new_node = create_new_token(fields[i], WORD, QUOTE_NONE);
        // Insert new_node after curr
        new_node->next = curr->next;
        if (curr->next)
            curr->next->prev = new_node;
        curr->next = new_node;
        new_node->prev = curr;

        curr = new_node;  // move to newly created node
        i++;
    }

    // curr now points to the last inserted token
    // so update *curr_ptr to it, so the main loop can continue from there
    *curr_ptr = curr;

    // free the fields array
    i = 0;
    while (fields[i])
        free(fields[i++]);
    free(fields);
}


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
void expand_tokens(t_split **head, char **envp)
{
	t_split *curr = *head;
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
			curr = remove_token(head, curr);
			free(expanded_str);
			continue ;
		}
		// if expansion introduces whitespace, split into multiple tokens
		if (curr->quote_state == QUOTE_NONE)
			{
				handle_field_splitting(head, &curr, expanded_str);
				free(expanded_str);
			}
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
	(void)envp;
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
	// if var_len = 0 => no variable name => return "$"
	if (var_len == 0)
	{
		(*i)++;
		return ft_strdup("$");
	}

	// build the variable name
	char *var_name = ft_substr(str, start, var_len);
	if (!var_name)
		return ft_strdup(""); //fallback if variable not set
	// skip over the var name
	(*i) += var_len + 1;

	// char *value = get_env_value(var_name, envp);
	char *value = getenv(var_name);
	free(var_name);
	if (value)
	{
		// duplicate the value to ensure it is heap-allocated
		char *duplicated_value = ft_strdup(value);
		if (!duplicated_value)
			return ft_strdup(""); //fallback if allocation fails
		return duplicated_value;
	}
	else
		return ft_strdup(""); //fallback if variable not set
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
	if (!expanded)  // Check for ft_strdup failure
        return NULL;
    char   *tmp = NULL;

    while (token && token[i])
	{
		if (token[i] == '$' && quote_state != QUOTE_SINGLE)
		{
			// expand the variable
			char *var_value = expand_var(&token[i], envp, &i);
			if (!var_value)
				var_value = ft_strdup(""); // ensure var_value is not NULL
			// append the expansion to the expanded string
			tmp = ft_strjoin(expanded, var_value);
			free(var_value);
			free(expanded);
			if (!tmp)
				return NULL;
			expanded = tmp;
		}
		else if (i == 0 && token[i] == '~' && (token[i+1] == '/' || token[i+1] == '\0'))
		{
			// expand the tilde
			char *env_home = get_env_value("HOME", envp);
			char *home;
			if (env_home)
			{
				home = ft_strdup(env_home);
				if (!home)
					home = ft_strdup("~"); //fallback if HOME not set
			}
			else
				home = ft_strdup("~"); //fallback if HOME not set
				
			tmp = ft_strjoin(expanded, home);
			free(home);
			free(expanded);
			if (!tmp)
				return NULL;
			expanded = tmp;
			i++;
		}
		else
		{
			char onechar[2] = {token[i], '\0'};
			tmp = ft_strjoin(expanded, onechar);
			free(expanded);
			if (!tmp)
				return NULL;
			expanded = tmp;
			i++;
		}
	}
	if (!tmp)
	{
		// handling ft_strjoin failure
		free(expanded);
		return NULL;
	}
	return expanded;
}

int	main(int argc, char **argv, char **envp)
{
	extern char **environ;
	t_split	*input;
	t_op	*cmd;
	char	*str;
	(void)argc;
    (void)argv;

	input = NULL;
	cmd = NULL;
	while (1)
	{
		/*
         * 1) If input is from an interactive terminal, use readline.
         * 2) Otherwise (piped or from a file/tester), use get_next_line.
         */
        if (isatty(fileno(stdin)))
        {
            str = readline("Minishell: ");
			if(!ft_strncmp(str, "exit", ft_strlen("exit")) || !str)
				break;
        }
        else
        {
            char *line = get_next_line(fileno(stdin));
            if (!line)  // get_next_line might return NULL if EOF reached
            {
                // If there's no more input, we should clean up and exit.
                // Possibly break or return, depending on your logic.
                break;
            }
            str = ft_strtrim(line, "\n");
            free(line);
        }
		// str = readline("\nMinishell: ");
		if (!str) // user pressed Ctrl+D perhaps
			break;
		add_history(str);
		// if(!ft_strncmp(str, "exit", ft_strlen("exit")) || !str)
		// 	break;
		input = split_inputs(str);
		// print input
		// print_split(input);
		
		expand_tokens(&input, envp);
		// expand_tokens(&input, environ);

		
		if (split_errors(input) == 1)
		{
			free_split(input);
			input = NULL;
			free(str);
			continue ;
		}
		
		// convert t_split -> t_op
		cmd = initialise_cmd(input);
		execute_commands(cmd);
		// print_cmd(cmd);
		free_split(input);
		free_op(cmd);
		free(str);
		input = NULL;
		cmd = NULL;
	}
	free(str);
	rl_clear_history();
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
			free(current);
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

// void	print_cmd(t_op *cmd)
// {
// 	t_op	*ptr;
// 	int		i;
// 	int 	counter;

// 	ptr = cmd;
// 	i = 0;
// 	counter = 0;
// 	while (ptr)
// 	{
// 		i = 0;
// 		printf("String inside %d: \n", counter);
// 		while (ptr->str && ptr->str[i])
// 		{
// 			printf("%s ", ptr->str[i]);
// 			i++;
// 		}
// 		printf("\n");
// 		ptr = ptr->next;
// 		counter++;
// 	}
// 	printf("\n");
// }

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


