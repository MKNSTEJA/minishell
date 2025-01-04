/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 16:19:35 by ykhattab          #+#    #+#             */
/*   Updated: 2025/01/03 23:09:42 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

char	*ft_strchr(const char *str, int c)
{
	unsigned char	uc;

	uc = (unsigned char)c;
	while (*str)
	{
		if ((unsigned char)*str == uc)
		{
			return ((char *)str);
		}
		str++;
	}
	if (uc == '\0')
	{
		return ((char *)str);
	}
	return (NULL);
}
