
#include "../include/minishell.h"

void free_split(char **split)
{
    int i = 0;
	while (split[i])
		free(split[i++]);
    free(split);
}


int count_commands(command_t *cmd)
{
    int count = 0;
    while (cmd)
    {
        count++;
        cmd = cmd->next;
    }
    return count;
}

int is_builtin(char **argv)
{
	if (!argv || !argv[0])
        return 0;
    return (strcmp(argv[0], "exit") == 0 ||
            strcmp(argv[0], "cd") == 0 ||
            strcmp(argv[0], "export") == 0 ||
            strcmp(argv[0], "unset") == 0 ||
            strcmp(argv[0], "env") == 0 ||
            strcmp(argv[0], "pwd") == 0 ||
            strcmp(argv[0], "echo") == 0);
}