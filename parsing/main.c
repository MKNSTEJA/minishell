/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 02:42:19 by mknsteja          #+#    #+#             */
/*   Updated: 2025/01/04 06:36:12 by ykhattab         ###   ########.fr       */
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
