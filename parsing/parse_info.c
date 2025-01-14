/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_info.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknsteja <mknsteja@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 13:11:31 by mknsteja          #+#    #+#             */
/*   Updated: 2025/01/08 12:20:33 by mknsteja         ###   ########.fr       */
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
		if (string[i] == '\"' || string[i] == '\'')
			append_list(input, handle_delimiter(string, string[i], &i));
		else if (ft_isprint(string[i]))
			append_list(input, handle_delimiter(string, ' ', &i));
    if(string[i])
		  i++;
	}
	tokenise(input);
	return (input);
}

char	*handle_delimiter(char *string, char c, int(*i))
{
	char			*result;
	unsigned int	start;

	result = NULL;
	start = (unsigned int)(*i);
	while (string[*i] && string[*i] != '\"' && string[*i] != '\''
		&& string[*i] != ' ')
		(*i)++;
	if (string[*i] == '\"' || string[*i] == '\'')
	{
		c = string[(*i)++];
		while (string[*i] && string[*i] != c)
			(*i)++;
    if(string[*(i)])
		  (*i)++;
	}
	result = ft_substr(string, start, (unsigned int)(*i) - start);
	return (result);
}

void	append_list(t_split *input, char *string)
{
	t_split	*new;
	t_split	*ptr;

	new = malloc(sizeof(t_split));
	if (!new)
		exit(-1);
	ptr = input;
	new->str = string;
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
