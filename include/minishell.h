#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

/* Enum for token types */
// typedef enum e_type
// {
// 	NONE,
// 	WORD,
// 	PIPES,      // Matches usage: ptr->type = PIPES; (instead of PIPE)
// 	IN,         // Matches usage: ptr->type = IN;    (for "<")
// 	OUT,        // Matches usage: ptr->type = OUT;   (for ">")
// 	APPEND,     // Matches usage: ptr->type = APPEND;(for ">>")
// 	HEREDOC     // Matches usage: ptr->type = HEREDOC;(for "<<")
// }	t_type;

typedef enum s_type
{
	NONE,
	PIPES,
	IN,
	OUT,
	APPEND,
	HEREDOC,
	WORD,
	END
}						t_type;

/*

	* This struct (t_split) is used to store each 'split' token along with its type,

	* forming a linked list (possibly doubly-linked if you want to navigate backward).
 */
/* Linked list for tokens */
typedef struct s_split
{
	char *str; // The actual token/string (e.g., "ls", ">", "file",etc.)
	t_type type;          // The token type (WORD, IN, OUT, etc.)
	struct s_split	*prev; 	// Pointer to previous element (if using a doubly-linked list)
	struct s_split *next; // Pointer to the next element
}						t_split;

typedef struct s_op
{
	char				**str;
	int					append;
	int					fd_in;
	int					fd_out;
	int					squote;
	struct s_op			*prev;
	struct s_op			*next;
}						t_op;

typedef struct s_env
{
	char				*extract;
	int					right_len;
	int					left_len;
	int					garbage;
	int					len_ext;
	int					total;
	char				*new;
}						t_env;

// Token *tokenize(const char *input);
// Token *create_token(char *value, TokenType type);
void					set_signals_interactive(void);
void					ignore_sigquit(void);
void					signal_reset_prompt(int signo);
// void free_split(char **split);
void					handle_exit(char **argv);
void					handle_cd(char **argv);
void					handle_unset(char **argv);
void					handle_env(char **argv);
void					handle_pwd(char **argv);
void					handle_echo(char **argv);
// int count_commands(command_t *cmd);
// command_t *mock_simple_command(void);
int						is_builtin(char **argv);
// void execute_commands(command_t *cmd);
// void execute_pipeline(command_t *cmd);
// void execute_simple_command(command_t *cmd);
void					execute_builtin(char **argv);
void					print_cmd(t_op *cmd);
void					free_split(t_split *list);
void					print_split(t_split *input);
void					free_op(t_op *cmd);

t_split					*split_inputs(char *string);
int						split_errors(t_split *input);
t_op					*initialise_cmd(t_split *input, char **envp);
void					expand_dollar(t_op *cmd, char **envp);

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

// typedef enum {
//     WORD,
//     PIPE,
//     REDIRECT_IN,
//     REDIRECT_OUT,
//     APPEND,
//     HEREDOC,
//     END,
//     UNKNOWN
// } TokenType;

// typedef struct s_token {
// linked list of tokens (should've been named token_ll)
//     char *value;
//     TokenType type;
//     struct s_token *next;
// } Token;

// typedef struct s_redirection {
//     TokenType type;           // e.g., REDIRECT_IN, REDIRECT_OUT, APPEND, HEREDOC
//     char *filename;           // Target file or delimiter for heredoc
//     struct s_redirection *next;
// } redirection_t;

// typedef struct s_command {
//     char *name; // not there
//     char **args;
//     redirection_t *redirections;
//     struct s_command *next;
// } command_t;

#endif
