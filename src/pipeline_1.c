/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 22:11:37 by ykhattab          #+#    #+#             */
/*   Updated: 2025/02/12 23:13:33 by ykhattab         ###   ########.fr       */
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

static void	execute_child(t_op *current, t_data *data, int prev_fd,
		int pipe_fds[2])
{
	char	*exec_path;

	signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
	if (apply_redirections(current, data->env) < 0)
		_exit(1);
	setup_child_io(current, prev_fd, pipe_fds);
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

static void	process_command(t_op *current, t_data *data, t_pipe_state *state)
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
		execute_child(current, data, state->prev_fd, pipe_fds);
	else
		execute_parent(state, current, pipe_fds, pid);
}

void	execute_pipeline(t_op *cmd, t_data *data)
{
	int				pipeline_length;
	t_op			*current;
	t_pipe_state	state;

	pipeline_length = count_commands(cmd);
	current = cmd;
	init_pipe_state(&state, pipeline_length);
	while (current)
	{
		process_command(current, data, &state);
		current = current->next;
	}
	wait_for_children(&state);
	free(state.pids);
}
