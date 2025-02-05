/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmummadi <kmummadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 02:42:19 by mknsteja          #+#    #+#             */
/*   Updated: 2025/02/05 21:15:24 by kmummadi         ###   ########.fr       */
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
	t_data	data;
	(void)argc; // this will be used later, so if norminette marked it as an additional parameter, ignore it
    (void)argv;

	input = NULL;
	cmd = NULL;
	str = NULL;
	if (!init_data(&data, envp))
		return EXIT_FAILURE;
	remove_env_variable(&data.env, "OLDPWD");
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
		expand_tokens(&input, data.env);
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
		execute_commands(cmd, &data);
		free_split(input);
		free_op(cmd);
		free(str);
	}
	rl_clear_history();
	free_data(&data);
	// system("leaks minishell");
	return g_exit_code;
}

int init_data(t_data *data, char **envp)
{
    int i = 0;
    while (envp[i])
        i++;

    data->env = malloc(sizeof(char *) * (i + 1));
    if (!data->env)
        {
			return 0;
		}
		
	int j = 0;
    while (j < i)
    {
        data->env[j] = ft_strdup(envp[j]);
        if (!data->env[j])
        {
            while (--j >= 0)
                free(data->env[j]);
            free(data->env);
            return 0;
        }
		j++;
    }
    data->env[i] = NULL;
    return 1;
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
			printf("Seg%d = %s, quote = %d\n",i, ptrs->text, ptrs->quote);
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
