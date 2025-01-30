#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include "../libft/libft.h"

extern int g_exit_code;


typedef struct s_char_node {
    char c;
    struct s_char_node *next;
} t_char_node;

/* Enum for token types */
typedef enum e_type
{
	NONE,
	WORD,
	PIPES,      
	IN,         // (for "<")
	OUT,        // (for ">")
	APPEND,     // (for ">>")
	HEREDOC     // (for "<<")
}	t_type;

typedef enum e_quote_state
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
} t_quote_state;

typedef struct s_segment {
    char *text;
    t_quote_state quote_state;
	struct s_segment *current_segment;
    struct s_segment *next;
} t_segment;


/*
 * This struct (t_split) is used to store each 'split' token along with its type,
 * forming a linked list (possibly doubly-linked if you want to navigate backward).
 */
/* Linked list for tokens */
typedef struct s_split
{
	char			*str;       // The actual token/string (e.g., "ls", ">", "file", etc.)
	t_type			type;       // The token type (WORD, IN, OUT, etc.)
	t_segment		*segments;
	struct s_split	*prev;      // Pointer to previous element (if using a doubly-linked list)
	struct s_split	*next;      // Pointer to the next element
	// t_quote_state	quote_state;
}	t_split;



typedef struct s_redir
{
	t_type type; // e.g., IN, OUT, APPEND, HEREDOC
	char *filename; // e.g., "file.txt", or the heredoc limiter
	struct s_redir *next;
}	t_redir;

typedef struct s_op
{
	char		**str;
	t_redir		*redirections;
	struct s_op	*next;
}	t_op;

int is_numeric(const char *s);
int print_command_error(char *command, char *detail, char *error_message, int error_nb);
void ignore_sigquit(void);
void signal_reset_prompt(int signo);
t_segment 	*create_segment(const char *text, t_quote_state state);
void 		append_segment(t_segment **head, t_segment *new_seg);
t_segment	*final_assign(char *string, int *i, t_segment **current_segment,
		t_segment **current_segments);
void		handle_segments(char *string, t_split **input);
t_segment		*handle_space(t_split **input, t_segment **current_segments, t_type *token, int *i);
t_segment	*handle_quotes(t_quote_state *quote_state, t_segment **current_segments, int *i, char *string, t_segment **current_segment);
void		handle_others(t_split **input, t_segment **current_segments, t_type *token, int *i, char *string);
t_segment	*handle_dollar(t_segment **current_segments, t_quote_state *quote_state, int *i);
void handle_exit(char **argv);
void handle_cd(char **argv);
void handle_unset(char **argv);
void handle_env(char **argv);
void handle_pwd(char **argv);
void handle_echo(char **argv);
void handle_export(char **argv);
void add_env_variable(const char *key, const char *value);
char *create_env_string(const char *key, const char *value);
void set_env_variable(const char *key, const char *value);
void print_exported_environ(void);
int apply_redirections(t_op *cmd);
int count_commands(t_op *cmd);
char *get_env_value(const char *var_name, char **envp);
void expand_tokens(t_split **head, char **envp);
char *expand_one_token(char *token, char **envp, t_quote_state quote_state);
t_split *remove_token(t_split **head, t_split *token);
void handle_field_splitting(t_split **head, t_split **curr_ptr, char *expanded_str);
char *expand_escape(const char *str);
void append_list(t_split **head, t_segment *segments, t_type type);
int is_builtin(t_op *cmd);
void execute_commands(t_op *cmd);
void execute_pipeline(t_op *cmd);
void execute_simple_command(t_op *cmd);
void execute_builtin(t_op *cmd);
void	print_cmd(t_op *cmd);
void	free_split(t_split *list);
void	print_split(t_split *input);
void	free_op(t_op *cmd);
char *find_executable(char **argv);


#endif