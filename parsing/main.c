/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknsteja <mknsteja@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 02:42:19 by mknsteja          #+#    #+#             */
/*   Updated: 2024/12/25 12:02:42 by mknsteja         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


void	free_list(t_split *list);
void print_list(t_split *input);

int main(void)
{
	t_split *input;

	input = NULL;
	while (1)
	{
		char *str = readline("\nMinishell> ");		
		input = split_inputs(str);
		print_list(input);
	}	
	free_list(input);
}

void	free_list(t_split *list)
{
	t_split	*current;
	t_split	*next_node;

	if (!list)
		return ;
	current = list;
	while (current)
	{
		next_node = current->next;
		free(current->str);
		free(current);
		current = next_node;
	}
	free(list);
}

void print_list(t_split *input)
{
	t_split *ptr;
	ptr = input;
	while(ptr)
	{
		printf("string = %s |-> token = %d\n", ptr->str, ptr->type);
		ptr = ptr->next;
	}
}