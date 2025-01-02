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

#endif // MINISHELL_H
