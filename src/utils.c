
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

int is_numeric(const char *s)
{
	if (!s || !*s)
		return 0;
	int i = 0;
	if (s[i] == '-' || s[i] == '+')
		i++;
	while (s[i])
	{
		if (!isdigit(s[i]))
			return 0;
		i++;
	}
	return 1;
}

void print_error_msg(const char *cmd, const char *arg, const char *err_msg)
{
    write(2, "minishell: ", 11);
    write(2, cmd, ft_strlen(cmd));
    write(2, ": ", 2);
    if (arg)
    {
        write(2, arg, ft_strlen(arg));
        write(2, ": ", 2);
    }
    write(2, err_msg, ft_strlen(err_msg));
    write(2, "\n", 1);
}


// int print_command_error(char *command, char *detail, char *error_message, int error_nb)
// {
//     char *msg;
//     char *temp;

//     msg = ft_strjoin("minishell: ", command);
//     if (command)
//     {
//         temp = ft_strjoin(msg, ": ");
//         free(msg);
//         msg = temp;
//     }
//     if (detail)
//     {
//         temp = ft_strjoin(msg, detail);
//         free(msg);
//         msg = temp;

//         temp = ft_strjoin(msg, ": ");
//         free(msg);
//         msg = temp;
//     }

//     temp = ft_strjoin(msg, error_message);
//     free(msg);
//     msg = temp;

//     ft_putendl_fd(msg, STDERR_FILENO);
//     free(msg);

//     return error_nb;
// }


