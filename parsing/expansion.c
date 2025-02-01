/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 09:04:14 by mknsteja          #+#    #+#             */
/*   Updated: 2025/02/01 22:42:16 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char *expand_one_token(char *token, char **envp, t_quote_state quote_state);
char *expand_var(const char *str, char **envp, size_t *i);
char *get_env_value(const char *var_name, char **envp);
t_split *remove_token(t_split **head, t_split *token);
t_split *create_new_token(char *str, t_type type);
void handle_field_splitting(t_split **head, t_split **curr_ptr, char *expanded_str);
void append_char_node(t_char_node **head, t_char_node **tail, char c);
int is_token_unquoted(t_split *token);


int is_token_unquoted(t_split *token)
{
    t_segment *seg = token->segments;
    while (seg)
    {
        if (seg->quote != QUOTE_NONE)
            return 0;
        seg = seg->next;
    }
    return 1;
}


void expand_double_quote(const char *str, char **envp, size_t *i, t_char_node **head, t_char_node **tail)
{
    (*i)++; // Skip the opening double quote
    while (str[*i] && str[*i] != '"')
    {
        if (str[*i] == '$')
        {
            // Handle variable expansion
            char *var = expand_var(&str[*i], envp, i);
            for (size_t j = 0; var[j]; j++)
                append_char_node(head, tail, var[j]);
            free(var);
        }
        else if (str[*i] == '\\' && str[*i + 1] != '\0')
        {
            // Handle escape sequences within double quotes
            append_char_node(head, tail, str[*i + 1]);
            (*i) += 2;
        }
        else
        {
            append_char_node(head, tail, str[*i]);
            (*i)++;
        }
    }
    if (str[*i] == '"')
        (*i)++;
    else
    {
        // will see how to handle mismatched quotes
    }
}


void expand_single_quote(const char *str, size_t *i, t_char_node **head, t_char_node **tail)
{
    (*i)++; // Skip the opening single quote
    while (str[*i] && str[*i] != '\'')
    {
        append_char_node(head, tail, str[*i]);
        (*i)++;
    }
    if (str[*i] == '\'')
        (*i)++;
    else
    {
        // will see how to handle mismatched quotes
    }
}


char *convert_char_list_to_string(t_char_node *head)
{
	size_t len = 0;
	size_t i = 0;
	t_char_node *current = head;
	while (current)
	{
		len++;
		current = current->next;
	}
	char *str = malloc(len + 1);
	if (!str)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	current = head;
	while (i < len)
	{
		str[i] = current->c;
		current = current->next;
		i++;
	}
	str[len] = '\0';
	return str;
}

void free_char_list(t_char_node *head)
{
	t_char_node *current = head;
	while (current)
	{
		t_char_node *next = current->next;
		free(current);
		current = next;
	}
}



void append_char_node(t_char_node **head, t_char_node **tail, char c)
{
	t_char_node *new_node = malloc(sizeof(t_char_node));
	if (!new_node)
	{
		perror("malloc");
		exit(1);
	}
	new_node->c = c;
	new_node->next = NULL;
	if (*tail)
	{
		(*tail)->next = new_node;
		*tail = new_node;
	}
	else
	{
		*head = new_node;
		*tail = new_node;
	}
}


void expand_tokens(t_split **head, char **envp)
{
	if (!head || !*head)
		return;
	t_split *curr = *head;
	int escaped = 0;
	while (curr)
	{
		// printf("Expanding token: %s\n", curr->str);
		t_char_node *expanded_head = NULL;
		t_char_node *expanded_tail = NULL;

		t_segment *curr_segment = curr->segments;
		while (curr_segment)
		{
			char *str = curr_segment->text;
			size_t i = 0;

			while (str[i])
			{
				// printf("Inspecting char: %c (at index %zu)\n", str[i], i);
				// $"..."
				if (str[i] == '\\' && !escaped)
				{
					escaped = 1;
					i++;
					continue;
				}
				if (escaped)
				{
					append_char_node(&expanded_head, &expanded_tail, str[i]);
					escaped = 0;
					i++;
					continue;
				}
				if (str[i] == '$' && str[i + 1] == '"' && !escaped)
				{
					i += 2;
					while (str[i] && str[i] != '"')
					{
						append_char_node(&expanded_head, &expanded_tail, str[i]);
						i++;
					}
					if (str[i] == '"')
						i++;
				}
				else if (str[i] == '"' && curr_segment->quote == DQ && !escaped)
					expand_double_quote(str, envp, &i, &expanded_head, &expanded_tail);
				else if (str[i] == '\'' && curr_segment->quote == SQ && !escaped)
					expand_single_quote(str, &i, &expanded_head, &expanded_tail);
				else if (str[i] == '$' && curr_segment->quote != SQ && !escaped)
				{
					char *var = expand_var(&str[i], envp, &i);
					if (var)
					{
						size_t j = 0;
						while (var[j])
						{
							append_char_node(&expanded_head, &expanded_tail, var[j]);
							j++;
						}
						free(var);
					}
				}
				else if (str[i] == '~' && (i == 0) && (str[i + 1] == '/' || str[i + 1] == '\0'))
				{
					char *home = get_env_value("HOME", envp);
					if (home)
					{
						size_t j = 0;
						while (home[j])
						{
							append_char_node(&expanded_head, &expanded_tail, home[j]);
							j++;
						}
					}
					else
						append_char_node(&expanded_head, &expanded_tail, '~');
					i++;
				}
				else
				{
					append_char_node(&expanded_head, &expanded_tail, str[i]);
                    i++;
				}
			}
			curr_segment = curr_segment->next;
		}

        char *expanded_str = convert_char_list_to_string(expanded_head);
		free_char_list(expanded_head);

        if (!expanded_str || expanded_str[0] == '\0')
        {
            free(curr->str);
            curr->str = NULL;
            t_split *to_remove = curr;
            // curr = remove_token(head, to_remove);
			curr = curr->next;
			remove_token(head, to_remove);
            free(expanded_str);
            continue;
        }
		// if expansion introduces whitespace, split into multiple tokens
		int token_unquoted = is_token_unquoted(curr);
		if (token_unquoted)
		{
			handle_field_splitting(head, &curr, expanded_str);
			// printf("After splitting, current token is: %s\n", curr ? curr->str : "NULL");
			if(expanded_str)
				free(expanded_str);
			if (curr)
				curr = curr->next;
		}
		else
		{
			free(curr->str);
			curr->str = expanded_str;
			curr = curr->next;
		}
	}
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
        t_split *new_node = create_new_token(fields[i], WORD);
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
		var_len++; // is this the right way to stop? i know it stops before $.. i'm not sure if it's foolproof (could check Imad's)
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

	char *value = get_env_value(var_name, envp);
	// char *value = getenv(var_name);
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

t_split *create_new_token(char *str, t_type type)
{
	t_split *new_node = malloc(sizeof(t_split));
	if (!new_node)
	{
		perror("malloc");
		exit(1);
	}
	new_node->str = ft_strdup(str);
	new_node->type = type;
	new_node->prev = NULL;
	new_node->next = NULL;
	return new_node;
}

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
    return NULL;
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
		if (token[i] == '$' && quote_state != SQ)
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
