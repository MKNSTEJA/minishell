/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 22:13:57 by ykhattab          #+#    #+#             */
/*   Updated: 2025/02/08 23:59:02 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	handle_input_redirection(t_redir *redir)
{
	int	fd_in;

	fd_in = open(redir->filename, O_RDONLY);
	if (fd_in < 0)
	{
		print_error_msg("Minishell", redir->filename, strerror(errno));
		g_exit_code = 1;
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
		print_error_msg("Minishell", redir->filename, strerror(errno));
		g_exit_code = 1;
		return (-1);
	}
	dup2(fd_out, STDOUT_FILENO);
	close(fd_out);
	return (0);
}

static int	handle_append_redirection(t_redir *redir)
{
	int	fd_out;

	fd_out = open(redir->filename,
					O_WRONLY | O_CREAT | O_APPEND,
					0644);
	if (fd_out < 0)
	{
		print_error_msg("Minishell", redir->filename, strerror(errno));
		g_exit_code = 1;
		return (-1);
	}
	dup2(fd_out, STDOUT_FILENO);
	close(fd_out);
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
		else if (redir->type == HEREDOC && handle_heredoc_redirection(redir,
					envp) < 0)
			return (-1);
		redir = redir->next;
	}
	return (0);
}
