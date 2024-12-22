/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykhattab <ykhattab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 16:23:29 by ykhattab          #+#    #+#             */
/*   Updated: 2024/10/11 17:51:46 by ykhattab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>

void	*ft_memchr(const void *ptr, int value, size_t n)
{
	size_t				limit;
	const unsigned char	*p;

	limit = 0;
	p = (const unsigned char *)ptr;
	while (limit < n)
	{
		if (p[limit] == (unsigned char)value)
			return ((void *)&p[limit]);
		limit++;
	}
	return (NULL);
}
