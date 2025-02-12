/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_redirections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 23:24:20 by ykhattab          #+#    #+#             */
/*   Updated: 2025/02/13 00:30:11 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	write_to_pipe(int pipe_fd, char *line)
{
	write(pipe_fd, line, ft_strlen(line));
	write(pipe_fd, "\n", 1);
}

char	*expand_if_needed(char *line, t_redir *redir, t_data *data)
{
	char	*expanded_line;
	if (redir->quoted <= 0)
	{
		expanded_line = expand_one_token(line, data, DQ);
		free(line);
		if (!expanded_line)
			return (NULL);
		return (expanded_line);
	}
	return (line);
}

void	process_heredoc_input(int write_fd, t_redir *redir, t_data *data)
{
	char	*line;
	size_t	len;

	line = NULL;
	while (1)
	{
		ft_putstr_fd("> ", 1);
		line = get_next_line(0);
		if (!line)
			break ;
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		if (ft_strcmp(line, redir->filename) == 0)
		{
			free(line);
			break ;
		}
		line = expand_if_needed(line, redir, data);
		write_to_pipe(write_fd, line);
		free(line);
	}
}

int	handle_heredoc_redirection(t_redir *redir, t_data *data)
{
	int		heredoc_pipe[2];

	if (pipe(heredoc_pipe) < 0)
		return (-1);
	process_heredoc_input(heredoc_pipe[1], redir, data);
	close(heredoc_pipe[1]);
	dup2(heredoc_pipe[0], STDIN_FILENO);
	close(heredoc_pipe[0]);
	return (0);
}
