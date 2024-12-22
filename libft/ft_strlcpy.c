/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 20:37:25 by ykhattab          #+#    #+#             */
/*   Updated: 2024/10/11 18:45:15 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	remaining;
	size_t	src_len;

	remaining = size;
	src_len = ft_strlen(src);
	if (size > 0)
	{
		while (*src && remaining > 1)
		{
			*dest++ = *src++;
			remaining--;
		}
		*dest = '\0';
	}
	return (src_len);
}
