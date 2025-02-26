/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 22:11:37 by ykhattab          #+#    #+#             */
/*   Updated: 2025/02/27 00:32:16 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	init_pipe_state(t_pipe_state *state, int len)
{
	state->pipeline_length = len;
	state->index = 0;
	state->prev_fd = -1;
	state->last_pid = 0;
	state->pids = malloc(sizeof(pid_t) * len);
	if (!state->pids)
	{
		perror("malloc");
		exit(1);
	}
}

static void cleanup_heredoc_state(t_heredoc_state *state)
{
    if (state->heredoc_pipes)
    {
        for (int i = 0; i < state->heredoc_count * 2; i++)
            close(state->heredoc_pipes[i]);
        free(state->heredoc_pipes);
		free(state->cmd_indices);
    }
}


static void apply_heredoc_pipes(t_op *cmd, t_heredoc_state *state, int cmd_index)
{
    for (int i = 0; i < state->heredoc_count; i++)
    {
        if (state->cmd_indices[i] == cmd_index)
        {
            // Find the heredoc redirection in this command
            t_redir *redir = cmd->redirections;
            while (redir)
            {
                if (redir->type == HEREDOC)
                {
                    dup2(state->heredoc_pipes[i * 2], STDIN_FILENO);
                    close(state->heredoc_pipes[i * 2]);
                    break;  // Only apply the first heredoc for this command
                }
                redir = redir->next;
            }
        }
    }
}


static void	execute_child(t_op *current, t_data *data, int prev_fd,
		int pipe_fds[2], t_heredoc_state *heredoc_state, int cmd_index)
{
	char	*exec_path;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	
	setup_child_io(current, prev_fd, pipe_fds);
	if (heredoc_state->has_heredocs)
	{
		apply_heredoc_pipes(current, heredoc_state, cmd_index);
	}
	if (apply_redirections(current, data) < 0)
		_exit(1);
	if (is_builtin(current))
	{
		execute_builtin(current, data);
		_exit(0);
	}
	exec_path = find_executable(current->str, data->env);
	if (!exec_path)
	{
		print_error_msg(current->str[0], NULL, "command not found");
		_exit(127);
	}
	execve(exec_path, current->str, data->env);
	perror("execve");
	_exit(1);
}

static void	execute_parent(t_pipe_state *state, t_op *current, int pipe_fds[2],
		pid_t pid)
{
	state->pids[state->index] = pid;
	state->last_pid = pid;
	if (state->prev_fd != -1)
		close(state->prev_fd);
	if (current->next)
	{
		close(pipe_fds[1]);
		state->prev_fd = pipe_fds[0];
	}
	state->index++;
}

static void	process_command(t_op *current, t_data *data, t_pipe_state *state, t_heredoc_state *heredoc_state)
{
	int		pipe_fds[2];
	pid_t	pid;

	if (current->next && pipe(pipe_fds) == -1)
	{
		perror("pipe");
		free(state->pids);
		exit(1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(state->pids);
		exit(1);
	}
	if (pid == 0)
		execute_child(current, data, state->prev_fd, pipe_fds, heredoc_state, state->index);
	else
		execute_parent(state, current, pipe_fds, pid);
}

static void process_pipeline_heredocs(t_op *cmd, t_data *data, t_heredoc_state *state)
{
	t_op *current = cmd;
	int heredoc_index = 0;
	int cmd_index = 0;
	
	while (current)
	{
		t_redir *redir = current->redirections;
		while (redir)
		{
			if (redir->type == HEREDOC)
			{
				if (pipe(state->heredoc_pipes + heredoc_index * 2) < 0)
				{
					perror("pipe");
					free(state->cmd_indices);
					free(state->heredoc_pipes);
					exit(1);
				}
				process_heredoc_input(state->heredoc_pipes[heredoc_index * 2 + 1], redir, data);
				close(state->heredoc_pipes[heredoc_index * 2 + 1]);
				heredoc_index++;
			}
			redir = redir->next;
		}
		current = current->next;
		cmd_index++;
	}
}

static void init_heredoc_state(t_heredoc_state *state, t_op *cmd, int len)
{
	len = count_commands(cmd);
    state->heredoc_count = 0;
    state->has_heredocs = false;
    
    t_op *current = cmd;
    int cmd_idx = 0;
    while (current)
    {
        t_redir *redir = current->redirections;
        while (redir)
        {
            if (redir->type == HEREDOC)
                state->heredoc_count++;
            redir = redir->next;
        }
        current = current->next;
        cmd_idx++;
    }
    state->heredoc_pipes = malloc(sizeof(int) * 2 * state->heredoc_count);
    state->cmd_indices = malloc(sizeof(int) * state->heredoc_count);
    
    if (!state->heredoc_pipes || !state->cmd_indices)
    {
        perror("malloc");
        exit(1);
    }
    state->has_heredocs = (state->heredoc_count > 0);
    current = cmd;
    cmd_idx = 0;
    int heredoc_idx = 0;
    while (current)
    {
        t_redir *redir = current->redirections;
        while (redir)
        {
            if (redir->type == HEREDOC)
            {
                state->cmd_indices[heredoc_idx] = cmd_idx;
                heredoc_idx++;
            }
            redir = redir->next;
        }
        current = current->next;
        cmd_idx++;
    }
}

void	execute_pipeline(t_op *cmd, t_data *data)
{
	int				pipeline_length;
	t_op			*current;
	t_pipe_state	state;
	t_heredoc_state heredoc_state;

	pipeline_length = count_commands(cmd);
	current = cmd;
	while (current)
	{
		current->is_in_pipeline = true;
		current = current->next;
	}
	init_heredoc_state(&heredoc_state, cmd, pipeline_length);
	if (heredoc_state.has_heredocs)
		process_pipeline_heredocs(cmd, data, &heredoc_state);
	current = cmd;
	init_pipe_state(&state, pipeline_length);
	while (current)
	{
		process_command(current, data, &state, &heredoc_state);
		current = current->next;
	}
	cleanup_heredoc_state(&heredoc_state);
	wait_for_children(&state, data);
	free(state.pids);
}
