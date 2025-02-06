/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 22:13:57 by ykhattab          #+#    #+#             */
/*   Updated: 2025/02/06 23:57:33 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	handle_input_redirection(t_redir *redir)
{
	int	fd_in;

	fd_in = open(redir->filename, O_RDONLY);
	if (fd_in < 0)
	{
		perror(redir->filename);
		return (-1);
	}
	dup2(fd_in, STDIN_FILENO);
	close(fd_in);
	return (0);
}

static int	handle_output_redirection(t_redir *redir)
{
	int	fd_out;

	fd_out = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_out < 0)
	{
		perror(redir->filename);
		return (-1);
	}
	dup2(fd_out, STDOUT_FILENO);
	close(fd_out);
	return (0);
}

static int	handle_append_redirection(t_redir *redir)
{
	int fd_out = open(redir->filename,
						O_WRONLY | O_CREAT | O_APPEND,
						0644);
	if (fd_out < 0)
	{
		perror(redir->filename);
		return (-1);
	}
	dup2(fd_out, STDOUT_FILENO);
	close(fd_out);
	return (0);
}

static void	write_to_pipe(int pipe_fd, char *line)
{
	write(pipe_fd, line, ft_strlen(line));
	write(pipe_fd, "\n", 1);
}

static char	*expand_if_needed(char *line, t_redir *redir, char **envp)
{
	char	*expanded_line;

	if (!redir->quoted)
	{
		expanded_line = expand_line(line, envp);
		free(line);
		if (!expanded_line)
			return (NULL);
		return (expanded_line);
	}
	return (line);
}

static void	process_heredoc_input(int write_fd, t_redir *redir, char **envp)
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
		line = expand_if_needed(line, redir, envp);
		write_to_pipe(write_fd, line);
		free(line);
	}
}

static int	handle_heredoc_redirection(t_redir *redir, char **envp)
{
	int		heredoc_pipe[2];

	if (pipe(heredoc_pipe) < 0)
		return (-1);
	process_heredoc_input(heredoc_pipe[1], redir, envp);
	close(heredoc_pipe[1]);
	dup2(heredoc_pipe[0], STDIN_FILENO);
	close(heredoc_pipe[0]);
	return (0);
}

int	apply_redirections(t_op *cmd, char **envp)
{
	t_redir	*redir;

	redir = cmd->redirections;
	while (redir)
	{
		if (redir->type == IN && handle_input_redirection(redir) < 0)
			return (-1);
		else if (redir->type == OUT && handle_output_redirection(redir) < 0)
			return (-1);
		else if (redir->type == APPEND && handle_append_redirection(redir) < 0)
			return (-1);
		else if (redir->type == HEREDOC && handle_heredoc_redirection(redir, envp) < 0)
			return (-1);
		redir = redir->next;
	}
	return (0);
}
