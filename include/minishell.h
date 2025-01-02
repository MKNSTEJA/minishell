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


// typedef enum {
//     CMD_NONE,
//     CMD_SIMPLE,
//     CMD_PIPE,
//     CMD_REDIRECT_IN,
//     CMD_REDIRECT_OUT,
//     // Add more as needed
// } command_type_t;

// typedef struct s_command {
//     command_type_t type;
//     char **argv;
// 	int argc;
//     char *infile;    // for <
//     char *outfile; // for > or >>
// 	int append; // 0 if >, 1 if >>
// 	struct s_command *next;     // Next command in a pipeline
// 	// possibly more fields for heredoc, etc.
// } command_t;

typedef enum {
    WORD,
    PIPE,
    REDIRECT_IN,
    REDIRECT_OUT,
    APPEND,
    HEREDOC,
    END,
    UNKNOWN
} TokenType;

typedef struct s_token {
    char *value;
    TokenType type;
    struct s_token *next;
} Token;

typedef enum e_command_type {
    CMD_EXTERNAL,
    CMD_BUILTIN,
} command_type_t;

typedef struct s_redirection {
    TokenType type;           // e.g., REDIRECT_IN, REDIRECT_OUT, APPEND, HEREDOC
    char *filename;           // Target file or delimiter for heredoc
    struct s_redirection *next;
} redirection_t;

typedef struct s_command {
    command_type_t type;
    char *name;
    char **args;
    redirection_t *redirections;
    struct s_command *next;
} command_t;

Token *tokenize(const char *input);
Token *create_token(char *value, TokenType type);
void set_signals_interactive(void);
void ignore_sigquit(void);
void signal_reset_prompt(int signo);
void free_split(char **split);
void handle_exit(char **argv);
void handle_cd(char **argv);
void handle_unset(char **argv);
void handle_env(char **argv);
void handle_pwd(char **argv);
void handle_echo(char **argv);
int count_commands(command_t *cmd);
command_t *mock_simple_command(void);
int is_builtin(char **argv);
void execute_commands(command_t *cmd);
void execute_pipeline(command_t *cmd);
void execute_simple_command(command_t *cmd);
void execute_builtin(char **argv);
void	print_cmd(t_op *cmd);
void	free_split(t_split *list);
void	print_split(t_split *input);
void	free_op(t_op *cmd);

#ifndef MINISHELL_STRUCTS_H
# define MINISHELL_STRUCTS_H

# include <stdlib.h>     // for NULL, malloc
# include <unistd.h>     // for STDIN_FILENO, STDOUT_FILENO

/*
 * This enum is used to mark the type of each token/string in the input.
 * Adjust the names/values based on your usage in tokenise() and elsewhere.
 */
typedef enum e_type
{
	NONE,
	WORD,
	PIPES,      // Matches usage: ptr->type = PIPES; (instead of PIPE)
	IN,         // Matches usage: ptr->type = IN;    (for "<")
	OUT,        // Matches usage: ptr->type = OUT;   (for ">")
	APPEND,     // Matches usage: ptr->type = APPEND;(for ">>")
	HEREDOC     // Matches usage: ptr->type = HEREDOC;(for "<<")
}	t_type;

/*
 * This struct (t_split) is used to store each 'split' token along with its type,
 * forming a linked list (possibly doubly-linked if you want to navigate backward).
 */
typedef struct s_split
{
	char			*str;       // The actual token/string (e.g., "ls", ">", "file", etc.)
	t_type			type;       // The token type (WORD, IN, OUT, etc.)
	struct s_split	*prev;      // Pointer to previous element (if using a doubly-linked list)
	struct s_split	*next;      // Pointer to the next element
}	t_split;

/*
 * This struct (t_op) is used to represent an entire command in your shell.
 *    - str:     An array of strings (the command + its arguments).
 *    - append:  A flag to indicate if redirection is >> (append) or > (truncate).
 *    - fd_in:   File descriptor for input redirection.
 *    - fd_out:  File descriptor for output redirection.
 *    - next:    Pointer to the next command in a pipeline or list.
 */
typedef struct s_op
{
	char		**str;
	int			append;
	int			fd_in;
	int			fd_out;
	struct s_op	*next;
}	t_op;

#endif


#endif // MINISHELL_H
