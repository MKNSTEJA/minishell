/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 20:58:04 by ykhattab          #+#    #+#             */
/*   Updated: 2024/10/11 20:58:08 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static int	count_words(const char *str, char delim)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*str)
	{
		if (*str != delim && in_word == 0)
		{
			in_word = 1;
			count++;
		}
		else if (*str == delim)
			in_word = 0;
		str++;
	}
	return (count);
}

static char	*allocate_word(const char *start, int length)
{
	char	*word;
	int		i;

	word = (char *)malloc((length + 1) * sizeof(char));
	if (!word)
		return (NULL);
	i = 0;
	while (i < length)
	{
		word[i] = start[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static int	process_word(const char **str_ptr, char delim, char **result,
		int *word_index)
{
	const char	*word_start;
	int			word_length;

	while (**str_ptr == delim)
		(*str_ptr)++;
	if (!**str_ptr)
		return (0);
	word_start = *str_ptr;
	word_length = 0;
	while (**str_ptr && **str_ptr != delim)
	{
		(*str_ptr)++;
		word_length++;
	}
	result[*word_index] = allocate_word(word_start, word_length);
	if (!result[*word_index])
		return (-1);
	(*word_index)++;
	return (1);
}

static int	fill_result(const char *str, char delim, char **result)
{
	int	word_index;
	int	ret;

	word_index = 0;
	while (*str)
	{
		ret = process_word(&str, delim, result, &word_index);
		if (ret == -1)
		{
			while (word_index >= 0)
			{
				free(result[word_index]);
				word_index--;
			}
			free(result);
			return (0);
		}
		else if (ret == 0)
			break ;
	}
	result[word_index] = NULL;
	return (1);
}

char	**ft_split(const char *str, char delim)
{
	char	**result;
	int		word_count;

	if (!str)
		return (NULL);
	word_count = count_words(str, delim);
	result = (char **)malloc((word_count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	if (!fill_result(str, delim, result))
		return (NULL);
	return (result);
}
