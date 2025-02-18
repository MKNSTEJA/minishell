/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/25 02:42:19 by mknsteja          #+#    #+#             */
/*   Updated: 2025/02/17 15:52:31 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"



void	increment_shlvl(char ***envp);

int	main(int argc, char **argv, char **envp)
{
	t_split	*input;
	t_op	*cmd;
	char	*str;
	t_data	data;
	char	*line;

	(void)argc;
	(void)argv;
	input = NULL;
	cmd = NULL;
	str = NULL;
	if (!init_data(&data, envp))
		return (EXIT_FAILURE);
	remove_env_variable(&data.env, "OLDPWD");
	set_signals_interactive();
	while (1)
	{

		if (isatty(fileno(stdin)))
		{
			g_waiting_for_input = 1;
			str = readline("Minishell: ");
			g_waiting_for_input = 0;
		}
		else
		{
			line = get_next_line(fileno(stdin));
			if (!line)
				break ;
			str = ft_strtrim(line, "\n");
			free(line);
		}
		// str = readline("\nMinishell: ");
		if (!str)
			break ;
		add_history(str);
		input = split_inputs(str);
		// print_split(input);
		if (split_errors(input, &data) == 1)
		{
			free_split(input);
			input = NULL;
			free(str);
			continue ;
		}
		expand_tokens(&input, &data);
		cmd = initialise_cmd(input);
		execute_commands(cmd, &data);
		free_split(input);
		free_op(cmd);
		free(str);
	}
	rl_clear_history();
	free_data(&data);
	// system("leaks minishell");
	return (data.last_exit);
}

int	init_data(t_data *data, char **envp)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (envp[i])
		i++;
	data->env = malloc(sizeof(char *) * (i + 1));
	if (!data->env)
		return (0);
	while (j < i)
	{
		data->env[j] = ft_strdup(envp[j]);
		if (!data->env[j])
		{
			while (--j >= 0)
				free(data->env[j]);
			free(data->env);
			return (0);
		}
		j++;
	}
	data->env[i] = NULL;
	data->last_exit = 0;
	increment_shlvl(&data->env);
	return (1);
}

void	increment_shlvl(char ***envp)
{
	char	*shlvl_str;
	int		shlvl;
	char	*new_shlvl;

	shlvl_str = my_getenv("SHLVL", *envp);
	shlvl = ft_atoi(shlvl_str);
	shlvl++;
	new_shlvl = ft_itoa(shlvl);
	if (!new_shlvl)
		return ;
	set_env_variable("SHLVL", new_shlvl, envp);
	free(new_shlvl);
}

// void	print_split(t_split *input)
// {
// 	t_split		*ptr;
// 	t_segment	*ptrs;
// 	int			i;

// 	ptr = input;
// 	ptrs = NULL;
// 	i = 0;
// 	while (ptr)
// 	{
// 		ptrs = ptr->segments;
// 		printf("string = %s |-> token = %d\n", ptr->str, ptr->type);
// 		i = 0;
// 		while (ptrs)
// 		{
// 			printf("Seg%d = %s, quote = %d\n", i, ptrs->text, ptrs->quote);
// 			i++;
// 			ptrs = ptrs->next;
// 		}
// 		ptr = ptr->next;
// 	}
// }

void	print_cmd(t_op *cmd)
{
	t_op	*ptr;
	int		i;
	int		counter;

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
