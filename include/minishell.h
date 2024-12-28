#ifndef MINISHELL_H
#define MINISHELL_H
#define MAX_ARGS 100

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

typedef enum {
    CMD_NONE,
    CMD_SIMPLE,
    CMD_PIPE,
    CMD_REDIRECT_IN,
    CMD_REDIRECT_OUT,
    // Add more as needed
} command_type_t;

typedef struct s_command {
    command_type_t type;
    char **argv;
	int argc;
    char *infile;    // for <
    char *outfile; // for > or >>
	int append; // 0 if >, 1 if >>
	struct s_command *next;     // Next command in a pipeline
	// possibly more fields for heredoc, etc.
} command_t;


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

#endif // MINISHELL_H
