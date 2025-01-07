/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_info.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yousef <yousef@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 13:11:31 by mknsteja          #+#    #+#             */
/*   Updated: 2025/01/07 06:10:27 by yousef           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <string.h>

char	*handle_delimiter(char *string, char c, int(*i));
void	append_list(t_split *input, char *string);
void	tokenise(t_split *input);

t_split	*first_initialise(char *string, int(*i))
{
	t_split	*input;

	(void)i;
	input = malloc(sizeof(t_split));
	if (!input)
		exit(-1);
	input->str = NULL;
	input->prev = NULL;
	input->type = NONE;
	while (string[(*i)] == ' ' || string[(*i)] == '\t')
		(*i)++;
	if (string[(*i)] == '\"' || string[(*i)] == '\'')
		input->str = handle_delimiter(string, string[(*i)], i);
	else if (ft_isprint(string[(*i)]) == 1)
		input->str = handle_delimiter(string, ' ', i);
	input->next = NULL;
	return (input);
}

t_split	*split_inputs(char *string)
{
	int		i;
	t_split	*input;

	i = 0;
	input = first_initialise(string, &i);
	while (string[i])
	{
		while (string[i] == ' ' || string[i] == '\t')
			i++;
		if (!string[i]) // We might be at '\0' after skipping spaces
        break;
		if (string[i] == '\"' || string[i] == '\'')
			append_list(input, handle_delimiter(string, string[i], &i));
		else if (ft_isprint(string[i]))
			append_list(input, handle_delimiter(string, ' ', &i));
    // Now see if we ended on a valid character or at the end
    	if (!string[i]) 
			{ // If we are at end of string, no more tokens
        	break;
			}
		i++;
	}
	tokenise(input);
	return (input);
}

char *handle_delimiter(char *string, char c, int *i)
{
    char          *result;
    unsigned int  start;

    // If c is the quote character passed in (e.g. '"' or '\''),
    // skip the opening quote itself so `start` points just after it:
    start = (unsigned int)(*i);
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
            && string[*i] != ' '
            && string[*i] != '\t'
            && string[*i] != '\"'
            && string[*i] != '\'')
        {
            (*i)++;
        }
        // substring from start..(*i)-1
        result = ft_substr(string, start, (*i) - start);
    }
    return (result);
}


// char	*handle_delimiter(char *string, char c, int(*i))
// {
// 	char			*result;
// 	unsigned int	start;

// 	result = NULL;
// 	start = (unsigned int)(*i);
// 	// if(c == ' ' || c == '\t')
// 	while (string[*i] && string[*i] != '\"' && string[*i] != '\''
// 		&& string[*i] != ' ')
// 		(*i)++;
// 	if (string[*i] == '\"' || string[*i] == '\'')
// 	{
// 		c = string[(*i)++];
// 		while (string[*i] && string[*i] != c)
// 			(*i)++;
//     if(string[*(i)])
// 		  (*i)++;
// 	}
// 	result = ft_substr(string, start, (unsigned int)(*i) - start);
// 	return (result);
// }

void	append_list(t_split *input, char *string)
{
	t_split	*new;
	t_split	*ptr;

	new = malloc(sizeof(t_split));
	if (!new)
		exit(-1);
	ptr = input;
	new->str = ft_strdup(string);
	if (!new->str)
		exit(-1);
	new->next = NULL;
	if (!input->next)
		input->next = new;
	else
	{
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = new;
	}
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
