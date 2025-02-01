#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
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

typedef struct s_data {
    char    **env;       
    int     last_exit; // last exit code
    // some more data
} t_data;

typedef struct s_char_node
{
	char				c;
	struct s_char_node	*next;
}						t_char_node;

typedef struct s_expand
{
	t_char_node *expanded_head;
	t_char_node *expanded_tail;
}						t_expand;

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
	QUOTE_SINGLE,
	QUOTE_DOUBLE
}						t_quote_state;

typedef struct s_segment
{
	char				*text;
	t_quote_state		quote_state;
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
}						t_redir;

typedef struct s_op
{
	char				**str;
	t_redir				*redirections;
	struct s_op			*next;
}						t_op;


int init_data(t_data *data, char **envp);
int						is_numeric(const char *s);
int						print_command_error(char *command, char *detail,
							char *error_message, int error_nb);
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
void					handle_cd(char **argv);
void					handle_unset(char **argv, char **envp);
void					handle_env(char **argv, t_data *data);
void					handle_pwd(char **argv);
void					handle_echo(char **argv);
void					handle_export(char **argv, t_data *data);
void					add_env_variable(const char *key, const char *value, char ***envp);
char					*create_env_string(const char *key, const char *value);
void					set_env_variable(const char *key, const char *value, char ***envp);
void					print_exported_environ(char **envp);
int						apply_redirections(t_op *cmd);
int						count_commands(t_op *cmd);
char					*get_env_value(const char *var_name, char **envp);
void					expand_tokens(t_split **head, char **envp);
char					*expand_one_token(char *token, char **envp,
							t_quote_state quote_state);
t_split					*remove_token(t_split **head, t_split *token);
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
char					*find_executable(char **argv);

#endif