/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmummadi <kmummadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 00:03:26 by ykhattab          #+#    #+#             */
/*   Updated: 2025/02/09 16:09:52 by kmummadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>
# include <readline/history.h>
# include <readline/readline.h>

extern int				g_exit_code;

typedef struct s_data
{
	char				**env;
	int					last_exit;
}						t_data;

typedef struct s_char_node
{
	char				c;
	struct s_char_node	*next;
}						t_char_node;

typedef struct s_pipe_state
{
	pid_t				*pids;
	int					index;
	int					prev_fd;
	pid_t				last_pid;
	int					pipeline_length;
}						t_pipe_state;

typedef enum e_type
{
	NONE,
	WORD,
	PIPES,
	IN,
	OUT,
	APPEND,
	HEREDOC
}						t_type;

typedef enum e_quote_state
{
	QUOTE_NONE,
	SQ,
	DQ
}						t_quote_state;

typedef struct s_segment
{
	char				*text;
	t_quote_state		quote;
	struct s_segment	*current_segment;
	struct s_segment	*next;
}						t_segment;

typedef struct s_parts
{
	char				*string;
	int					i;
	t_quote_state		quote;
	t_type				token;
	t_segment			*current_segment;
}						t_parts;

typedef struct s_split
{
	char				*str;
	t_type				type;
	t_segment			*segments;
	struct s_split		*prev;
	struct s_split		*next;

}						t_split;

typedef struct s_redir
{
	t_type				type;
	char				*filename;
	struct s_redir		*next;
	int					quoted;
}						t_redir;

typedef struct s_op
{
	char				**str;
	t_redir				*redirections;
	struct s_op			*next;
}						t_op;

typedef struct s_expand
{
	t_char_node			*expanded_head;
	t_char_node			*expanded_tail;
	t_split				*split;
	t_split				*to_remove;
	t_segment			*seg;
	char				*expanded_str;
	int					token_unquoted;
}						t_expand;

t_split					*split_inputs(char *string);
int						split_errors(t_split *input);
t_op					*initialise_cmd(t_split *input);
void					write_to_pipe(int pipe_fd, char *line);
char					*expand_if_needed(char *line, t_redir *redir,
							char **envp);
void					process_heredoc_input(int write_fd, t_redir *redir,
							char **envp);
int						handle_heredoc_redirection(t_redir *redir, char **envp);
void					wait_for_children(t_pipe_state *state);
void					setup_child_io(t_op *current, int prev_fd,
							int pipe_fds[2]);
int						is_valid_identifier(const char *key);
int						is_n_flag(char *arg);
int						init_data(t_data *data, char **envp);
void					free_data(t_data *data);
void					free_split(t_split *list);
void					free_segment(t_split *list);
void					free_op(t_op *cmd);
int						free_expanded_str(t_expand *exp, t_split **head);
void					free_char_list(t_char_node *head);
void					print_error_msg(const char *cmd, const char *arg,
							const char *err_msg);
int						is_numeric(const char *s);
void					ignore_sigquit(void);
void					signal_reset_prompt(int signo);
t_segment				*create_segment(const char *text, t_quote_state state);
void					append_segment(t_segment **head, t_segment *new_seg);
void					final_assign(t_parts **parts,
							t_segment **current_segments, int *i);
void					handle_segments(char *string, t_split **input);
void					handle_space(t_split **input,
							t_segment **current_segments, t_parts **parts,
							int *i);
void					handle_quotes(t_segment **current_segments,
							t_parts **parts, int *i);
void					handle_others(t_split **input,
							t_segment **current_segments, t_parts **parts,
							int *i);
void					handle_dollar(t_segment **current_segments,
							t_parts **parts, int *i);
void					handle_exit(char **argv);
void					handle_cd(char **argv, char ***envp);
char					*my_getenv(const char *name, char **env);
void					handle_unset(char **argv, char **envp);
void					handle_env(char **argv, t_data *data);
void					handle_pwd(char **argv, char **envp);
void					handle_echo(char **argv);
void					handle_export(char **argv, t_data *data);
void					add_env_variable(const char *key, const char *value,
							char ***envp);
void					remove_env_variable(char ***envp, const char *var);
char					*create_env_string(const char *key, const char *value);
void					set_env_variable(const char *key, const char *value,
							char ***envp);
void					print_exported_environ(char **envp);
int						apply_redirections(t_op *cmd, char **envp);
int						count_commands(t_op *cmd);
void					expand_tokens(t_split **head, char **envp);
char					*expand_one_token(char *token, char **envp,
							t_quote_state quote_state);
void					handle_field_splitting(t_split **head,
							t_split **curr_ptr, char *expanded_str);
char					*expand_escape(const char *str);
void					append_list(t_split **head, t_segment *segments,
							t_type type);
int						is_builtin(t_op *cmd);
void					execute_commands(t_op *cmd, t_data *data);
void					execute_pipeline(t_op *cmd, t_data *data);
void					execute_simple_command(t_op *cmd, t_data *data);
void					execute_builtin(t_op *cmd, t_data *data);
void					print_cmd(t_op *cmd);
void					free_split(t_split *list);
void					print_split(t_split *input);
void					free_op(t_op *cmd);
char					*find_executable(char **argv, char **envp);
void					loop_string(char *str, t_expand *exp, char **envp,
							t_segment *curr_segment);
void					append_char_node(t_expand *exp, char c);
void					expand_double_quote(const char *str, char **envp,
							size_t *i, t_expand *exp);
void					expand_single_quote(const char *str, size_t *i,
							t_expand *exp);
char					*expand_var(const char *str, char **envp, size_t *i);
t_split					*create_new_token(char *str, t_type type);
int						is_token_unquoted(t_split *token);
t_split					*remove_token(t_split **head, t_split *token);
char					*expand_var(const char *str, char **envp, size_t *i);
char					*get_env_value(const char *var_name, char **envp);
void					add_redirection(t_op *cmd, t_type type, char *filename);
void					append_str(t_op *cmd, char *string);
void					append_cmd(t_op *cmd, char *string);
void					split_cmds(t_split *input, t_op *cmd);

#endif