

#include "../include/minishell.h"

Token *create_token(char *value, TokenType type)
{
	Token *token = malloc(sizeof(Token));
	if (!token)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	token->value = value;
	token->type = type;
	token->next = NULL;
	return token;
}

TokenType classify_token(const char *str)
{
	if (strcmp(str, "|") == 0)
		return PIPE;
	if (strcmp(str, "<") == 0)
		return REDIRECT_IN;
	if (strcmp(str, ">") == 0)
		return REDIRECT_OUT;
	if (strcmp(str, ">>") == 0)
		return APPEND;
	if (strcmp(str, "<<") == 0)
		return HEREDOC;
	if (strcmp(str, ";") == 0)
		return END;
	return WORD;
}

Token *tokenize(const char *input)
{
	Token *head = NULL;
	Token *current = NULL;

	size_t i = 0;

	char **split = ft_split(input, ' ');
	if (!split)
		return NULL;
	while (split[i])
	{
		TokenType type = classify_token(split[i]);
		if (type == WORD)
		{
			char *dup_value = strdup(split[i]);
			if (!head)
			{
				head = create_token(dup_value, type);
				current = head;
			}
			else
			{
				current->next = create_token(dup_value, type);
				current = current->next;
			}
		}
		else
		{
			current->next = create_token(NULL, type); 
			current = current->next;
		}
		i++;
	}
	for (size_t j = 0; split[j] != NULL; j++)
	{
    	printf("Split[%zu]: %s\n", j, split[j]);
	}

	free_split(split);
	return head;
}