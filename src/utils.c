
#include "../include/minishell.h"

void free_split(char **split)
{
    int i = 0;
	while (split[i])
		free(split[i++]);
    free(split);
}