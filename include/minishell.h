#ifndef MINISHELL_H
#define MINISHELL_H
#define MAX_ARGS 100

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h> 
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
    char **argv;                // Arguments for the command
    char *input_redirection;    // File for input redirection
    char *output_redirection;   // File for output redirection
    struct s_command *next;     // Next command in a pipeline
} command_t;


void set_signals_interactive(void);
void ignore_sigquit(void);
void signal_reset_prompt(int signo);

#endif // MINISHELL_H
