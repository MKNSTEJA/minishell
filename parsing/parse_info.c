/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_info.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 13:11:31 by mknsteja          #+#    #+#             */
/*   Updated: 2025/01/13 21:49:26 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


char	*handle_delimiter(char *string, char c, int *i);
void	tokenise(t_split *input);


/*
 * first_initialise():
 *    Creates the very first t_split node by looking at string[i] and deciding:
 *       - If it starts with quote => handle quoted token
 *       - Else if it starts with operator (|, <, >) => special token
 *       - Else => normal "word" token, read until space or operator
 */
t_split	*first_initialise(char *string, int *i)
{
	t_split	*input;

	
	// input->str = NULL;
	// input->prev = NULL;
	// input->type = NONE;
	while (string[(*i)] == ' ' || string[(*i)] == '\t')
		(*i)++;
	// if the line was all spaces, build a minimal node
	if (!string[(*i)])
	{
		input = calloc(1, sizeof(t_split));
		input->str = ft_strdup("");
		input->quote_state = QUOTE_NONE;
		return (input);
	}
	
	input = calloc(1, sizeof(t_split));
	if (!input)
		exit(-1);
	
	if (string[(*i)] == '\"' || string[(*i)] == '\'') // If we are at a quote, call the function with the quote as the delimiter
	{
		char quote = string[(*i)];
		input->str = handle_delimiter(string, string[(*i)], i);
		if (quote == '"')
			input->quote_state = QUOTE_DOUBLE;
		else
			input->quote_state = QUOTE_SINGLE;
	}
	else if (string[*i] == '|' || string[*i] == '<' || string[*i] == '>')
	{
		if ((string[*i] == '>' && string[*i + 1] == '>')
			|| (string[*i] == '<' && string[*i + 1] == '<'))
			{
				// double operator >> or <<
				input->str = ft_substr(string, *i, 2);
				*i += 2;
			}
			else
			{
				//single operator >, <, or |
				input->str = ft_substr(string, *i, 1);
				(*i)++;
			}
			input->quote_state = QUOTE_NONE;
	}
	else
	{
		// normal unquoted text
		input->str = handle_delimiter(string, ' ', i);
		input->quote_state = QUOTE_NONE;
	}
	return (input);
}

t_split	*split_inputs(char *string)
{
	int		i;
	t_split	*input;

	i = 0;
	if (!string || !*string)
		return NULL;

	input = first_initialise(string, &i);
	while (string[i])
	{
		//skip spaces
		while (string[i] == ' ' || string[i] == '\t')
			i++;
		if (!string[i]) // We might be at '\0' after skipping spaces
        	break;
		//quoted token?
		if (string[i] == '"' || string[i] == '\'')
        {
            char quote = string[i];
            if (quote == '"')
                append_list(input, handle_delimiter(string, string[i], &i), QUOTE_DOUBLE);
            else
                append_list(input, handle_delimiter(string, string[i], &i), QUOTE_SINGLE);
        }
		// operator token? (|, <, >, <<, >>)
		else if (string[i] == '|' || string[i] == '<' || string[i] == '>')
        {
            if ((string[i] == '>' && string[i + 1] == '>')
                || (string[i] == '<' && string[i + 1] == '<'))
            {
                // double operator >> or <<
                append_list(input, ft_substr(string, i, 2), QUOTE_NONE);
                i += 2;
            }
            else
            {
                // single operator
                append_list(input, ft_substr(string, i, 1), QUOTE_NONE);
                i++;
            }
        }
		else if (ft_isprint(string[i]))
			append_list(input, handle_delimiter(string, ' ', &i), QUOTE_NONE);
	}
	tokenise(input);
	return (input);
}

/*
 * handle_delimiter():
 *    If passed quote char (single or double), it reads until the matching quote.
 *    Otherwise (c == ' ' in typical usage), read until space, tab, or quote/operator.
 */
char *handle_delimiter(char *string, char c, int *i)
{
    char          *result;
    unsigned int  start;

    start = (unsigned int)(*i);
	
	// If 'c' is a real quote, handle the quoted substring
    if (c == '\"' || c == '\'')
    {
        // skip opening quote
        (*i)++;     // now we're inside the quotes
        while (string[*i] && string[*i] != c)
            (*i)++;
        // now we stopped on closing quote or '\0'
        // substring start..(*i)-1 is the real content
        result = ft_substr(string, start + 1, (*i) - (start + 1));
        if (string[*i]) // skip closing quote if present
            (*i)++;
    }
    else
    {
        // Unquoted: read until space or a quote
        while (string[*i]
            && string[*i] != ' ' && string[*i] != '\t'
            && string[*i] != '\"' && string[*i] != '\''
            && string[*i] != '|' && string[*i] != '>'
            && string[*i] != '<')
        {
            (*i)++;
        }
        // substring from start..(*i)-1
        result = ft_substr(string, start, (*i) - start);
    }
    return (result);
}

/*
 * append_list():
 *    Appends a newly-allocated t_split node at the end of 'input'.
 */
void	append_list(t_split *input, char *string, t_quote_state quote_state)
{
	t_split	*new;
	t_split	*ptr;

	new = calloc(1, sizeof(t_split));
	if (!new)
		exit(-1);
	ptr = input;
	new->str = string;
	new->quote_state = quote_state;

	 // Traverse to the last node
	while (ptr->next)
        ptr = ptr->next;
    ptr->next = new;
    new->prev = ptr;
}

void	tokenise(t_split *input)
{
	t_split	*ptr;
	int		strlen;

	ptr = input;
	strlen = 0;
	while (ptr)
	{
		strlen = ft_strlen(ptr->str);
		if (ft_strncmp(ptr->str, "|", strlen) == 0)
			ptr->type = PIPES;
		else if (ft_strncmp(ptr->str, "<", strlen) == 0)
			ptr->type = IN;
		else if (ft_strncmp(ptr->str, ">", strlen) == 0)
			ptr->type = OUT;
		else if (ft_strncmp(ptr->str, ">>", strlen) == 0)
			ptr->type = APPEND;
		else if (ft_strncmp(ptr->str, "<<", strlen) == 0)
			ptr->type = HEREDOC;
		else
			ptr->type = WORD;
		ptr = ptr->next;
	}
}
