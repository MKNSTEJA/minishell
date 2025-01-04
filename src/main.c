// #include "../include/minishell.h"

// void execute_commands(t_op *cmd)
// {
//     // If we see multiple commands, we assume pipeline
//     if (cmd && cmd->next != NULL)
//         execute_pipeline(cmd);
//     else
//         execute_simple_command(cmd);
// }

// // int main(void)
// // {
// //     // Instead of real input, let's just test the execution code directly:
// //     command_t *cmd = NULL;

// //     // Option 1: Test a simple command (e.g., "ls -la")
// //     cmd = mock_simple_command();
// //     // or Option 2: Test a pipeline (e.g., "cat | wc")
// //     // cmd = mock_pipeline();

// //     // Now just call your execute_functions
// //     execute_commands(cmd);

// //     // Possibly free the allocated command structures
// //     // e.g. free_command_list(cmd); // if you have a function for that
// //     // but for a quick test, you can skip it or write it later.

// //     return 0;
// // }

// int main(void)
// {
//     char *input;

//     // Set up signal handling
//     set_signals_interactive();

//     while (1)
//     {
//         input = readline("minishell> ");
//         if (!input) // Handle EOF (Ctrl+D)
//         {
//             printf("exit\n");
//             break;
//         }
//         if (*input)
//             add_history(input); // Add non-empty input to history
        
// 		Token *tokens = tokenize(input);
// 		printf("Tokens:\n");
//         for (Token *current = tokens; current != NULL; current = current->next)
//         {
//             if (current->value)
//                 printf("Type: %d, Value: %s\n", current->type, current->value);
//             else
//                 printf("Type: %d, Value: NULL\n", current->type);
//         }
//         free(input);
//     }
//     return 0;
// }
