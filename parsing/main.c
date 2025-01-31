/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 02:42:19 by mknsteja          #+#    #+#             */
/*   Updated: 2025/01/31 21:22:29 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int g_exit_code = 0;

void	print_cmd(t_op *cmd);
void	free_split(t_split *list);
void	print_split(t_split *input);
void	free_op(t_op *cmd);
t_split *split_inputs(char *string);
int      split_errors(t_split *input);
t_op    *initialise_cmd(t_split *input);
void free_segment(t_split *list);

int	main(int argc, char **argv, char **envp)
{
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
            str = readline("Minishell: ");
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
		input = split_inputs(str);
		// print_split(input);
		// printf("\n");
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
		// print_cmd(cmd);
		// printf("\n");
		execute_commands(cmd, envp);
		free_split(input);
		free_op(cmd);
		free(str);
	}
	free(str);
	rl_clear_history();
	// system("leaks minishell");
	return g_exit_code;
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
		free_segment(current);
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
void free_segment(t_split *list)
{
	t_segment *current_seg;
	t_segment *next_seg;
	current_seg = list->segments;
	next_seg = NULL;
	
	while(current_seg)
	{
		next_seg = current_seg->next;
		if(current_seg->text)
		{
			free(current_seg->text);
			current_seg->text = NULL;
		}
		if(current_seg)
			free(current_seg);
		current_seg = next_seg;
	}
}

void	print_split(t_split *input)
{
	t_split	*ptr;
	t_segment *ptrs;
	int i;
	
	ptr = input;
	ptrs = NULL;
	i = 0;
	while (ptr)
	{	
		ptrs = ptr->segments;
		printf("string = %s |-> token = %d\n", ptr->str, ptr->type);
		i = 0;
		while(ptrs)
		{
			printf("Seg%d = %s, quote = %d\n",i, ptrs->text, ptrs->quote_state);
			i++;
			ptrs = ptrs->next;
		}
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
  	i = 0;
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



