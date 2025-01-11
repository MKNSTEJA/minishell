#include "../include/minishell.h"

// void squote_checker(t_op *ptr);

char	*find_env(char *string, char **envp);
char	*extract_envp(char *string, char **envp);
void	squote_checker(t_op *ptr);

// char	*find_env(char *string, char **envp)
// {
// 	int		len;
// 	int		i;
// 	int		j;
// 	char	*extract;
// 	char	*new;

// 	j = 0;
// 	len = ft_strlen(string);
// 	new = malloc(sizeof(char) * (len + 1));
// 	if (!new)
// 		exit(-1);
// 	extract = NULL;
// 	i = 0;
// 	while (string[i] != '$')
// 	{
// 		new[i] = string[i];
// 		i++;
// 	}
// 	len = len - i - 1;
// 	if (string[i + 1] == '{')
// 	{
// 		if (ft_strchr(&string[i + 1], '}'))
// 		{
// 			i += 2;
// 			len -= 2;
// 		}
// 		else
// 			return (NULL);
// 	}
// 	while (envp[j])
// 	{
// 		if (!ft_strncmp(envp[j], &string[i], len) && envp[j][len] == '=')
// 		{
// 			extract = envp[j] + len + 1;
// 			ft_strlcat(&new[i], extract, ft_strlen(extract));
// 			break ;
// 		}
// 		j++;
// 	}
// 	if (!extract)
// 	{
// 		free(new);
// 		new = NULL;
// 		return (NULL);
// 	}
// 	while (string[i] && ft_isalpha(string[i]))
// 		i++;
// 	while (string[i++])
// 		new[i] = string[i];
// 	new[i] = '\0';
// 	// while(*string)
// 	// {
// 	// 	while(string[j++] != '$')
// 	// 		new[j] = string[j];
// 	// 	while (envp[i])
// 	// 	{
// 	// 		if (!ft_strncmp(envp[i], string, len) && envp[i][len] == '=')
// 	// 		{
// 	// 			extract = envp[i] + len + 1;
// 	// 			ft_strlcat(&new[i], extract, ft_strlen(extract));
// 	// 			break ;
// 	// 		}
// 	// 		i++;
// 	// 	}
// 	// 	new[j] = string[i];
// 	// 	j++;
// 	// }
// 	return (new);
// }

void	expand_dollar(t_op *cmd, char **envp)
{
	t_op	*ptr;
	char	*string;
	int		i;
	char	*find;

	ptr = cmd;
	i = 0;
	string = NULL;
	while (ptr)
	{
		i = 0;
		squote_checker(ptr);
		while (ptr->str[i])
		{
			if (ft_strchr(ptr->str[i], '$') && ptr->str[i][1] != '\0'
				&& ptr->squote % 2 == 0)
			{
				string = find_env(&ptr->str[i][0], envp);
				if (string)
				{
					free(ptr->str[i]);
					ptr->str[i] = string;
				}
			}
			i++;
		}
		ptr = ptr->next;
	}
}

void	squote_checker(t_op *ptr)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (ptr->str[i])
	{
		j = 0;
		while (ptr->str[i][j] && ptr->str[i][j] != '$')
		{
			if (ptr->str[i][j] == '$')
				return ;
			if (ptr->str[i][j] == '\'')
				ptr->squote++;
			j++;
		}
		i++;
	}
}

char	*find_env(char *string, char **envp)
{
	char	*extract;
	int		left_len;
	int		garbage;
	int		right_len;
	int		len_ext;
	int		total;
	int		i;
	int		j;
	int		k;
	char	*new;

	right_len = 0;
	total = 0;
	i = 0;
	j = 0;
	k = 0;
	new = NULL;
	len_ext = 0;
	garbage = 1;
	extract = NULL;
	left_len = 0;
	while (string[left_len] != '$')
		left_len++;
	while (string[left_len + garbage] && (ft_isalnum(string[left_len + garbage])
			|| string[left_len + garbage] == '{' || string[left_len
			+ garbage] == '}'))
		garbage++;
	extract = extract_envp(&string[left_len + 1], envp);
	if (!extract)
		return (NULL);
	while (extract[len_ext])
		len_ext++;
	while (string[left_len + garbage + right_len])
		right_len++;
	total = left_len + len_ext + right_len;
	new = malloc(sizeof(char) * (total + 1));
	if (!new)
		exit(-1);
	while (i < left_len)
	{
		new[i] = string[i];
		i++;
	}
	while (j < len_ext)
	{
		new[i + j] = extract[j];
		j++;
	}
	while (k < right_len)
	{
		new[i + j + k] = string[left_len + garbage + k];
		k++;
	}
	new[total] = '\0';
	return (new);
}

char	*extract_envp(char *string, char **envp)
{
	int	len;
	int	i;

	len = 0;
	i = 0;
	if (string[0] == '{')
	{
		if (ft_strchr(string, '}'))
			string++;
		else
			return (NULL);
	}
	while (string[len] && ft_isalnum(string[len]))
		len++;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], string, len))
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}
