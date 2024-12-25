#include "../include/minishell.h"


command_t *mock_simple_command(void)
{
    command_t *cmd = malloc(sizeof(command_t));
    if (!cmd)
        return NULL;

    cmd->type = CMD_SIMPLE;
    cmd->argv = malloc(sizeof(char*) * 3);
    cmd->argv[0] = strdup("ls");
    cmd->argv[1] = strdup("-la");
    cmd->argv[2] = NULL;
    cmd->input_redirection = NULL;
    cmd->output_redirection = NULL;
    cmd->next = NULL;

    return cmd;
}


command_t *mock_pipeline(void)
{
	// first command
	command_t *cmd1 = malloc(sizeof(command_t));
	cmd1->type = CMD_PIPE;
	cmd1->argv = malloc(sizeof(char*) * 2);
	cmd1->argv[0] = strdup("cat");
	cmd1->argv[1] = NULL;
	cmd1->input_redirection = NULL;
	cmd1->output_redirection = NULL;

	// second command
	command_t *cmd2 = malloc(sizeof(command_t));
	cmd2->type = CMD_SIMPLE;  // or CMD_PIPE if chaining multiple
	cmd2->argv = malloc(sizeof(char*) * 2);
	cmd2->argv[0] = strdup("wc");
	cmd2->argv[1] = NULL;
	cmd2->input_redirection = NULL;
	cmd2->output_redirection = NULL;

	// link them
	cmd1->next = cmd2;
	cmd2->next = NULL;

	return cmd1;
}