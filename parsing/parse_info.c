/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_info.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknsteja <mknsteja@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 13:11:31 by mknsteja          #+#    #+#             */
/*   Updated: 2025/01/26 07:03:11 by mknsteja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


char	*handle_delimiter(char *string, char c, int *i);
// void	tokenise(t_split *input);
t_segment *create_segment(const char *text, t_quote_state state);
void append_segment(t_segment **head, t_segment *new_seg);


t_split	*split_inputs(char *string)
{
	int i = 0;
    t_split *input = NULL;
    t_segment *current_segments = NULL;
    t_segment *current_segment = NULL;
    t_quote_state quote_state = QUOTE_NONE;
    t_type token_type = WORD;
	t_quote_state new_state = QUOTE_NONE;
	
	if (!string || !*string)
		return NULL;
	while (string[i])
	{
		if (quote_state == QUOTE_NONE && (string[i] == ' ' || string[i] == '\t'))
		{
			if (current_segments)
			{
				append_list(&input, current_segments, token_type);
				current_segments = NULL;
				current_segment = NULL;
				token_type = WORD;
			}
			i++;
			continue;
		}			
		if (string[i] == '"' || string[i] == '\'')
		{
			if (string[i] == '"')
				new_state = QUOTE_DOUBLE;
			else
				new_state = QUOTE_SINGLE;
			if (quote_state == QUOTE_NONE)
			{
				quote_state = new_state;
				current_segment = create_segment("", quote_state);
				append_segment(&current_segments, current_segment);
				i++;
				continue;
			}
			else if (quote_state == new_state)
			{
				quote_state = QUOTE_NONE;
				current_segment = NULL;
				i++;
				continue;
			}
			else
			{
				//will handle here later mismatched quotes
			}
		}
		if (quote_state == QUOTE_NONE && (string[i] == '|' || string[i] == '<' || string[i] == '>'))
		{
			if (current_segments)
			{
				append_list(&input, current_segments, token_type);
                current_segments = NULL;
                token_type = WORD;
			}
			if (string[i] == '|')
			{
				t_segment *op_seg = create_segment("|", QUOTE_NONE);
                append_list(&input, op_seg, PIPES);
                i++;
			}
			else if (string[i] == '<')
            {
                if (string[i + 1] == '<')
                {
                    t_segment *op_seg = create_segment("<<", QUOTE_NONE);
                    append_list(&input, op_seg, HEREDOC);
                    i += 2;
                }
                else
                {
                    t_segment *op_seg = create_segment("<", QUOTE_NONE);
                    append_list(&input, op_seg, IN);
                    i += 1;
                }
            }
			else if (string[i] == '>')
			{
				if (string[i + 1] == '>')
				{
					t_segment *op_seg = create_segment(">>", QUOTE_NONE);
                    append_list(&input, op_seg, APPEND);
                    i += 2;
				}
				else
				{
					t_segment *op_seg = create_segment(">", QUOTE_NONE);
                    append_list(&input, op_seg, OUT);
                    i += 1;
				}
			}
			continue;
		}
		// $"..." pattern
		if (quote_state == QUOTE_NONE && string[i] == '$' && string[i + 1] == '"')
        {
			// printf("Found $\"\n");
            quote_state = QUOTE_DOUBLE; 
            current_segment = create_segment("$\"", quote_state); 
            append_segment(&current_segments, current_segment);
            i += 2;  // Skip over the $" part
            continue;
        }
		if (!current_segment)
		{
			current_segment = create_segment("", QUOTE_NONE);
			append_segment(&current_segments, current_segment);
		}
		char temp_char[2] = { string[i], '\0' };
		char *updated_text = ft_strjoin(current_segment->text, temp_char);
		free(current_segment->text);
		current_segment->text = updated_text;

		i++;
	}
    if (current_segments)
        append_list(&input, current_segments, token_type);
    t_split *ptr = input;
    while (ptr)
    {
        size_t total_length = 0;
        t_segment *seg = ptr->segments;
        while (seg)
        {
            total_length += strlen(seg->text);
            seg = seg->next;
        }
        ptr->str = malloc(total_length + 1);
        if (!ptr->str)
        {
            perror("malloc");
            exit(1);
        }
        ptr->str[0] = '\0';
        seg = ptr->segments;
        while (seg)
        {
            strcat(ptr->str, seg->text);
            seg = seg->next;
        }
        ptr = ptr->next;
    }
    // tokenise(input);
    return input;
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

t_segment *create_segment(const char *text, t_quote_state state)
{
	t_segment *seg = malloc(sizeof(t_segment));
	if (!seg)
	{
		perror("malloc");
		exit(1);
	}
	seg->text = ft_strdup(text);
	seg->quote_state = state;
	seg->next = NULL;
	return seg;
}

void append_segment(t_segment **head, t_segment *new_seg)
{
	if (!*head)
	{
		*head = new_seg;
	}
	else
	{
		t_segment *current = *head;
		while (current->next)
			current = current->next;
		current->next = new_seg;
	}
}
/*
 * append_list():
 *    Appends a newly-allocated t_split node at the end of 'input'.
 */
void append_list(t_split **head, t_segment *segments, t_type type)
{
	t_split	*new_node = malloc(sizeof(t_split));
	t_split *current = *head;
	if (!new_node)
	{
		perror("malloc");
		exit(1);
	}
	new_node->str = NULL;
	new_node->segments = segments;
	new_node->type = type;
	new_node->prev = NULL;
	new_node->next = NULL;
	// what will we do with new_node->type?
	
	if (!*head)
		*head = new_node;
	else
	{
		while (current->next)
			current = current->next;
		current->next = new_node;
		new_node->prev = current;
	}
}

// void	tokenise(t_split *input)
// {
// 	t_split	*ptr;
// 	int		strlen;

// 	ptr = input;
// 	strlen = 0;
// 	while (ptr)
// 	{
// 		strlen = ft_strlen(ptr->str);
// 		if (ft_strncmp(ptr->str, "|", strlen) == 0)
// 			ptr->type = PIPES;
// 		else if (ft_strncmp(ptr->str, "<", strlen) == 0)
// 			ptr->type = IN;
// 		else if (ft_strncmp(ptr->str, ">", strlen) == 0)
// 			ptr->type = OUT;
// 		else if (ft_strncmp(ptr->str, ">>", strlen) == 0)
// 			ptr->type = APPEND;
// 		else if (ft_strncmp(ptr->str, "<<", strlen) == 0)
// 			ptr->type = HEREDOC;
// 		else
// 			ptr->type = WORD;
// 		ptr = ptr->next;
// 	}
// }
