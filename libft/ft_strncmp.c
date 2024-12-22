/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 18:46:11 by ykhattab          #+#    #+#             */
/*   Updated: 2024/10/11 18:46:12 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

int	ft_strncmp(const char *str1, const char *str2, size_t n)
{
	size_t	count;

	count = 0;
	while (*str1 && *str2 && count < n)
	{
		if (*str1 != *str2)
		{
			return ((unsigned char)*str1 - (unsigned char)*str2);
		}
		str1++;
		str2++;
		count++;
	}
	if (count < n)
	{
		return ((unsigned char)*str1 - (unsigned char)*str2);
	}
	return (0);
}
