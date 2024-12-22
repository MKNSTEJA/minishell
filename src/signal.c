
#include "../include/minishell.h"

// Reset readline prompt on SIGINT
void signal_reset_prompt(int signo)
{
    (void)signo; // Unused parameter
    write(1, "\n", 1); // Write a newline
    rl_on_new_line();   // Prepare readline for a new input line
    rl_replace_line("", 0); // Clear the current line
    rl_redisplay();     // Redisplay the prompt
}

// Ignore SIGQUIT
void ignore_sigquit(void)
{
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = SIG_IGN; // Ignore the signal
    sigaction(SIGQUIT, &act, NULL);
}

// Set interactive signal handlers
void set_signals_interactive(void)
{
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = signal_reset_prompt; // Set custom handler for SIGINT
    sigaction(SIGINT, &act, NULL);        // Apply handler for SIGINT
    ignore_sigquit();                     // Ignore SIGQUIT
}
