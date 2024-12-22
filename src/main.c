#include "../include/minishell.h"

int main(void)
{
    char *input;

    // Set up signal handling
    set_signals_interactive();

    while (1)
    {
        input = readline("minishell> ");
        if (!input) // Handle EOF (Ctrl+D)
        {
            printf("exit\n");
            break;
        }
        if (*input)
            add_history(input); // Add non-empty input to history
        printf("You entered: %s\n", input);
        free(input);
    }
    return 0;
}
