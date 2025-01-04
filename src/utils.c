
#include "../include/minishell.h"

// void free_split(char **split)
// {
//     int i = 0;
// 	while (split[i])
// 		free(split[i++]);
//     free(split);
// }


int count_commands(t_op *cmd)
{
    int count = 0;
    while (cmd)
    {
        count++;
        cmd = cmd->next;
    }
    return count;
}

int is_builtin(t_op *cmd)
{
	if (!cmd || !cmd->str || !cmd->str[0])
        return 0;
    return (strcmp(cmd->str[0], "exit") == 0 ||
            strcmp(cmd->str[0], "cd") == 0 ||
            strcmp(cmd->str[0], "export") == 0 ||
            strcmp(cmd->str[0], "unset") == 0 ||
            strcmp(cmd->str[0], "env") == 0 ||
            strcmp(cmd->str[0], "pwd") == 0 ||
            strcmp(cmd->str[0], "echo") == 0);
}