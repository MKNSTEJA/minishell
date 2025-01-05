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

/*
 * This struct (t_split) is used to store each 'split' token along with its type,
 * forming a linked list (possibly doubly-linked if you want to navigate backward).
 */
/* Linked list for tokens */
typedef struct s_split
{
	char			*str;       // The actual token/string (e.g., "ls", ">", "file", etc.)
	t_type			type;       // The token type (WORD, IN, OUT, etc.)
	struct s_split	*prev;      // Pointer to previous element (if using a doubly-linked list)
	struct s_split	*next;      // Pointer to the next element
	t_quote_state	quote_state; // The current quote state
}	t_split;

typedef enum e_quote_state
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
} t_quote_state;

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


// Token *tokenize(const char *input);
// Token *create_token(char *value, TokenType type);
// void set_signals_interactive(void);
void ignore_sigquit(void);
void signal_reset_prompt(int signo);
void handle_exit(char **argv);
void handle_cd(char **argv);
void handle_unset(char **argv);
void handle_env(char **argv);
void handle_pwd(char **argv);
void handle_echo(char **argv);
int apply_redirections(t_op *cmd);
int count_commands(t_op *cmd);

// int count_commands(command_t *cmd);
// command_t *mock_simple_command(void);
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