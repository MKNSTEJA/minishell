/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 23:50:37 by yousef            #+#    #+#             */
/*   Updated: 2024/11/17 14:44:15 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*free_and_return_null(char *buffer, char *remaining)
{
	free(buffer);
	free(remaining);
	return (NULL);
}

char	*ft_read_to_remaining(int fd, char *remaining)
{
	char	*buffer;
	int		bytes_read;
	char	*temp;

	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	bytes_read = 1;
	while (!ft_strchr_gnl(remaining, '\n') && bytes_read != 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
			return (free_and_return_null(buffer, remaining));
		buffer[bytes_read] = '\0';
		temp = ft_strjoin_gnl(remaining, buffer);
		if (!temp)
			return (free_and_return_null(buffer, remaining));
		remaining = temp;
	}
	free(buffer);
	buffer = NULL;
	return (remaining);
}

char	*ft_get_line(char *remaining)
{
	int		i;
	char	*line;

	i = 0;
	if (!remaining[i])
		return (NULL);
	while (remaining[i] && remaining[i] != '\n')
		i++;
	line = (char *)malloc(sizeof(char) * (i + 2));
	if (!line)
		return (NULL);
	i = 0;
	while (remaining[i] && remaining[i] != '\n')
	{
		line[i] = remaining[i];
		i++;
	}
	if (remaining[i] == '\n')
	{
		line[i] = remaining[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

char	*ft_new_remaining(char *remaining)
{
	int		i;
	int		j;
	char	*new_remaining;

	i = 0;
	while (remaining[i] && remaining[i] != '\n')
		i++;
	if (!remaining[i])
	{
		free(remaining);
		return (NULL);
	}
	new_remaining = (char *)malloc(sizeof(char)
			* (ft_strlen_gnl(remaining) - i + 1));
	if (!new_remaining)
		return (NULL);
	i++;
	j = 0;
	while (remaining[i])
		new_remaining[j++] = remaining[i++];
	new_remaining[j] = '\0';
	free(remaining);
	return (new_remaining);
}

char	*get_next_line(int fd)
{
	static char	*remaining;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	remaining = ft_read_to_remaining(fd, remaining);
	if (!remaining)
		return (NULL);
	line = ft_get_line(remaining);
	remaining = ft_new_remaining(remaining);
	return (line);
}
